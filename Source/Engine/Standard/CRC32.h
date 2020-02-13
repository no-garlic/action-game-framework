
#ifndef __STD_CRC32_H_
#define __STD_CRC32_H_

#include "Type.h"
#include "Str.h"
#include "Log.h"

#define ID_InvalidCRC32 ~0

crc32 GetCRC(const String &string);

class CRC32
{
public:
    inline CRC32() { constructor(CRC32); Reset(); }
    inline CRC32(const void *buffer, uint size) { constructor(CRC32); Reset(); Hash(buffer, size); }
    ~CRC32() { destructor(CRC32); }

    inline operator uint() const { return Get(); }

    inline crc32 Get() const { return (crc32) ~m_Crc; }

    virtual void Reset() { m_Crc = ID_InvalidCRC32; }
    virtual void Hash(const void *buffer, uint size);

private:
    uint m_Crc;
    static bool m_TableBuilt;
    static uint m_Table[0x100];

    static const uint POLYNOMIAL = 0x04C11DB7;

private:
    static void BuildTable();
    static uint Reflect(uint v, int bits);

};

#endif // __STD_CRC32_H_
