#include <string>
#include <cstring>
#include <stdlib.h>
#include <time.h>

std::string AES256_Encrypt(std::string const & in2, std::string const & key);
std::string AES256_Decrypt(std::string const & in, std::string const & key);