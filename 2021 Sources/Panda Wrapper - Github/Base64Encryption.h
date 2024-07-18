#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <vector>

using namespace std;


static std::string base64_encode(const std::string &in) {

	std::string out;

	int val = 0, valb = -6;
	for (int jj = 0; jj < in.size(); jj++) {
		char c = in[jj];
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}

static std::string base64_decode(const std::string &in) {

	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

	int val = 0, valb = -8;
	for (int jj = 0; jj < in.size(); jj++) {
		char c = in[jj];
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}

std::string AVAILABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

int index(char c) {
	for (int ii = 0; ii < AVAILABLE_CHARS.size(); ii++) {
		if (AVAILABLE_CHARS[ii] == c) {
			// std::cout << ii << " " << c << std::endl;
			return ii;
		}
	}
	return -1;
}


std::string extend_key(std::string& msg, std::string& key) {
	//generating new key
	int msgLen = msg.size();
	std::string newKey(msgLen, 'x');
	int keyLen = key.size(), i, j;
	for (i = 0, j = 0; i < msgLen; ++i, ++j) {
		if (j == keyLen)
			j = 0;

		newKey[i] = key[j];
	}
	newKey[i] = '\0';
	return newKey;
}


std::string encrypt_vigenere(std::string& msg, std::string& key) {
	int msgLen = msg.size(), keyLen = key.size(), i, j;
	std::string encryptedMsg(msgLen, 'x');
	// char newKey[msgLen], encryptedMsg[msgLen], decryptedMsg[msgLen];

	std::string newKey = extend_key(msg, key);

	//encryption
	for (i = 0; i < msgLen; ++i) {
		// std::cout << msg[i] << " " << isalnum(msg[i]) << std::endl;
		if (isalnum(msg[i]) or msg[i] == ' ') {
			encryptedMsg[i] = AVAILABLE_CHARS[((index(msg[i]) + index(newKey[i])) % AVAILABLE_CHARS.size())];
		}
		else {
			encryptedMsg[i] = msg[i];
		}
	}

	encryptedMsg[i] = '\0';
	return encryptedMsg;
}

std::string decrypt_vigenere(std::string& encryptedMsg, std::string& newKey) {
	// decryption
	int msgLen = encryptedMsg.size();
	std::string decryptedMsg(msgLen, 'x');
	int i;
	for (i = 0; i < msgLen; ++i) {
		if (isalnum(encryptedMsg[i]) or encryptedMsg[i] == ' ') {
			decryptedMsg[i] = AVAILABLE_CHARS[(((index(encryptedMsg[i]) - index(newKey[i])) + AVAILABLE_CHARS.size()) % AVAILABLE_CHARS.size())];
		}
		else {
			decryptedMsg[i] = encryptedMsg[i];
		}
	}
	decryptedMsg[i] = '\0';
	return decryptedMsg;
}

std::string encrypt(std::string& msg, std::string key) {
	std::string b64_str = base64_encode(msg);
	std::string vigenere_msg = encrypt_vigenere(b64_str, key);
	return vigenere_msg;
}


std::string decrypt(std::string& encrypted_msg, std::string key) {
	std::string newKey = extend_key(encrypted_msg, key);
	std::string b64_encoded_str = decrypt_vigenere(encrypted_msg, newKey);
	std::string b64_decode_str = base64_decode(b64_encoded_str);
	return b64_decode_str;
}