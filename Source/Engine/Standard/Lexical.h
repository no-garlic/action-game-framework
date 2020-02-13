
#ifndef __STD_LEXICAL_H_
#define __STD_LEXICAL_H_

#include "Type.h"

template <typename Target, typename Source>
Target LexicalCast(Source in)
{
    bool success;
    return LexicalCast<Target, Source>(in, success);
}

template <typename Target, typename Source>
Target LexicalCast(Source in, bool &success)
{
    StringStream interpreter;
    interpreter.unsetf(std::ios::skipws);

    Target result;

    if (sizeof(result) == sizeof(int))
    {
        memset(&result, 0, sizeof(result));
    }
    else if (sizeof(result) == sizeof(int *))
    {
        memset(&result, 0, sizeof(result));
    }

    success = true;
    if (!(interpreter << in) || !(interpreter >> result) || !(interpreter >> std::ws).eof())
    {
        success = false;
    }

    return result;
}

template <>
inline String LexicalCast<String, String>(String in, bool &success)
{
    success = true;
    return in;
}

template <>
inline String LexicalCast<String, String>(String in)
{
    return in;
}

template <>
inline bool LexicalCast<bool, String>(String in)
{
    String s = TrimString(in);

    if (s.length() == 0)
        return false;

    if (StrICmp(s, "false"))
        return false;

    if (StrICmp(s, "0"))
        return false;

    return true;
}

#endif // __STD_LEXICAL_H_
