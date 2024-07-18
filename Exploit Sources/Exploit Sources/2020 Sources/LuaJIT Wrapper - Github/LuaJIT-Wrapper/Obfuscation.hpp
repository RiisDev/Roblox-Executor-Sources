#pragma once
#include <string>
#include <algorithm>
#include "XorString.hpp"

std::string RandomString(unsigned int length)
{
    auto RandomCharacters = []() -> char
    {
        const char Characters[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t MaximumLength = (sizeof(Characters) - 1);
        return Characters[rand() % MaximumLength];
    };
    std::string EnryptedString(length, 0);
    std::generate_n(EnryptedString.begin(), length, RandomCharacters);
    return EnryptedString;
}

#define RAND_STR(l) RandomString(l)