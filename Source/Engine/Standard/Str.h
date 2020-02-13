
#ifndef __STD_STR_H_
#define __STD_STR_H_

#include "Type.h"
#include <string>
#include <sstream>

typedef std::string                   String;
typedef std::stringstream             StringStream;
typedef Array<std::string>            StringArray;
typedef Map<std::string, std::string> StringMap;

inline String &PrintF(String &str, const char *format, ...);
inline String PrintF(const char *format, ...);

inline void ToUpper(String &str)
{
    strupr((char *)&str[0]);
}

inline String ToUpper(const String &str)
{
    String s = str;
    ToUpper(s);
    return s;
}

inline void ToLower(String &str)
{
    strlwr((char *)&str[0]);
}

inline String ToLower(const String &str)
{
    String s = str;
    ToLower(s);
    return s;
}

inline const String &ToString(const String &value)
{
    return value;
}

inline String ToString(int value)
{
    char buffer[32];
    if (sprintf(buffer, "%d", value) == -1)
        return "#ERROR#";

    return buffer;
}

inline String ToString(uint value)
{
    char buffer[32];
    if (sprintf(buffer, "%u", value) == -1)
        return "#ERROR#";

    return buffer;
}

inline String ToString(DWORD value)
{
    char buffer[32];
    if (sprintf(buffer, "%u", value) == -1)
        return "#ERROR#";

    return buffer;
}

inline String ToString(float value)
{
    char buffer[32];
    if (sprintf(buffer, "%f", value) == -1)
        return "#ERROR#";

    return buffer;
}

inline String ToString(bool value)
{
    char buffer[32];
    if (sprintf(buffer, "%d", (value ? 1 : 0)) == -1)
        return "#ERROR#";

    return buffer;
}

inline String ToString(const float2 &value)
{
    return PrintF("(%f, %f)", value.x, value.y);
}

inline String ToString(const float3 &value)
{
    return PrintF("(%f, %f, %f)", value.x, value.y, value.z);
}

inline String ToString(const float4 &value)
{
    return PrintF("(%f, %f, %f, %f)", value.x, value.y, value.z, value.w);
}

inline String ToString(const int2 &value)
{
    return PrintF("(%d, %d)", value.x, value.y);
}

inline String ToString(const int3 &value)
{
    return PrintF("(%d, %d, %d)", value.x, value.y, value.z);
}

inline String ToString(const int4 &value)
{
    return PrintF("(%d, %d, %d, %d)", value.x, value.y, value.z, value.w);
}

inline String ToString(const bool2 &value)
{
    return PrintF("(%s, %s)", value.x ? "true" : "false", value.y ? "true" : "false");
}

inline String ToString(const bool3 &value)
{
    return PrintF("(%s, %s, %s)", value.x ? "true" : "false", value.y ? "true" : "false", value.z ? "true" : "false");
}

inline String ToString(const bool4 &value)
{
    return PrintF("(%s, %s, %s, %s)", value.x ? "true" : "false", value.y ? "true" : "false", value.z ? "true" : "false", value.w ? "true" : "false");
}

enum PaddingType
{
    PT_Left = 1,            // Add padding to the left of the string
    PT_Right = 2,           // Add padding to the right of the string
    PT_Default = PT_Right   // The default padding type
};

inline String PadString(uint width, char character)
{
    String str;

    while (str.length() < width)
        str += character;

    return str;
}

inline String &PadString(String &str, uint newSize, PaddingType paddingType)
{
    if (str.length() < newSize)
    {
        // The string is shorter than the new size, so add whitespace
        switch (paddingType)
        {
        case PT_Left:
            // Padding before the string (for right justified text)
            str = PadString(newSize - str.length(), ' ') + str;
            break;

        case PT_Right:
            // Padding after the string (for left justified text)
            str.append(newSize - str.length(), ' ');
            break;
        }
    }
    else
    {
        // The string is already longer than the padding, so truncate it
        str = str.substr(0, newSize);
    }

    return str;
}

inline String PadString(const String &str, uint newSize, PaddingType paddingType)
{
    String outString = str;
    PadString(outString, newSize, paddingType);
    return outString;
}

inline String StrSpace(uint width)
{
    return PadString(width, ' ');
}

inline String &AppendInt(String &str, int i)
{
    char buffer[32];
    sprintf(buffer, "%d", i);
    str += buffer;
    return str;
}

inline String &AppendUInt(String &str, uint i)
{
    char buffer[32];
    sprintf(buffer, "%u", i);
    str += buffer;
    return str;
}

inline String &TrimString(String &str)
{
    if (str.empty())
        return str;

    while (!str.empty() && str.at(0) == ' ')
        str.erase(str.begin());

    if (str.empty())
        return str;

    while (str.at(str.size() - 1) == ' ')
        str = str.substr(0, str.size() - 1);

    return str;
}

inline String &TrimQuotes(String &str)
{
    if (str.empty())
        return str;

    while (str.at(0) == '"')
        str.erase(str.begin());

    if (str.empty())
        return str;

    while (str.at(str.size() - 1) == '"')
        str = str.substr(0, str.size() - 1);

    return str;
}

inline String &TrimEnd(String &str, char token)
{
    if (str.empty())
        return str;

    while (str.at(str.size() - 1) == token)
        str = str.substr(0, str.size() - 1);

    return str;
}

inline String &TrimStart(String &str, char token)
{
    if (str.empty())
        return str;

    while (str.at(0) == token)
        str.erase(str.begin());
    
    return str;
}

inline String &EraseChars(String &str, const String &chars)
{
    for (char c : chars)
    {
        for (size_t i = 0; i < str.length(); )
        {
            if (str.at(i) == c)
            {
                str.erase(i, 1);
            }
            else
            {
                ++i;
            }
        }
    }
    return str;
}

inline String GetQuotedString(const String &str)
{
    static const String::size_type npos = -1;

    String::size_type pos1;
    String::size_type pos2;

    pos1 = str.find_first_of("\"", 0);
    if (pos1 == npos)
        return "";

    pos2 = str.find_first_of("\"", pos1 + 1);
    if (pos2 == npos)
        return "";

    return str.substr(pos1 + 1, pos2 - (pos1 + 1));
}

inline void SplitString(const String &str, char token, String &left, String &right)
{
    size_t pos = str.find_first_of(token);

    if (pos == String::npos)
    {
        left = str;
        right = "";
        return;
    }

    left = str.substr(0, pos);
    right = str.substr(pos + 1);
}

inline StringArray Tokenize(const String &str, char token = ' ')
{
    StringArray tokens;
    String tmp(str);

    size_t pos = tmp.find_first_of(token);
    while (pos != String::npos)
    {
        String token_str = tmp.substr(0, pos);
        tmp = tmp.substr(pos + 1);

        if (token_str.size())
            tokens.push_back(token_str);

        pos = tmp.find_first_of(token);
    }

    if (tmp.size())
        tokens.push_back(tmp);

    return tokens;
}

inline String GetStringArrayStrings(const StringArray &stringArray)
{
    StringStream str;
    str << "{ ";

    StringArray::ConstIterator iter(stringArray.begin()), first(iter), end(stringArray.end());
    for (; iter != end; ++iter)
    {
        if (iter != first)
        {
            str << ", ";
        }

        str << *iter;
    }

    str << " }";
    return str.str();
}

inline StringArray &TrimStringsInArray(StringArray &stringArray)
{
    StringArray::iterator iter(stringArray.begin()), end(stringArray.end());
    for (; iter != end; ++iter)
    {
        TrimString(*iter);
    }

    return stringArray;
}

inline void SplitFilename(const String &str, String &filename, String &extension)
{
    // See if the string has a . in it
    int pos = (int)str.find_last_of('.', str.length());
    if (pos == -1)
        return;

    filename = str.substr(0, pos);
    extension = str.substr(pos + 1);
}

inline void SplitPath(const String &str, String &path, String &name)
{
    path = "";
    name = "";

    if (str.empty())
        return;

    int fs = (int)str.find_last_of('/', str.size());
    int bs = (int)str.find_last_of('\\', str.size());

    int last = Max<int>(fs, bs);
    if (last == -1)
    {
        name = str;
    }
    else
    {
        path = str.substr(0, last);
        name = str.substr(last + 1, str.size());
    }
}

inline String &PrintF(String &str, const char *format, ...)
{
    va_list args;
    int     len;
    char   *buffer;

    va_start(args, format);
    len = _vscprintf(format, args) + 1;
    buffer = new char[len * sizeof(char)];
    vsprintf(buffer, format, args);

    str = String(buffer);
    delete[] buffer;

    return str;
}

inline String PrintF(const char *format, ...)
{
    va_list args;
    int     len;
    char   *buffer;

    va_start(args, format);
    len = _vscprintf(format, args) + 1;
    buffer = new char[len * sizeof(char)];
    vsprintf(buffer, format, args);

    String str = String(buffer);
    delete[] buffer;

    return str;
}

inline bool StrICmp(const String &a, const String &b)
{
    return (_stricmp(a.c_str(), b.c_str()) == 0);
}

inline bool StartsWith(const String &str, const String &token)
{
    if (token.length() > str.length())
        return false;

    return (_strnicmp(str.c_str(), token.c_str(), token.length()) == 0);
}

inline bool EndsWith(const String &str, const String &token)
{
    if (token.length() > str.length())
        return false;

    const char *pSrc1 = str.c_str();
    const char *pSrc2 = token.c_str();

    pSrc1 += (str.length() - token.length());

    return (_stricmp(pSrc1, pSrc2) == 0);
}

inline String StrMid(const String &str, int start, int length)
{
    return str.substr(start, length);
}

inline uint HashCStr(const char *pStr)
{
    uint i;
    for (i = 0; *pStr; pStr++)
    {
        char c = *pStr;
        c = (char)(c - (c & (c >> 1) & 0x20));
        i = 131 * i + c;
    }
    return i;
}

inline uint HashStr(const String &str)
{
    return HashCStr(str.c_str());
}

inline bool WriteBinaryString(FILE *file, const String &text)
{
    size_t written = 0;
    for (size_t i = 0; i < text.size(); i++)
        written += fwrite(&text[i], sizeof(char), 1, file);

    char zero = 0;
    written += fwrite(&zero, sizeof(char), 1, file);

    return (written == text.size() + 1);
}

inline bool ReadBinaryString(FILE *file, String &text)
{
    text.clear();
    size_t read = 0, size = 0;

    char character;
    read += fread(&character, sizeof(char), 1, file);
    size++;

    while (character != 0)
    {
        text += character;
        read += fread(&character, sizeof(char), 1, file);
        size++;
    }

    return (read == size);
}

template <int Size>
class FileString
{
public:
    inline FileString();
    inline FileString(const FileString &source);
    inline FileString(const String &source);
    inline FileString(const char *pSource);

    inline operator String() const;
    inline operator bool() const;

    inline bool operator==(const char *pString) const;
    inline bool operator==(const String &string) const;
    inline bool operator==(const FileString &string) const;
    
    inline FileString &operator=(const char *pString);
    inline FileString &operator=(const String &string);
    inline FileString &operator=(const FileString &string);
    
    inline void Set(const char *pString);
    inline const char *Get() const;

private:
    char m_Buffer[Size];

};

template <int Size>
inline OutputStream &operator<<(OutputStream &os, const FileString<Size> &s)
{
    os << s.operator String();
    return os;
}

template <int Size>
inline InputStream &operator<<(InputStream &is, FileString<Size> &s)
{
    String str;
    is >> str;
    s.Set(str.c_str());
    return is;
}

template <int Size>
inline FileString<Size>::FileString()
{
    m_Buffer[0] = '\0';
}

template <int Size>
inline FileString<Size>::FileString(const FileString &source)
{
    (*this) = source;
}

template <int Size>
inline FileString<Size>::FileString(const String &source)
{
    (*this) = source;
}

template <int Size>
inline FileString<Size>::FileString(const char *pSource)
{
    (*this) = pSource;
}

template <int Size>
inline FileString<Size>::operator String() const
{
    return m_Buffer;
}

template <int Size>
inline FileString<Size>::operator bool() const
{
    return (m_Buffer[0] != '\0');
}

template <int Size>
inline bool FileString<Size>::operator==(const char *pString) const
{
    return (strcmp(m_Buffer, pString) == 0);
}

template <int Size>
inline bool FileString<Size>::operator==(const String &string) const
{
    return (strcmp(m_Buffer, string.c_str()) == 0);
}

template <int Size>
inline bool FileString<Size>::operator==(const FileString &string) const
{
    return (strcmp(m_Buffer, string.m_Buffer) == 0);
}

template <int Size>
inline FileString<Size> &FileString<Size>::operator=(const char *pString)
{
    strcpy(m_Buffer, pString);
    return *this;
}

template <int Size>
inline FileString<Size> &FileString<Size>::operator=(const String &string)
{
    strcpy(m_Buffer, string.c_str());
    return *this;
}

template <int Size>
inline FileString<Size> &FileString<Size>::operator=(const FileString &string)
{
    strcpy(m_Buffer, string.m_Buffer);
    return *this;
}

template <int Size>
inline void FileString<Size>::Set(const char *pString)
{
    strcmp(m_Buffer, pString);
}

template <int Size>
inline const char *FileString<Size>::Get() const
{
    return m_Buffer;
}

#endif // __STD_STR_H_
