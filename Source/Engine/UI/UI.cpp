
#include "UI.h"

_UI UI;

void _UI::BeginFrame()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void _UI::EndFrame()
{
    ImGui::Render();
}

bool _UI::BeginWindow(const String &title, int flags)
{   
    bool show = true;
    bool *pshow = &show;

    if ((flags & ImGuiWindowFlags_NoCloseButton) != 0)
    {
        flags = (flags & ~ImGuiWindowFlags_NoCloseButton);
        pshow = nullptr;
    }

    ImGui::Begin(title.c_str(), pshow, flags);
    return show;
}

void _UI::EndWindow()
{
    ImGui::End();
}

void _UI::Text(const String &text)
{
    ImGui::Text(text.c_str());
}

