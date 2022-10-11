#pragma once
#include <array>
#include <cstdint>

#include "misc.h"

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

struct Name_And_Where {
    Name _name;
    Where _w;
};

namespace Name_Util {
    // assume first character is FIRST OF VAR, returns where NOT name
    Name_And_Where extract_name(Where w);
}

