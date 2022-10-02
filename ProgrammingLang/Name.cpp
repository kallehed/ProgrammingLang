#include "Name.h"

#include "Util.h"

bool Name::operator==(const Name& other) const
{
    return this->number == other.number;
}

size_t Name::HashFunction::operator()(const Name& obj) const
{
    return std::hash<int64_t>()(obj.number);
}

// assume first character is FIRST OF VAR
Name_And_Length Name_Util::extract_name(const char* const text)
{
    int length = Util::len_while_condition(text, Util::legal_var_name_char);
    if (length > 8) { for (;; ) {}; }

    Name name;
    name.number = 0;

    for (int i = 0; i < length; ++i) {
        name.chars[i] = text[i];
    }

    return { name, length };
}