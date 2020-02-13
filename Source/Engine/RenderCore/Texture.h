
#ifndef __RC_TEXTURE_H_
#define __RC_TEXTURE_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"
#include "RenderState.h"

class Texture;
typedef Reference<Texture> TextureReference;

class Texture : public ManagedObject
{
    declare_typeinfo(ManagedObject);
    
public:
    Texture();
    virtual ~Texture();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    const int2 &GetSize() const { return m_Size; }
    const String &GetFilename() const { return m_Filename; }
    const Reference<RenderState> &GetRenderState() const { return m_RenderState; }

    void Apply();

private:
    uint   m_OpenGLTexture;
    int2   m_Size;
    String m_Filename;

    Reference<RenderState> m_RenderState;

};

#endif // __RC_TEXTURE_H_

