
#ifndef __CPT_AI_COMPONENT_H
#define __CPT_AI_COMPONENT_H

#include "libStandard.h"
#include "libObjectModel.h"
#include "libRenderCore.h"

class AIComponent : public Component
{
    declare_typeinfo(Component);

public:
    AIComponent();
    virtual ~AIComponent();
    
    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Update();

private:
    
};


#endif // __CPT_AI_COMPONENT_H
