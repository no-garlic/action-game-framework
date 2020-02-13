
#include "VisualComponent.h"

implement_typeinfo(VisualComponent);

VisualComponent::VisualComponent()
 : Parent()
{
    constructor(VisualComponent);
}

VisualComponent::~VisualComponent()
{
    destructor(VisualComponent);
}
    
void VisualComponent::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);
}

void VisualComponent::Initialize()
{
    Parent::Initialize();
}

void VisualComponent::Destroy()
{
    Parent::Destroy();
}

void VisualComponent::Update()
{
}
