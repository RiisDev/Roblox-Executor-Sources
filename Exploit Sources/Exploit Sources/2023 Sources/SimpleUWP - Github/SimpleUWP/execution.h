#pragma once
#include "luau.h"
#include "compiler/BytecodeBuilder.h"
#include "compiler/Compiler.h"
#include "compressor/xxhash.h"
#include "compressor/zstd.h"

class GoofyEncoder : public Luau::BytecodeEncoder
{
	std::uint8_t encodeOp(const std::uint8_t AxonUpdated)
	{
		return AxonUpdated * 227;
	}
};

// NOTE: THIS DLL WAS MADE FOR LEARNING PURPOUSES SO I MADE IT UNDERSTANABLE
// LICENSED UNDER NOSKID LICENSE THAT MEANS IF YOU WILL USE THIS SOURCE IN YOUR EXXPLOIT YOUR FEDERAL PROTECTION WILL BE TURNED OFF AND YOU WILL BE IN JAIL

namespace TheMagiec
{
	// RONLOX DECOODING ALGROHIRM FRO BEST EXPLOITS
	__forceinline std::uintptr_t RobloxDecodeSecretThing(std::uintptr_t RobloxSecretAgent)
	{
		return (RobloxSecretAgent + 0xF4) - *reinterpret_cast<std::uintptr_t*>(RobloxSecretAgent + 0xF4);
	}

	// GET HTE ROBLOX MAGIC THINGYY IT USES FOR EXECHUOTOION
	__forceinline std::uintptr_t getMagicRobloxThingy()
	{
		// GET MOST IMPORTAENT ROBLOX THINGY
		std::uintptr_t RobloxImportantThingyThing = luau::getscheduler();
		// LOOP THROUGHR ROBLOX IMPORTANT THINGYS TO GET JOBS FROM SYNAPSE X
		for (std::uintptr_t RobloxIteratorJox_X = *reinterpret_cast<std::uintptr_t*>(RobloxImportantThingyThing + 0x134); RobloxIteratorJox_X != *reinterpret_cast<std::uintptr_t*>(RobloxImportantThingyThing + 0x138); RobloxIteratorJox_X += 8)
			// IF WE FIND SYNAPSE X CLMV WE USE IT FOR GETTING MAGIC
			if (std::uintptr_t RobloxJob_X = *reinterpret_cast<std::uintptr_t*>(RobloxIteratorJox_X); *reinterpret_cast<std::string*>(RobloxJob_X + 0x10) == "WaitingHybridScriptsJob")
				// DECODE SYNAPSEX+ROBLOX ENCODED MAGIC
				return RobloxDecodeSecretThing(*reinterpret_cast<std::uintptr_t*>(RobloxJob_X + 0x130));
	}

	// THE MAIN THINGY FOR DDE ROBLOX SKID EXEXUTION IN SEVRERY SKIDSPOIT ALSOI ITS BETTER THATN SYNAPSEX AND FASTER!
	__forceinline void DoTheMainMagicViaRobloxUninliedFunctionsAndSomeCompilingWithCompressionsAndTopDecremention(std::uintptr_t Watafak, std::string todo)
	{
		std::string	Goofy = todo;
		// CONVERT TODO TO BYTECODE
		GoofyEncoder RobloxSecretBuildLeakedEncoderFromSynapseXCLVM{};
		Goofy = Luau::compile(Goofy, {}, {}, &RobloxSecretBuildLeakedEncoderFromSynapseXCLVM);

		std::string output = "RSB1";
		std::size_t dataSize = Goofy.size();
		std::size_t maxSize = ZSTD_compressBound(dataSize);
		std::vector<char> compressed(maxSize);
		std::size_t compSize = ZSTD_compress(&compressed[0], maxSize, Goofy.c_str(), dataSize, ZSTD_maxCLevel());
		output.append(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
		output.append(&compressed[0], compSize);
		std::uint32_t firstHash = XXH32(&output[0], output.size(), 42U);
		std::uint8_t hashedBytes[4];
		memcpy(hashedBytes, &firstHash, sizeof(firstHash));
		for (std::size_t i = 0; i < output.size(); ++i)
			output[i] ^= hashedBytes[i % 4] + i * 41U;
		Goofy = output;

		// DONT CAHENG THE @Skid OR ITW ILL NOT WORK
		if (luau::load(Watafak, &Goofy, "@Skid", 0) == 0)
			// LOAD THE SCRIPTS USING CLVMO FROM SYNAPSE X
			luau::task::spawn(Watafak);
		else
			// IF YOU EXCUTE SCRIPTRER WITH ERROS WE PIRNT IT
			luau::print(3, reinterpret_cast<const char*>((*reinterpret_cast<RobloxCumTypeValue**>(Watafak + offsets::smell_fart::baes))->value.gc + 0x14));
		// ADD ROBLOX FIX TO WATAFACK TOP SO IT WILL WORK
		//*reinterpret_cast<std::uintptr_t*>(Watafak + offsets::smell_fart::top) -= 16; // ROBLOX FIX WATAFAK!!
	}
}