
#include "Console.h"

namespace Console
{
    HANDLE                      hStdout;
    HANDLE                      hStdin;
    CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
    WORD                        wOldColorAttr;
    DWORD                       fdwOldMode;
    WORD                        wOldColorAttrs;
    WORD                        wColorAttr;

    bool Init()
    {
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        hStdin = GetStdHandle(STD_INPUT_HANDLE);

        if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE)
            return false;

        if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
            return false;

        wOldColorAttr = csbiInfo.wAttributes;
        return true;
    }
    
    bool Close()
    {
        if (!SetConsoleMode(hStdin, fdwOldMode))
            return false;

        if (!SetConsoleTextAttribute(hStdout, wOldColorAttrs))
            return false;

        return true;
    }
    
    void SetAttributes(unsigned short attrib)
    {
        wColorAttr = attrib;
        SetConsoleTextAttribute(hStdout, (WORD)attrib);
    }

    void Print(const char *str, ...)
    {
        va_list args;
        int     len;
        char   *buffer;
        DWORD   written;

        va_start(args, str);
        len = _vscprintf(str, args) + 1;
        buffer = new char[len * sizeof(char)];
        vsprintf(buffer, str, args);

        WriteConsole(hStdout, buffer, (WORD)strlen(buffer), &written, 0);
        delete[] buffer;
    }

    void Clear()
    {
        CONSOLE_SCREEN_BUFFER_INFO info;

        GetConsoleScreenBufferInfo(hStdout, &info);

        COORD coord;
        coord.X = 0;
        coord.Y = 0;
        CHAR chFillChar = ' ';
        DWORD cWritten;
        DWORD dwSize = info.dwSize.X * info.dwSize.Y;

        FillConsoleOutputCharacter(
            hStdout,          // screen buffer handle 
            chFillChar,       // fill with spaces 
            dwSize,           // number of cells to fill 
            coord,            // first cell to write to 
            &cWritten);       // actual number written 

        FillConsoleOutputAttribute(hStdout, wColorAttr, dwSize, coord, &cWritten);
        SetConsoleCursorPosition(hStdout, coord);
    }
    
    String GetText()
    {
        char buffer[256];
        DWORD read;
        ReadConsole(hStdin, buffer, 255, &read, 0);
        buffer[read - 2] = '\0';
        return buffer;
    }

};
