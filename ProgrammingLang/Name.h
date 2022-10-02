#pragma once
#include <array>
#include <cstdint>

struct Name {
    union {
        std::array<char, 8> chars;
        int64_t number;
    };

    bool operator==(const Name& other) const;

    struct HashFunction
    {
        size_t operator()(const Name& obj) const;
    };

    // assume first character is FIRST OF VAR
    
};

struct Name_And_Length {
    Name name;
    int length;
};

namespace Name_Util {
    // assume first character is FIRST OF VAR
    Name_And_Length extract_name(const char* const text);
}

