
#include "AIComponent.h"

implement_typeinfo(AIComponent);

AIComponent::AIComponent()
 : Parent()
{
    constructor(AIComponent);
}

AIComponent::~AIComponent()
{
    destructor(AIComponent);
}
    
void AIComponent::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);
}

void AIComponent::Initialize()
{
    Parent::Initialize();
}

void AIComponent::Destroy()
{
    Parent::Destroy();
}

void AIComponent::Update()
{
}

