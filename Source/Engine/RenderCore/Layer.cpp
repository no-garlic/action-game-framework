
#include "Layer.h"

implement_typeinfo(Layer);

Layer::Layer()
 :   m_Depth(ID_InvalidLayer)
{
}

Layer::~Layer()
{
}
    
void Layer::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_Depth << serializer("Depth");
}

void Layer::Initialize()
{
    Parent::Initialize();
}

void Layer::Destroy()
{
    Parent::Destroy();
}
