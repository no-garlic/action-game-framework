
#include "Texture.h"
#include "RenderState.h"
#include "ApplicationCentral.h"

implement_typeinfo(Texture);

Texture::Texture()
 :  Parent(),
    m_OpenGLTexture(GL_NONE)
{
    constructor(Texture);    
}

Texture::~Texture()
{
    destructor(Texture);
}

void Texture::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_Size     << serializer("Size");
    m_Filename << serializer("Filename");

    m_RenderState = GetOrCreateObjectFromContentDatabase<RenderState>(serializer, "RenderState");
}

void Texture::Initialize()
{
    Parent::Initialize();

    String filename = m_Filename;

    if (FileDatabase *textureDatabase = FileSystem("Assets"))
    {
        FileHandle fh = textureDatabase->FindFileHandle(filename);
        filename = fh.GetAbsoluteFilename();
    }

    int numberOfChannels;
    unsigned char *data = stbi_load(filename.c_str(), &m_Size.x, &m_Size.y, &numberOfChannels, 0);

    if (!data)
    {
        LOG_ERROR << "Failed to load the texture source file: " << filename << ENDLOG;
        return;
    }

    glGenTextures(1, &m_OpenGLTexture);
    glBindTexture(GL_TEXTURE_2D, m_OpenGLTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Size.x, m_Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

void Texture::Destroy()
{
    if (m_OpenGLTexture != GL_NONE)
    {
        glDeleteTextures(1, &m_OpenGLTexture);
        m_OpenGLTexture = GL_NONE;
    }
    m_RenderState.Release();

    Parent::Destroy();
}

void Texture::Apply()
{
    if (m_OpenGLTexture != 0)
    {
        glBindTexture(GL_TEXTURE_2D, m_OpenGLTexture);
        glEnable(GL_TEXTURE_2D);

        if (RenderState *renderState = m_RenderState.GetObject())
        {
            renderState->Apply();
        }
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }
}

