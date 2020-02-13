
#ifndef __RC_DEBUG_DISPLAY_H_
#define __RC_DEBUG_DISPLAY_H_

#include "libStandard.h"
#include "glFont.h"

class _DebugDisplay
{
public:
    _DebugDisplay();
    virtual ~_DebugDisplay();

    void Init();
    void Shutdown();

    void Draw();

    void AddLine(const vec2 &a, const vec2 &b, const vec3 &color);
    void AddLineStrip(const vec2 *points, uint numPoints, const vec3 &color);

    void AddBox(const vec2 &bottomLeft, const vec2 &topRight, const vec3 &color);
    void AddBox(const vec2 &bottomLeft, const vec2 &topRight, const mat4 &matrix, const vec3 &color);

    void AddText(const vec2 &position, const String &text, const vec3 &color);

private:
    GLfont m_Font;

    struct DrawCall
    {
        uint Type;
        uint Start;
        uint Count;
    };

    Queue<DrawCall>  m_DrawCalls;
    Array<vec2> m_VertexArray;
    Array<vec3> m_ColorArray;

    struct Text
    {
        String m_Text;
        vec2   m_Position;
        vec3   m_Color;
    };

    Array<Text> m_TextArray;

};

extern _DebugDisplay DebugDisplay;

#endif // __RC_DEBUG_DISPLAY_H_
