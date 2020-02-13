
#ifndef __CPT_SPRITE_COMPONENT_H
#define __CPT_SPRITE_COMPONENT_H

#include "libStandard.h"
#include "libObjectModel.h"
#include "libRenderCore.h"
#include "VisualComponent.h"

class SpriteComponent : public VisualComponent
{
    declare_typeinfo(VisualComponent);

public:
    SpriteComponent();
    virtual ~SpriteComponent();
    
    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Update();

    virtual int GetLayer() const;
    virtual void AddToRenderList(RenderList &renderList) const;

    const Reference<Sprite> &GetSprite() const { return m_Sprite; }

private:
    Reference<Sprite> m_Sprite;

};

#endif // __CPT_SPRITE_COMPONENT_H
