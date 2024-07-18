#include "Utility.h"
#include <thread>

std::string Utility::CreateRandomString(int length) {
    std::string randomString;

    char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL));

    randomString.reserve(length);

    for (int i = 0; i < length; ++i)
        randomString += alphanum[rand() % (sizeof(alphanum) - 1)];

    return randomString;
}