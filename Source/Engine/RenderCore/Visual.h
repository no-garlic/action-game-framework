#ifndef __RC_VISUAL_H_
#define __RC_VISUAL_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"
#include "RenderList.h"

class Visual : public ManagedObject
{
    declare_typeinfo(ManagedObject);

public:
    Visual();
    virtual ~Visual();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual int GetLayer() const = 0;
    virtual void Draw(const mat4 &matrix) const = 0;
    virtual void AddToRenderList(RenderList &renderList) const = 0;

};

#endif // __RC_VISUAL_H_
