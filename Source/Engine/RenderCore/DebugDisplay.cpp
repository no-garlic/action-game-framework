
#include "DebugDisplay.h"
#include "Camera.h"
#include "ApplicationCentral.h"
#include "libConfig.h"

_DebugDisplay DebugDisplay;

_DebugDisplay::_DebugDisplay()
{
    m_Font = GL_NONE;
}

_DebugDisplay::~_DebugDisplay()
{
}

void _DebugDisplay::Init()
{
    String fontName  = "comic";
    int    fontSize  = 32;
    uint   atlasSize = 256;

    if (const PropertySection *properties = Application.GetPropertySection("DebugDisplay"))
    {
        Serializer serializer(properties);

        fontName  << serializer("FontName");
        fontSize  << serializer("FontSize");
        atlasSize << serializer("AtlasSize");
    }

    m_Font = glGenFont(fontName.c_str(), fontSize, atlasSize);
}

void _DebugDisplay::Shutdown()
{
    glDeleteFont(m_Font);
    m_Font = GL_NONE;
}

void _DebugDisplay::Draw()
{
    if (!m_DrawCalls.empty())
    {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        float *vertexData = &(m_VertexArray.data()->x);
        float *colorData = &(m_ColorArray.data()->x);

        glVertexPointer(2, GL_FLOAT, 0, vertexData);
        glColorPointer(3, GL_FLOAT, 0, colorData);

        while (!m_DrawCalls.empty())
        {
            DrawCall &drawCall = m_DrawCalls.front();

            glDrawArrays(drawCall.Type, drawCall.Start, drawCall.Count);
    
            m_DrawCalls.pop();
        }

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        m_VertexArray.clear();
        m_ColorArray.clear();
    }

    for (auto iter : m_TextArray)
    {
        glFontColor3(iter.m_Color);
        glFontDrawText(m_Font, iter.m_Text.c_str(), iter.m_Position[0], iter.m_Position[1]);
    }
}

void _DebugDisplay::AddLine(const vec2 &a, const vec2 &b, const vec3 &color)
{   
    DrawCall drawCall;
    drawCall.Type = GL_LINES;
    drawCall.Start = m_VertexArray.size();
    drawCall.Count = 2;
    m_DrawCalls.push(drawCall);

    m_VertexArray.push_back(a);
    m_VertexArray.push_back(b);

    m_ColorArray.push_back(color);
    m_ColorArray.push_back(color);    
}

void _DebugDisplay::AddLineStrip(const vec2 *points, uint numPoints, const vec3 &color)
{
    DrawCall drawCall;
    drawCall.Type = GL_LINE_STRIP;
    drawCall.Start = m_VertexArray.size();
    drawCall.Count = numPoints;
    m_DrawCalls.push(drawCall);

    for (uint i = 0; i < numPoints; ++i)
    {
        m_VertexArray.push_back(*points++);
        m_ColorArray.push_back(color);
    }
}

void _DebugDisplay::AddBox(const vec2 &bottomLeft, const vec2 &topRight, const vec3 &color)
{
    float w = topRight.x - bottomLeft.x;
    float h = topRight.y - bottomLeft.y;

    if (w <= 0.0f || h <= 0.0f)
        return;

    vec2 lines[] = 
    {
        bottomLeft,
        bottomLeft + vec2(0, h),
        topRight,
        bottomLeft + vec2(w, 0),
    };

    vec2 *points = lines;

    DrawCall drawCall;
    drawCall.Type = GL_LINE_LOOP;
    drawCall.Start = m_VertexArray.size();
    drawCall.Count = 4;
    m_DrawCalls.push(drawCall);

    for (uint i = 0; i < 4; ++i)
    {
        m_VertexArray.push_back(*points++);
        m_ColorArray.push_back(color);
    }
}

void _DebugDisplay::AddBox(const vec2 &bottomLeft, const vec2 &topRight, const mat4 &matrix, const vec3 &color)
{
    vec4 bl = matrix * vec4(bottomLeft.x, bottomLeft.y, 0, 1);
    vec4 br = matrix * vec4(topRight.x, bottomLeft.y, 0, 1);
    vec4 tl = matrix * vec4(bottomLeft.x, topRight.y, 0, 1);
    vec4 tr = matrix * vec4(topRight.x, topRight.y, 0, 1);

    float left = min(min(min(bl.x, tr.x), br.x), tl.x);
    float right = max(max(max(bl.x, tr.x), br.x), tl.x);
    float bottom = min(min(min(bl.y, tr.y), br.y), tl.y);
    float top = max(max(max(bl.y, tr.y), br.y), tl.y);

    bl = vec4(left, bottom, 0, 1);
    tr = vec4(right, top, 0, 1);

    AddBox(bl, tr, color);
}

void _DebugDisplay::AddText(const vec2 &position, const String &text, const vec3 &color)
{
    Text t;
    t.m_Position = position;
    t.m_Text = text;
    t.m_Color = color;

    m_TextArray.push_back(t);
}

