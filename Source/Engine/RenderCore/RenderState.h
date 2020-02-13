
#ifndef __RC_RENDER_STATE_H_
#define __RC_RENDER_STATE_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"

class RenderState : public ManagedObject
{
    declare_typeinfo(ManagedObject);

public:
    RenderState();
    virtual ~RenderState();
    
    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Apply();

    crc32 GetCRC() const;

private:
    bool m_AlphaBlend;
    Map<uint, uint> m_States;
    mutable crc32 m_CRC;

};

#endif // __RC_RENDER_STATE_H_
