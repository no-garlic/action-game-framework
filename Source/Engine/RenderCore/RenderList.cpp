
#include "RenderList.h"
#include "DebugDisplay.h"

RenderList::RenderList()
{
    constructor(RenderList);
}

RenderList::~RenderList()
{
    destructor(RenderList);
}

void RenderList::AddQuad(const float4 &tCoords, const float4 &uCoords, const float3 &color, const TextureReference &texture, int layer)
{
    ASSERT(texture.GetObject(), "Texture cannot be null");

    RenderLayer &renderLayer = m_RenderData.m_RenderLayers[layer];
    renderLayer.m_Layer = layer;

    RenderTexture &renderTexture = renderLayer.m_RenderTextures[texture.GetObjectID()];
    renderTexture.m_Texture = texture.GetObject();
    
    RenderCall rc;
    rc.m_Color = color;
    rc.m_TextureCoords = tCoords;
    rc.m_UVCoords = uCoords;
    rc.m_Matrix = m_Matrix;
    renderTexture.m_RenderCalls.push_back(rc);
}

void RenderList::Flush()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (auto &renderLayerIter : m_RenderData.m_RenderLayers)
    {
        for (auto &renderTextureIter : renderLayerIter.second.m_RenderTextures)
        {
            renderTextureIter.second.m_Texture->Apply();

            for (RenderCall &rc : renderTextureIter.second.m_RenderCalls)
            {
                glLoadMatrixf(&(rc.m_Matrix[0][0]));
                
                glBegin(GL_QUADS);
                    glTexCoord2f(rc.m_UVCoords[0], rc.m_UVCoords[3]); glColor3fv(&(rc.m_Color[0])); glVertex3f(rc.m_TextureCoords[0], rc.m_TextureCoords[1], 0.0f);
                    glTexCoord2f(rc.m_UVCoords[2], rc.m_UVCoords[3]); glColor3fv(&(rc.m_Color[0])); glVertex3f(rc.m_TextureCoords[2], rc.m_TextureCoords[1], 0.0f);
                    glTexCoord2f(rc.m_UVCoords[2], rc.m_UVCoords[1]); glColor3fv(&(rc.m_Color[0])); glVertex3f(rc.m_TextureCoords[2], rc.m_TextureCoords[3], 0.0f);        
                    glTexCoord2f(rc.m_UVCoords[0], rc.m_UVCoords[1]); glColor3fv(&(rc.m_Color[0])); glVertex3f(rc.m_TextureCoords[0], rc.m_TextureCoords[3], 0.0f);
                glEnd();

                //DebugDisplay.AddBox(vec2(rc.m_TextureCoords[0], rc.m_TextureCoords[1]), vec2(rc.m_TextureCoords[2], rc.m_TextureCoords[3]), rc.m_Matrix, Color::Magenta);
            }
        }
    }

    m_RenderData.m_RenderLayers.clear();
}
