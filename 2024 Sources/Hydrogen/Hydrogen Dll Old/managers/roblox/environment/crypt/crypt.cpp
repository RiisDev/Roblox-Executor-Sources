#include "crypt.hpp"

#include <managers/managers.hpp>
#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>
#include <Luau/lmem.h>

#include <cryptopp/cryptlib.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/blowfish.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <cryptopp/hex.h>
#include <cryptopp/gcm.h>
#include <cryptopp/eax.h>
#include <cryptopp/md5.h>
#include <cryptopp/rsa.h>

enum class aes_type_t
{
	CBC, ECB, CTR, CFB, OFB, GCM
};

std::unordered_map<std::string, aes_type_t> aes_type_map{
	{ "CBC", aes_type_t::CBC },
	//{ "ECB", aes_type_t::ECB },
	{ "CTR", aes_type_t::CTR },
	{ "CFB", aes_type_t::CFB },
	{ "OFB", aes_type_t::OFB },
	{ "GCM", aes_type_t::GCM }
};

enum class hash_type_t
{
	sha1, sha384, sha512, md5, sha256, sha3_224, sha3_256, sha3_512
};

std::unordered_map<std::string, hash_type_t> hash_type_map{
	{ "sha1", hash_type_t::sha1 },
	{ "sha384", hash_type_t::sha384 },
	{ "sha512", hash_type_t::sha512 },
	{ "md5", hash_type_t::md5 },
	{ "sha256", hash_type_t::sha256 },
	{ "sha3-224", hash_type_t::sha3_224 },
	{ "sha3-256", hash_type_t::sha3_256 },
	{ "sha3-512", hash_type_t::sha3_512 }
};

std::string generate_iv()
{
	std::uint8_t iv[16];

	CryptoPP::AutoSeededRandomPool pool{};
	pool.GenerateBlock(iv, sizeof(iv));

	return std::string{ (const char*)iv, sizeof(iv) };
}

std::string generate_key()
{
	std::uint8_t key[32];

	CryptoPP::AutoSeededRandomPool pool{};
	pool.GenerateBlock(key, sizeof(key));

	return std::string{ (const char*)key, sizeof(key) };
}

std::string b64enc(std::uint8_t* data, std::size_t sz)
{
	CryptoPP::Base64Encoder encoder{ nullptr, false };
	encoder.Put(data, sz);
	encoder.MessageEnd();

	std::string encoded;

	const auto size = encoder.MaxRetrievable();
	if (size)
	{
		encoded.resize(size);
		encoder.Get((byte*)&encoded[0], encoded.size());

		return encoded;
	}

	return encoded;
}

std::string b64dec(std::uint8_t* data, std::size_t sz)
{
	CryptoPP::Base64Decoder decoder{};
	decoder.Put(data, sz);
	decoder.MessageEnd();

	std::string decoded;

	const auto size = decoder.MaxRetrievable();
	if (size)
	{
		decoded.resize(size);
		decoder.Get((byte*)&decoded[0], decoded.size());

		return decoded;
	}

	return decoded;
}

template<typename algo_t>
std::string hash_algo(const char* data, std::size_t data_sz)
{
	std::string output;

	algo_t hash;
	CryptoPP::StringSource(std::string(data, data_sz), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output), false)));

	return output;
}

template<typename algo_t>
std::string transform(std::uint8_t* data, std::size_t sz, std::uint8_t* key, std::size_t key_sz, std::uint8_t* iv, std::size_t iv_sz)
{
	std::string output;

	algo_t algo;
	algo.SetKeyWithIV((std::uint8_t*)(key), key_sz, (std::uint8_t*)(iv), iv_sz);

	CryptoPP::StringSource(std::string((const char*)data, sz), true, new CryptoPP::StreamTransformationFilter(algo, new CryptoPP::StringSink(output)));

	return output;
}

template<typename algo_t>
std::string enc_adf(std::uint8_t* data, std::size_t sz, std::uint8_t* key, std::size_t key_sz, std::uint8_t* iv, std::size_t iv_sz)
{
	std::string output;

	algo_t algo;
	algo.SetKeyWithIV((std::uint8_t*)(key), key_sz, (std::uint8_t*)(iv), iv_sz);

	CryptoPP::AuthenticatedDecryptionFilter aef(algo, new CryptoPP::StringSink(output));

	aef.Put((std::uint8_t*)(data), sz);
	aef.MessageEnd();

	return output;
}

template<typename algo_t>
std::string enc_aef(std::uint8_t* data, std::size_t sz, std::uint8_t* key, std::size_t key_sz, std::uint8_t* iv, std::size_t iv_sz)
{
	std::string output;

	algo_t algo;
	algo.SetKeyWithIV((std::uint8_t*)(key), key_sz, (std::uint8_t*)(iv), iv_sz);

	CryptoPP::AuthenticatedEncryptionFilter aef(algo, new CryptoPP::StringSink(output));

	aef.Put((std::uint8_t*)(data), sz);
	aef.MessageEnd();

	return output;
}

std::int32_t roblox::environment::crypt::base_64_encode(lua_State* state)
{
	std::size_t len;
	const auto str = luaL_checklstring(state, 1, &len);

	try
	{
		const auto encoded = b64enc((std::uint8_t*)str, len);

		lua_pushlstring(state, encoded.c_str(), encoded.size());

		return 1;
	}
	catch (const CryptoPP::Exception& e)
	{
		luaL_error(state, e.what());
	}

	return 0;
}

std::int32_t roblox::environment::crypt::base_64_decode(lua_State* state)
{
	std::size_t len;
	const auto str = luaL_checklstring(state, 1, &len);

	try
	{
		const auto decoded = b64dec((std::uint8_t*)str, len);

		lua_pushlstring(state, decoded.c_str(), decoded.size());

		return 1;
	}
	catch (const CryptoPP::Exception& e)
	{
		luaL_error(state, e.what());
	}

	return 0;
}


std::int32_t roblox::environment::crypt::encrypt(lua_State* state)
{
	std::size_t data_sz;
	const auto data = luaL_checklstring(state, 1, &data_sz);

	std::size_t key_sz;
	const auto key = luaL_checklstring(state, 2, &key_sz);
	const auto decoded_key = b64dec((std::uint8_t*)key, key_sz);

	const auto get_iv = [&]() -> std::string
	{
		if (lua_gettop(state) >= 3 && !lua_isnil(state, 3))
		{
			std::size_t temp_sz;
			const auto temp_data = luaL_checklstring(state, 3, &temp_sz);

			return b64dec((std::uint8_t*)temp_data, temp_sz);
		}

		return generate_iv();
	};

	const auto iv = get_iv();

	const auto mode = lua_gettop(state) == 4 ? luaL_checkstring(state, 4) : "CBC";

	if (!aes_type_map.contains(mode))
		luaL_error(state, "Invalid aes type");

	switch (aes_type_map.at(mode))
	{
	case aes_type_t::CBC:
	{
		const auto res = transform<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>((std::uint8_t*)data, data_sz, (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)iv.c_str(), iv.length());

		const auto b64_res = b64enc((std::uint8_t*)res.c_str(), res.length());

		lua_pushlstring(state, b64_res.c_str(), b64_res.length());

		break;
	}
	//case aes_type_t::ECB:
	//{
	//	const auto res = transform<CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption>((std::uint8_t*)data, data_sz, (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)iv.c_str(), iv.length());

	//	const auto b64_res = b64enc((std::uint8_t*)res.c_str(), res.length());

	//	lua_pushlstring(state, b64_res.c_str(), b64_res.length());

	//	break;
	//}
	case aes_type_t::CTR:
	{
		const auto res = transform<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>((std::uint8_t*)data, data_sz, (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)iv.c_str(), iv.length());

		const auto b64_res = b64enc((std::uint8_t*)res.c_str(), res.length());

		lua_pushlstring(state, b64_res.c_str(), b64_res.length());

		break;
	}
	case aes_type_t::CFB:
	{
		const auto res = transform<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption>((std::uint8_t*)data, data_sz, (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)iv.c_str(), iv.length());

		const auto b64_res = b64enc((std::uint8_t*)res.c_str(), res.length());

		lua_pushlstring(state, b64_res.c_str(), b64_res.length());

		break;
	}
	case aes_type_t::OFB:
	{
		const auto res = transform<CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption>((std::uint8_t*)data, data_sz, (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)iv.c_str(), iv.length());

		const auto b64_res = b64enc((std::uint8_t*)res.c_str(), res.length());

		lua_pushlstring(state, b64_res.c_str(), b64_res.length());

		break;
	}
	case aes_type_t::GCM:
	{
		const auto res = enc_aef<CryptoPP::GCM<CryptoPP::AES>::Encryption>((std::uint8_t*)data, data_sz, (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)iv.c_str(), iv.length());

		const auto b64_res = b64enc((std::uint8_t*)res.c_str(), res.length());

		lua_pushlstring(state, b64_res.c_str(), b64_res.length());

		break;
	}
	}

	const auto b64_iv = b64enc((std::uint8_t*)iv.c_str(), iv.length());

	lua_pushlstring(state, b64_iv.c_str(), b64_iv.length());

	return 2;
}

std::int32_t roblox::environment::crypt::decrypt(lua_State* state)
{
	std::size_t data_sz;
	const auto data = luaL_checklstring(state, 1, &data_sz);
	const auto decoded_data = b64dec((std::uint8_t*)data, data_sz);

	std::size_t key_sz;
	const auto key = luaL_checklstring(state, 2, &key_sz);
	const auto decoded_key = b64dec((std::uint8_t*)key, key_sz);

	std::size_t iv_sz;
	const auto iv = luaL_checklstring(state, 3, &iv_sz);
	const auto decoded_iv = b64dec((std::uint8_t*)iv, iv_sz);

	const auto mode = luaL_checkstring(state, 4);

	if (!aes_type_map.contains(mode))
		luaL_error(state, "Invalid aes type");

	switch (aes_type_map.at(mode))
	{
	case aes_type_t::CBC:
	{
		const auto res = transform<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>((std::uint8_t*)decoded_data.c_str(), decoded_data.length(), (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)decoded_iv.c_str(), decoded_iv.length());

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	//case aes_type_t::ECB:
	//{
	//	const auto res = transform<CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption>((std::uint8_t*)decoded_data.c_str(), decoded_data.length(), (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)decoded_iv.c_str(), decoded_iv.length());

	//	lua_pushlstring(state, res.c_str(), res.length());

	//	return 1;
	//}
	case aes_type_t::CTR:
	{
		const auto res = transform<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>((std::uint8_t*)decoded_data.c_str(), decoded_data.length(), (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)decoded_iv.c_str(), decoded_iv.length());

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case aes_type_t::CFB:
	{
		const auto res = transform<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption>((std::uint8_t*)decoded_data.c_str(), decoded_data.length(), (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)decoded_iv.c_str(), decoded_iv.length());

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case aes_type_t::OFB:
	{
		const auto res = transform<CryptoPP::OFB_Mode<CryptoPP::AES>::Decryption>((std::uint8_t*)decoded_data.c_str(), decoded_data.length(), (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)decoded_iv.c_str(), decoded_iv.length());

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case aes_type_t::GCM:
	{
		const auto res = enc_adf<CryptoPP::GCM<CryptoPP::AES>::Decryption>((std::uint8_t*)decoded_data.c_str(), decoded_data.length(), (std::uint8_t*)decoded_key.c_str(), decoded_key.length(), (std::uint8_t*)decoded_iv.c_str(), decoded_iv.length());

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	}

	return 0;
}

std::int32_t roblox::environment::crypt::generatekey(lua_State* state)
{
	const auto encoded = b64enc((std::uint8_t*)generate_key().c_str(), 32);

	lua_pushlstring(state, encoded.c_str(), encoded.size());

	return 1;
}

std::int32_t roblox::environment::crypt::generatebytes(lua_State* state)
{
	const auto count = luaL_checkinteger(state, 1);

	std::uint8_t* bytes = new std::uint8_t[count];

	CryptoPP::AutoSeededRandomPool pool{};
	pool.GenerateBlock(bytes, count);

	const auto encoded = b64enc(bytes, count);

	delete[] bytes;

	lua_pushlstring(state, encoded.c_str(), encoded.size());

	return 1;
}

std::int32_t roblox::environment::crypt::hash(lua_State* state)
{
	std::size_t sz;
	const auto data = luaL_checklstring(state, 1, &sz);

	const auto mode = luaL_checkstring(state, 2);

	if (!hash_type_map.contains(mode))
		luaL_error(state, "Invalid hash type");

	switch (hash_type_map.at(mode))
	{
	case hash_type_t::md5:
	{
		const auto res = hash_algo<CryptoPP::MD5>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha1:
	{
		const auto res = hash_algo<CryptoPP::SHA1>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha256:
	{
		const auto res = hash_algo<CryptoPP::SHA256>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha384:
	{
		const auto res = hash_algo<CryptoPP::SHA384>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha512:
	{
		const auto res = hash_algo<CryptoPP::SHA512>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha3_224:
	{
		const auto res = hash_algo<CryptoPP::SHA3_224>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha3_256:
	{
		const auto res = hash_algo<CryptoPP::SHA3_256>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	case hash_type_t::sha3_512:
	{
		const auto res = hash_algo<CryptoPP::SHA3_512>(data, sz);

		lua_pushlstring(state, res.c_str(), res.length());

		return 1;
	}
	}

	return 0;
}
