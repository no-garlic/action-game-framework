
#include "AnimationComponent.h"

implement_typeinfo(AnimationComponent);

AnimationComponent::AnimationComponent()
 :  Parent(),
    m_AutoStart(false),
    m_AutoStarted(false)
{
    constructor(AnimationComponent);
}

AnimationComponent::~AnimationComponent()
{
    destructor(AnimationComponent);
}
    
void AnimationComponent::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_DefaultAnimationName << serializer("Default");
    m_AutoStart << serializer("AutoStart");

    const PropertySection *section = nullptr;
    while (section = serializer.Properties->GetChildSectionByType("Animations", section))
    {
        for (auto pair : section->GetValues())
        {
            const String &name = pair.first;
            const String &animationName = pair.second;
            
            Reference<Animation> animation = GetOrCreateObjectFromContentDatabase<Animation>(animationName);
            if (animation.IsValid())
            {
                String lName = ToLower(name);
                m_Animations[lName] = animation;
            }
        }
    }
}

void AnimationComponent::Initialize()
{
    Parent::Initialize();

    PlayAnimation(m_DefaultAnimationName, m_AutoStart);
}

void AnimationComponent::Destroy()
{
    for (auto pair : m_Animations)
    {
        pair.second.Release();
    }
    m_Animations.clear();
    m_CurrentAnimation.Release();

    Parent::Destroy();
}

void AnimationComponent::Update()
{
    if (m_AutoStart && !m_AutoStarted)
    {
        m_AutoStarted = true;
        if (Animation *animation = m_CurrentAnimation.GetObject())
        {
            animation->Start();
        }
    }
}

int AnimationComponent::GetLayer() const
{
    if (Animation *animation = m_CurrentAnimation.GetObject())
    {
        return animation->GetLayer();
    }
    return ID_InvalidLayer;
}

void AnimationComponent::AddToRenderList(RenderList &renderList) const
{
    if (Animation *animation = m_CurrentAnimation.GetObject())
    {
        animation->AddToRenderList(renderList);
    }
}

bool AnimationComponent::PlayAnimation(const String &name, bool autoStart)
{
    String lName = ToLower(name);
    auto pair = m_Animations.find(lName);
    if (pair == m_Animations.end())
    {
        m_CurrentAnimation.Release();
        return false;
    }
    else
    {
        m_CurrentAnimation = pair->second;
        if (autoStart)
        {
            m_AutoStarted = false;
        }
    }
    return true;
}
