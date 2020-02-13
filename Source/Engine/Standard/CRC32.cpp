
#include "CRC32.h"

bool CRC32::m_TableBuilt = false;
uint CRC32::m_Table[0x100];

void CRC32::BuildTable()
{
    for (int i = 0; i < 0x100; ++i)
    {
        m_Table[i] = Reflect(static_cast<uint>(i), 8) << 24;

        for (int j = 0; j < 8; ++j)
        {
            m_Table[i] = (m_Table[i] << 1) ^ ((m_Table[i] & (1 << 31)) ? POLYNOMIAL : 0);
        }

        m_Table[i] = Reflect(m_Table[i], 32);
    }

    m_TableBuilt = true;
}

uint CRC32::Reflect(uint v, int bits)
{
    uint ret = 0;

    --bits;
    for (int i = 0; i <= bits; ++i)
    {
        if (v & (1<<i))
        {
            ret |= 1 << (bits - i);
        }
    }
    return ret;
}

void CRC32::Hash(const void *buffer, uint size)
{
    const uchar *p = reinterpret_cast<const uchar *>(buffer);

    if(!m_TableBuilt)
    {
        BuildTable();
    }

    for (uint i = 0; i < size; ++i)
    {
        m_Crc = (m_Crc >> 8) ^ m_Table[ (m_Crc & 0xFF) ^ p[i] ];
    }
}

crc32 GetCRC(const String &string)
{
    if (string.length() == 0)
        return 0;

    const void *buffer = string.c_str();
    uint len = string.length();
    crc32 crc32 = CRC32(buffer, len);
    
    return crc32;
}
