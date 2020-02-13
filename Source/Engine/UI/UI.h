
#ifndef __UI_UI_H_
#define __UI_UI_H_

#include "libStandard.h"

#define ImGuiWindowFlags_NoCloseButton (1 << 29)

struct _UI
{
    void BeginFrame();
    void EndFrame();

    bool BeginWindow(const String &title, int flags = 0);
    void EndWindow();

    void Text(const String &text);
};

extern _UI UI;

#endif // __UI_UI_H_
