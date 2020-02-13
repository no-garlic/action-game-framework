
#include "Sprite.h"
#include "ApplicationCentral.h"
#include "DebugDisplay.h"
#include "Layer.h"

implement_typeinfo(Sprite);

Sprite::Sprite()
:   Parent(),
    m_Layer(ID_InvalidLayer)
{
    constructor(Sprite);
}

Sprite::~Sprite()
{
    destructor(Sprite);
}

void Sprite::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_Pivot << serializer("Pivot");
    m_UV1   << serializer("UV1");
    m_UV2   << serializer("UV2");

    Reference<Layer> layerRef = GetOrCreateObjectFromContentDatabase<Layer>(serializer, "Layer");
    if (Layer *layer = layerRef.GetObject())
    {
        m_Layer = layer->GetDepth();
    }

    m_Texture = GetOrCreateObjectFromContentDatabase<Texture>(serializer, "Texture");
}

void Sprite::Initialize()
{
    Parent::Initialize();

    m_SpriteSize.x = fabs(m_UV2.x - m_UV1.x);
    m_SpriteSize.y = fabs(m_UV2.y - m_UV1.y);

    Texture *texture = m_Texture.GetObject();
    ASSERT(texture, "Texture is NULL");

    m_TextureSize = texture->GetSize();

    if (m_SpriteSize.x == 0)
    {
        m_SpriteSize.x = m_TextureSize.x;
        m_UV1.x = 0;
        m_UV2.x = m_SpriteSize.x;
    }

    if (m_SpriteSize.y == 0)
    {
        m_SpriteSize.y = m_TextureSize.y;
        m_UV1.y = 0;
        m_UV2.y = m_TextureSize.y;
    }
}

void Sprite::Destroy()
{
    m_Texture.Release();

    Parent::Destroy();
}

void Sprite::Draw(const mat4 &matrix) const
{
    if (Texture *texture = m_Texture.GetObject())
    {
        texture->Apply();

        float u1 = m_UV1.x / (float) m_TextureSize.x;
        float u2 = m_UV2.x / (float) m_TextureSize.x;
        float v1 = m_UV1.y / (float) m_TextureSize.y;
        float v2 = m_UV2.y / (float) m_TextureSize.y;
        
        float left   = 0.0f - m_Pivot.x;
        float right  = m_SpriteSize.x - m_Pivot.x;
        float bottom = 0.0f - m_Pivot.y;
        float top    = m_SpriteSize.y - m_Pivot.y;
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixf(&(matrix[0][0]));

        glBegin(GL_QUADS);
            glTexCoord2f(u1, v2); glColor3f(1.f, 1.f, 1.f); glVertex3f(left,  bottom, 0.0f);
            glTexCoord2f(u2, v2); glColor3f(1.f, 1.f, 1.f); glVertex3f(right, bottom, 0.0f);
            glTexCoord2f(u2, v1); glColor3f(1.f, 1.f, 1.f); glVertex3f(right, top,    0.0f);        
            glTexCoord2f(u1, v1); glColor3f(1.f, 1.f, 1.f); glVertex3f(left,  top,    0.0f);
        glEnd();

        DebugDisplay.AddBox(vec2(left, bottom), vec2(right, top), matrix, Color::Magenta);
    }
}

void Sprite::AddToRenderList(RenderList &renderList) const
{
    float u1 = m_UV1.x / (float) m_TextureSize.x;
    float u2 = m_UV2.x / (float) m_TextureSize.x;
    float v1 = m_UV1.y / (float) m_TextureSize.y;
    float v2 = m_UV2.y / (float) m_TextureSize.y;
        
    float left   = 0.0f - m_Pivot.x;
    float right  = m_SpriteSize.x - m_Pivot.x;
    float bottom = 0.0f - m_Pivot.y;
    float top    = m_SpriteSize.y - m_Pivot.y;

    float4 tCoords = float4(left, bottom, right, top);
    float4 uCoords = float4(u1, v1, u2, v2);
    float3 color   = Color::White;

    renderList.AddQuad(tCoords, uCoords, color, m_Texture, m_Layer);
}

