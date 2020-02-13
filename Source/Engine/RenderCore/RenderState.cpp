
#include "RenderState.h"
#include "ApplicationCentral.h"
#include "OpenGLResolver.h"

implement_typeinfo(RenderState);

RenderState::RenderState()
 :  Parent(),
    m_AlphaBlend(false),
    m_CRC(ID_InvalidCRC32)
{
    constructor(RenderState);
}

RenderState::~RenderState()
{
    destructor(RenderState);
}

void RenderState::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_AlphaBlend << serializer("AlphaBlend");

    for (auto iter = serializer.Properties->GetValues().begin(); iter != serializer.Properties->GetValues().end(); ++iter)
    {
        String param = iter->first;
        String value = iter->second;

        if (StartsWith(param, "GL_") || StartsWith(value, "GL_"))
        {
            uint openGLParam, openGLValue;
            bool resolved = (OpenGLResolver.ResolveToken(param, openGLParam) && OpenGLResolver.ResolveToken(value, openGLValue));
            if (!resolved)
            {
                LOG_WARNING << "Unable to resolve render state param: " << param << " = " << value << ENDLOG;
            }
            else
            {
                m_States[openGLParam] = openGLValue;
            }
        }
    }
}

void RenderState::Initialize()
{
    Parent::Initialize();
}

void RenderState::Destroy()
{
    Parent::Destroy();
}

void RenderState::Apply()
{
    for (auto pair : m_States)
    {
        uint state = pair.first;
        uint value = pair.second;

        glTexParameteri(GL_TEXTURE_2D, state, value);	
    }

    if (m_AlphaBlend)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

crc32 RenderState::GetCRC() const
{
    if (m_CRC == ID_InvalidCRC32)
    {
        CRC32 crc;

        for (auto pair : m_States)
        {
            uint state = pair.first;
            uint value = pair.second;

            crc.Hash(&state, sizeof(uint));
            crc.Hash(&value, sizeof(uint));
        }
        
        uint hash = m_AlphaBlend ? 0x7938a5bc : 0x38b0f716;
        crc.Hash(&hash, sizeof(uint));

        m_CRC = crc.Get();
    }

    return m_CRC;
}

