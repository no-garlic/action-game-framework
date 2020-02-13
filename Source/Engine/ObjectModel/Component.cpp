
#include "Component.h"

implement_typeinfo(Component);

Component::Component()
 : m_Parent(nullptr)
{
    constructor(Component);
}

Component::~Component()
{
    destructor(Component);
}

void Component::Initialize()
{
}

void Component::Destroy()
{
}

void Component::Update()
{
}
