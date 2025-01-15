#pragma once
#include <string>
#include <iostream>

#include <iomanip>
#include <sstream>

#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/buffer.h>

namespace crypt
{
	namespace base64
	{
		extern std::string encode(const std::string& data);

		extern std::string decode(const std::string& data);
	}

	extern std::string generatebytes(int length);

	extern std::string perform_hash(const EVP_MD* md, const std::string& data);

	extern std::string hash(const std::string& data, const std::string& algorithm);

	extern std::string perform_decryption(const EVP_CIPHER* cipher, const std::string& data, const std::string& key, const std::string& iv);

	extern std::string decrypt(const std::string& data, const std::string& key, const std::string& iv, const std::string& mode);

	extern std::string perform_encryption(const EVP_CIPHER* cipher, const std::string& data, const std::string& key, const std::string& iv);

	extern std::pair<std::string, std::string> encrypt(const std::string& data, const std::string& key, const std::string& iv, const std::string& mode);
}