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
Name_And_Where Name_Util::extract_name(Where w)
{
    Where where_not_name = Util::where_while_condition(w, &Util::legal_name_char);

    int length = where_not_name - w;

    if (length > 8) { for (;; ) {}; }

    Name name;
    name.number = 0;

    for (int i = 0; i < length; ++i) {
        name.chars[i] = code[w + i];
    }

    return { name, where_not_name };
}