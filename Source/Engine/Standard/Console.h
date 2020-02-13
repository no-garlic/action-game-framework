
#ifndef __STD_CONSOLE_H_
#define __STD_CONSOLE_H_

#include "Type.h"
#include "Str.h"

namespace Console
{
    // Foreground colors
    static const unsigned short FG_BLUE = 0x0001;
    static const unsigned short FG_GREEN = 0x0002;
    static const unsigned short FG_RED = 0x0004;
    static const unsigned short FG_WHITE = 0x0007;
    static const unsigned short FG_BLACK = 0x0000;
    static const unsigned short FG_INTENSITY = 0x0008;

    // Background colors
    static const unsigned short BG_BLUE = 0x0010;
    static const unsigned short BG_GREEN = 0x0020;
    static const unsigned short BG_RED = 0x0040;
    static const unsigned short BG_WHITE = 0x0070;
    static const unsigned short BG_BLACK = 0x0000;
    static const unsigned short BG_INTENSITY = 0x0080;

    bool Init();
    bool Close();

    void SetAttributes(unsigned short attrib);
    void Clear();
    
    void Print(const char *str, ...);
    String GetText();

};

#endif // __STD_CONSOLE_H_
