
#ifndef __RC_LAYER_H_
#define __RC_LAYER_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"

#define ID_InvalidLayer ((int) INT_MIN)

class Layer : public ManagedObject
{
    declare_typeinfo(ManagedObject);

public:
    Layer();
    virtual ~Layer();
    
    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    inline int GetDepth() const { return m_Depth; }
    inline operator int() const { return m_Depth; }

    static int GetInvalidLayer() { return ID_InvalidLayer; }

private:
    int m_Depth;

};

inline int GetLayerDepth(const Layer &layer)
{
    return layer.GetDepth();
}

#endif // __RC_LAYER_H_
