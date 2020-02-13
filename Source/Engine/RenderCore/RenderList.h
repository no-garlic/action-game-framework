
#ifndef __RC_RENDER_LIST_H_
#define __RC_RENDER_LIST_H_

#include "libStandard.h"
#include "Texture.h"

class RenderList
{
public:
    RenderList();
    ~RenderList();

    inline void PushMatrix(const mat4 &matrix) { m_Matrix = matrix; }
    inline void PopMatrix() {}

    void AddQuad(const float4 &tCoords, const float4 &uCoords, const float3 &color, const TextureReference &texture, int layer);

    void Flush();
    
private:
    struct RenderCall
    {        
        mat4   m_Matrix;
        float4 m_TextureCoords;
        float4 m_UVCoords;
        float3 m_Color;
    };

    struct RenderTexture
    {
        Texture *m_Texture;
        Array<RenderCall> m_RenderCalls;
    };

    struct RenderLayer
    {
        int m_Layer;
        Map<ObjectID, RenderTexture> m_RenderTextures;
    };

    struct RenderData
    {
        Map<int, RenderLayer> m_RenderLayers;
    };

    mat4 m_Matrix;
    RenderData m_RenderData;

};

#endif // __RC_RENDER_LIST_H_
