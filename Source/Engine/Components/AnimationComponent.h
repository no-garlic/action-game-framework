
#ifndef __CPT_ANIMATION_COMPONENT_H
#define __CPT_ANIMATION_COMPONENT_H

#include "libStandard.h"
#include "libObjectModel.h"
#include "libRenderCore.h"
#include "libAnimation.h"
#include "VisualComponent.h"

class AnimationComponent : public VisualComponent
{
    declare_typeinfo(VisualComponent);

public:
    AnimationComponent();
    virtual ~AnimationComponent();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Update();

    virtual int GetLayer() const;
    virtual void AddToRenderList(RenderList &renderList) const;

    bool PlayAnimation(const String &name, bool autoStart = false);

private:
    bool m_AutoStart;
    bool m_AutoStarted;
    Map< String, Reference<Animation> > m_Animations;
    Reference<Animation> m_CurrentAnimation;
    String m_DefaultAnimationName;
};

#endif // __CPT_ANIMATION_COMPONENT_H
