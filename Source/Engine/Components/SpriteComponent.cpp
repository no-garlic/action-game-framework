
#include "SpriteComponent.h"

implement_typeinfo(SpriteComponent);

SpriteComponent::SpriteComponent()
 : Parent()
{
    constructor(SpriteComponent);
}

SpriteComponent::~SpriteComponent()
{
    destructor(SpriteComponent);
}
    
void SpriteComponent::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_Sprite = GetOrCreateObjectFromContentDatabase<Sprite>(serializer, "Sprite");
}

void SpriteComponent::Initialize()
{
    Parent::Initialize();
}

void SpriteComponent::Destroy()
{
    m_Sprite.Release();

    Parent::Destroy();
}

void SpriteComponent::Update()
{
}

int SpriteComponent::GetLayer() const
{
    if (Sprite *sprite = m_Sprite.GetObject())
    {
        return sprite->GetLayer();
    }
    return ID_InvalidLayer;
}

void SpriteComponent::AddToRenderList(RenderList &renderList) const
{
    if (Sprite *sprite = m_Sprite.GetObject())
    {
        sprite->AddToRenderList(renderList);
    }
}
