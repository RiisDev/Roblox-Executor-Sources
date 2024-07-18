/* Written by Gogo1000, iivillian, 0x90 */

#include "memcheck.hpp"
#include "../utils/utils.hpp"

constexpr auto hasher_list_length = 0x1E;

std::vector<memcheck::chunk_t> chunk_data;

/* Spastic hack to determine the operation that hasher uses against the hash and the seed */
memcheck::operation memcheck::determine_operation(std::uintptr_t silent_hasher)
{
	/* Iterate untill epilogue */
	while(*reinterpret_cast<std::uintptr_t*>(++silent_hasher) != 0xC35DE58B) {}

	/* Check for one of the operations used. The operation between hash and seed is always the last arithmetic instruction in the function */
	while (*reinterpret_cast<std::uint8_t*>(--silent_hasher) != 0x33 && *reinterpret_cast<std::uint8_t*>(silent_hasher) != 0x03 && *reinterpret_cast<std::uint8_t*>(silent_hasher) != 0x2b) {}

	return (*reinterpret_cast<memcheck::operation*>(silent_hasher));
}

std::uintptr_t* memcheck::populate_hash_list()
{
	const auto cur_seed = *reinterpret_cast<std::uintptr_t*>(seed_address);

	/* Setting seed to 0 and calling main hasher will result in an easier calculation to recieve unseeded hash */
	*reinterpret_cast<std::uintptr_t*>(seed_address) = 0;

	std::uintptr_t dtc[100]{};

	using core_hasher_t = int(__thiscall*)(std::uintptr_t* arg);
	const auto core_hasher_fn = reinterpret_cast<core_hasher_t>(core_hasher);

	/* Invoking core hasher with an empty arg, each iteration it will set the desired hash to an array within the passed allocation */
	for (auto i = 0; i < hasher_list_length; ++i)
		core_hasher_fn(dtc);

	/* Restore the seed so that the next hash will be a correct calculation */
	*reinterpret_cast<std::uintptr_t*>(seed_address) = cur_seed;

	return dtc;
}

/* Roblox contains a single active silent hasher with multiple unactive ones that have a similar signature, the difference between these hashers is the operation used against the hash and the seed,
	The server decides the active hasher and it will always be the same even upon rejoining aslong as it is the same server.
	we can retrieve the active silent hasher the same way it is grabbed to be invoked by the client */
std::uintptr_t memcheck::get_active_silent_hasher()
{
	const auto a3 = *reinterpret_cast<std::uint32_t*>(get_client_replicator() + 0x2BC4);
	const auto v6 = (0x1594FE2C * a3 - 0x32E7FDBF) ^ (0xD2C21B15 - 0x344B5409 * a3);

	return m_base - 0x400000 + v6 + 2;
}

/* Lookup the current chunk being accessed within our chunk map and use the proper operation against the server determined seed */
memcheck::operation op;
std::uint32_t silent_hasher_hook(std::uintptr_t chunk_start, std::uint32_t chunk_size, std::uint32_t unk, std::uint32_t seed)
{
	for (const auto& chunk : chunk_data)
	{
		if (chunk.start == chunk_start)
		{
			switch (op)
			{
				case memcheck::operation::xor_t:
					return chunk.hash ^ seed;
				case memcheck::operation::add_t:
					return chunk.hash + seed;
				case memcheck::operation::sub_t:
					return chunk.hash - seed;
			}
		}
	}

	return 0;
}

std::uintptr_t seed_end;

auto hash = 0;

/* Replicate the xor calculation the normal hasher uses, but instead using a previous hash of the current chunk we retrieved that is clean along with the correct server seed */
_declspec(naked) void main_hasher_hook()
{
	std::uintptr_t curr_chunk;

	__asm
	{
		mov curr_chunk, edi

		pushad
		pushfd
	}

	hash = chunk_data[curr_chunk].core_hash;

	__asm
	{
		popfd
		popad

		mov esi, hash /* Insert clean hash into esi as it is used later */
		xor ecx, esi /* ecx contains current seed sent by server, xor it by the clean hash of the current chunk */
		mov[ebx + edi * 4 + 0x60], ecx

		jmp seed_end
	}
}

#pragma section(".text")
__declspec(allocate(".text")) std::uint8_t stub[] = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 };

void memcheck::initiate()
{
	/* Stall the memory checker job so that hasher doesnt fire with the improper seed as we set it to 0 */
	const auto mcj = get_job("US14116");

	constexpr std::uint8_t virtual_function_table_size = 7u, step_index = 5u;

	auto virtual_function_table = new void* [virtual_function_table_size];

	std::memcpy(virtual_function_table, *reinterpret_cast<std::uintptr_t**>(mcj), virtual_function_table_size * sizeof(void*));

	const auto step_original = reinterpret_cast<std::uintptr_t>(virtual_function_table[step_index]);

	virtual_function_table[step_index] = stub;

	*reinterpret_cast<std::uintptr_t*>(mcj) = reinterpret_cast<std::uintptr_t>(virtual_function_table);

	/* Collect hashes */
	const auto hasher_argument = populate_hash_list();

	using silent_hasher_t = std::uint32_t(*)(std::uintptr_t chunk_start, std::uint32_t chunk_size, std::uint32_t unk, std::uint32_t seed);
	const auto silent_hasher = reinterpret_cast<silent_hasher_t>(get_active_silent_hasher());

	for (auto i = 0; i < hasher_list_length; ++i)
	{
		const auto chunk_o = chunk_start[i];
		const auto chunk = m_base - 0x400000 + ((0x1594FE2C * chunk_o - 0x32E7FDBF) ^ (0xD2C21B15 - 0x344B5409 * chunk_o)) + 2;

		const auto size_o = size_start[i];
		const auto size = (0x1594FE2C * size_o - 0x32E7FDBF) ^ (0xD2C21B15 - 0x344B5409 * size_o);

		/* Get unseeded hash for silent hasher spoofing */
		const auto hash = silent_hasher(chunk, size, 0, 0);

		/* Read the hashes from the array we collected earlier, This contains semi unseeded hashes because we set base seed to 0 when collecting them. 
		   There is still some calculations that happen after that we can reverse due to base seed being 0 */
		const auto xored_hash = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(hasher_argument) + 0x60)[i];

		/* The calculation left over results in 0x68178A72, so we can do the inverse operation to retrieve a fully unseeded hash of the current chunk */
		const auto raw_hash = 0x68178A72 ^ xored_hash;

		/* Cache the silent and core checker unseeded hashes */
		chunk_data.emplace_back(chunk, size, hash, raw_hash);

		if constexpr (debug)
			std::printf("Chunk %x: 0x%X | Size: 0x%X | Hash: 0x%X | Core Hash: 0x%X\n", i, chunk_data[i].start, chunk_data[i].size, chunk_data[i].hash, chunk_data[i].core_hash);
	}

	/* Determine the operation we need to use in our silent hasher hook */
	op = determine_operation(reinterpret_cast<std::uintptr_t>(silent_hasher));

	utils::hook(reinterpret_cast<std::uintptr_t>(silent_hasher), reinterpret_cast<std::uintptr_t>(silent_hasher_hook));

	if constexpr (debug)
		std::printf("Current Hasher: 0x%p\n", silent_hasher);

	seed_end = core_hasher_seed + 0x6;
	utils::hook(core_hasher_seed, reinterpret_cast<std::uintptr_t>(main_hasher_hook), 0x6);

	/* Restore the memcheck job now that we have hooked */
	virtual_function_table[step_index] = reinterpret_cast<void*>(step_original);

	*reinterpret_cast<std::uintptr_t*>(mcj) = reinterpret_cast<std::uintptr_t>(virtual_function_table);
}