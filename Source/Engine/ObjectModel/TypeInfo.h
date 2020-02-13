
#ifndef __OM_TYPEINFO_H_
#define __OM_TYPEINFO_H_

#include "libStandard.h"

#define declare_typeinfo(PARENTCLASS) \
    public: \
    static TypeInfo s_TypeInfo; \
    typedef PARENTCLASS Parent; \
    virtual const TypeInfo *GetTypeInfo() const { return &s_TypeInfo; }    

#define implement_typeinfo(CLASS) \
    TypeInfo CLASS::s_TypeInfo(&CLASS::Parent::s_TypeInfo, (#CLASS));

class TypeInfo
{
public:
    inline TypeInfo(const TypeInfo *pParentTypeInfo, const char *pClassName)
    : m_pName(pClassName)
    {
        m_pParentTypeInfo = pParentTypeInfo;
    }

    const TypeInfo     *m_pParentTypeInfo;
    const char *const   m_pName;
};

template<class TO, class FROM>
inline TO *dyn_cast(FROM * pFrom)
{
    if (pFrom)
    {
        const TypeInfo *pTypeInfo = pFrom->GetTypeInfo();
        for (;;)
        {
            if (pTypeInfo == &TO::s_TypeInfo)
            {
                return static_cast<TO *>(pFrom);
            }
            if (pTypeInfo->m_pParentTypeInfo == pTypeInfo)
                break;

            pTypeInfo = pTypeInfo->m_pParentTypeInfo;
        }
    }
    return NULL;
}

#endif // __OM_TYPEINFO_H_
