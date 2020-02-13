
#ifndef __RC_TEXTURE_SPRITE_H_
#define __RC_TEXTURE_SPRITE_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"
#include "Visual.h"
#include "Texture.h"

class Sprite : public Visual
{
    declare_typeinfo(Visual);

public:
    Sprite();
    virtual ~Sprite();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    const int2 &GetPivot() const { return m_Pivot; }
    const int2 &GetUV1() const { return m_UV1; }
    const int2 &GetUV2() const { return m_UV2; }
    const int2 &GetTextureSize() const { return m_TextureSize; }
    const int2 &GetSpriteSize() const { return m_SpriteSize; }
    const TextureReference &GetTexture() const { return m_Texture; }

    virtual int GetLayer() const { return m_Layer; }
    virtual void Draw(const mat4 &matrix) const;
    virtual void AddToRenderList(RenderList &renderList) const;

private:
    int2 m_Pivot;
    int2 m_UV1;
    int2 m_UV2;
    int2 m_TextureSize;
    int2 m_SpriteSize;
    int  m_Layer;

    TextureReference m_Texture;

};

#endif // __RC_TEXTURE_SPRITE_H_

