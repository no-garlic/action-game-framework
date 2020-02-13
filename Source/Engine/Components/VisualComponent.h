
#ifndef __CPT_VISUAL_COMPONENT_H_
#define __CPT_VISUAL_COMPONENT_H_

#include "libStandard.h"
#include "libObjectModel.h"
#include "RenderList.h"

class VisualComponent : public Component
{
public:
    declare_typeinfo(Component);

public:
    VisualComponent();
    virtual ~VisualComponent();
    
    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Update();

    virtual int GetLayer() const = 0;
    virtual void AddToRenderList(RenderList &renderList) const = 0;
    
};

#endif // __CPT_VISUAL_COMPONENT_H_

