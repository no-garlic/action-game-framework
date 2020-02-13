
#include "Visual.h"

implement_typeinfo(Visual);

Visual::Visual()
:   Parent()
{
    constructor(Visual);
}

Visual::~Visual()
{
    destructor(Visual);
}

void Visual::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);
}

void Visual::Initialize()
{
    Parent::Initialize();
}

void Visual::Destroy()
{
    Parent::Destroy();
}
