#include "crypt.h"

namespace crypt
{
    namespace base64
    {
        std::string encode(const std::string& data)
        {
            auto b64 = BIO_new(BIO_f_base64());
            BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

            auto mem = BIO_new(BIO_s_mem());
            BIO_push(b64, mem);

            BIO_write(b64, data.data(), static_cast<int>(data.length()));
            BIO_flush(b64);

            char* encoded_data{};
            std::string encoded(encoded_data, BIO_get_mem_data(mem, &encoded_data));

            BIO_free_all(b64);

            return encoded;
        }

        std::string decode(const std::string& data)
        {
            auto b64 = BIO_new(BIO_f_base64());
            BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

            auto mem = BIO_new_mem_buf(data.data(), static_cast<int>(data.length()));
            BIO_push(b64, mem);

            std::string decoded(data.length(), '\0');
            decoded.resize(BIO_read(b64, decoded.data(), static_cast<int>(decoded.length())));

            BIO_free_all(b64);

            return decoded;
        }
    }

    std::pair<std::string, std::string> encrypt(const std::string& data, const std::string& key, const std::string& iv, const std::string& mode)
    {
        std::string empty_iv = iv.empty() ? generatebytes(16) : iv;

        const EVP_CIPHER* cipher{};

        if (mode == "CBC")
        {
            cipher = EVP_aes_256_cbc();
        }
        else if (mode == "ECB")
        {
            cipher = EVP_aes_256_ecb();
        }
        else if (mode == "CTR")
        {
            cipher = EVP_aes_256_ctr();
        }
        else if (mode == "CFB")
        {
            cipher = EVP_aes_256_cfb();
        }
        else if (mode == "OFB")
        {
            cipher = EVP_aes_256_ofb();
        }
        else if (mode == "GCM")
        {
            cipher = EVP_aes_256_gcm();
        }
        else
        {
            std::cout << "invalid cipher mode" << std::endl;
        }

        return std::make_pair(base64::encode(perform_encryption(cipher, data, key, empty_iv)), empty_iv);
    }

    std::string decrypt(const std::string& data, const std::string& key, const std::string& iv, const std::string& mode)
    {
        const EVP_CIPHER* cipher{};

        if (mode == "CBC")
        {
            cipher = EVP_aes_256_cbc();
        }
        else if (mode == "ECB")
        {
            cipher = EVP_aes_256_ecb();
        }
        else if (mode == "CTR")
        {
            cipher = EVP_aes_256_ctr();
        }
        else if (mode == "CFB")
        {
            cipher = EVP_aes_256_cfb();
        }
        else if (mode == "OFB")
        {
            cipher = EVP_aes_256_ofb();
        }
        else if (mode == "GCM")
        {
            cipher = EVP_aes_256_gcm();
        }
        else
        {
            std::cout << "invalid cipher mode" << std::endl;
        }

        return perform_decryption(cipher, data, key, iv);
    }

    std::string hash(const std::string& data, const std::string& algorithm)
    {
        const EVP_MD* md{};

        if (algorithm == "sha1")
        {
            md = EVP_sha1();
        }
        else if (algorithm == "sha384")
        {
            md = EVP_sha384();
        }
        else if (algorithm == "sha512")
        {
            md = EVP_sha512();
        }
        else if (algorithm == "md5")
        {
            md = EVP_md5();
        }
        else if (algorithm == "sha256")
        {
            md = EVP_sha256();
        }
        else if (algorithm == "sha3-224")
        {
            md = EVP_sha3_224();
        }
        else if (algorithm == "sha3-256")
        {
            md = EVP_sha3_256();
        }
        else if (algorithm == "sha3-512")
        {
            md = EVP_sha3_512();
        }
        else
        {
            std::cout << "invalid hash algorithm" << std::endl;
        }

        return perform_hash(md, data);
    }

    std::string generatebytes(int length)
    {
        std::string bytes(length, 0);
        if (RAND_bytes(reinterpret_cast<uint8_t*>(bytes.data()), length) != 1)
        {
            std::cout << "failed to generate random bytes" << std::endl;
        }
        return base64::encode(bytes);
    }

    std::string perform_encryption(const EVP_CIPHER* cipher, const std::string& data, const std::string& key, const std::string& iv)
    {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, cipher, 0, reinterpret_cast<const uint8_t*>(key.data()), reinterpret_cast<const uint8_t*>(iv.data()));

        if (EVP_CIPHER_CTX_mode(ctx) == EVP_CIPH_GCM_MODE)
        {
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, 0);
        }

        std::string encrypted(data.length() + EVP_CIPHER_block_size(cipher), '\0');

        int actual_size, final_size;
        EVP_EncryptUpdate(ctx, reinterpret_cast<uint8_t*>(encrypted.data()), &actual_size, reinterpret_cast<const uint8_t*>(data.c_str()), static_cast<int>(data.length()));
        EVP_EncryptFinal_ex(ctx, reinterpret_cast<uint8_t*>(encrypted.data()) + actual_size, &final_size);
        encrypted.resize(actual_size + final_size);

        return encrypted;
    }

    std::string perform_decryption(const EVP_CIPHER* cipher, const std::string& data, const std::string& key, const std::string& iv)
    {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx, cipher, 0, reinterpret_cast<const uint8_t*>(key.data()), reinterpret_cast<const uint8_t*>(iv.data()));

        if (EVP_CIPHER_CTX_mode(ctx) == EVP_CIPH_GCM_MODE)
        {
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, 0);
        }

        std::string decrypted(data.length(), '\0');

        int actual_size, final_size;
        EVP_DecryptUpdate(ctx, reinterpret_cast<uint8_t*>(decrypted.data()), &actual_size, reinterpret_cast<const uint8_t*>(base64::decode(data).data()), base64::decode(data).length());
        EVP_DecryptFinal_ex(ctx, reinterpret_cast<uint8_t*>(decrypted.data() + actual_size), &final_size);
        decrypted.resize(actual_size + final_size);

        EVP_CIPHER_CTX_free(ctx);

        return decrypted;
    }

    std::string perform_hash(const EVP_MD* md, const std::string& data)
    {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();

        uint32_t hash_length;
        uint8_t hash[EVP_MAX_MD_SIZE];

        EVP_DigestInit_ex(ctx, md, 0);
        EVP_DigestUpdate(ctx, data.data(), data.length());
        EVP_DigestFinal_ex(ctx, hash, &hash_length);

        EVP_MD_CTX_free(ctx);

        std::stringstream stream;

        for (uint32_t i = 0; i < hash_length; i++)
        {
            stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(hash[i]);
        }

        return stream.str();
    }
}