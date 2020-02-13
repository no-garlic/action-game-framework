
#ifndef __CFG_PROPERTY_SECTION_H_
#define __CFG_PROPERTY_SECTION_H_

#include "libStandard.h"
#include "PropertyValue.h"

class PropertySection;

typedef Map <String, PropertyValue>     PropertyValueMap;
typedef std::pair<String, PropertyValue>     PropertyValuePair;
typedef Map <String, PropertySection *> PropertySectionMap;
typedef std::pair<String, PropertySection *> PropertySectionPair;

class PropertySection
{
public:
    explicit PropertySection(PropertySection *parent);
    PropertySection(PropertySection *parent, const String &name, const String &type);
    ~PropertySection();

    bool GetErrorState() const { return m_ErrorState; }
    void ClearErrorState() const { m_ErrorState = false; }

    PropertySection *GetParentSection() { return m_ParentSection; }
    const PropertySection *GetParentSection() const { return m_ParentSection; }

    const String &GetName() const { return m_Name; }
    const String &GetType() const { return m_Type; }

    void SetName(const String &name) { m_Name = ToLower(name); }
    void SetType(const String &type) { m_Type = ToLower(type); }

    PropertyValueMap &GetValues() { return m_Values; }
    const PropertyValueMap &GetValues() const { return m_Values; }

    template<typename T>
    inline void SetValue(const String &key, const T &value);

    inline PropertyValue operator()(const String &key) const;
    inline PropertyValue GetValue(const String &key) const;
    inline bool GetValue(const String &key, PropertyValue &outValue) const;
    inline bool HasValue(const String &key) const;

    void RemoveValue(const String &key);

    PropertySection *AddChildSection(const String &name, const String &type);
    void RemoveChildSection(const PropertySection *section);
    void RemoveChildSection(const String &name);

    PropertySection *GetChildSection(const String &name);
    const PropertySection *GetChildSection(const String &name) const;

    PropertySection *GetChildSectionByType(const String &objectType, const PropertySection *searchFrom = nullptr);
    const PropertySection *GetChildSectionByType(const String &objectType, const PropertySection *searchFrom = nullptr) const;

    template <typename T>
    PropertySection *GetChildSectionByType(const PropertySection *searchFrom = nullptr)
    {
        String objectType = T::s_TypeInfo.m_pName;
        return GetChildSectionByType(objectType, searchFrom);
    }

    template <typename T>
    const PropertySection *GetChildSectionByType(const PropertySection *searchFrom = nullptr) const
    {
        String objectType = T::s_TypeInfo.m_pName;
        return GetChildSectionByType(objectType, searchFrom);
    }

    PropertySectionMap &GetChildSections() { return m_ChildSections; }
    const PropertySectionMap &GetChildSections() const { return m_ChildSections; }

    void Merge(PropertySection *other);

private:
    PropertySection(const PropertySection &other);
    PropertySection &operator=(const PropertySection &other);

    void SetErrorState() const;

    mutable bool m_ErrorState;

    String m_Name;
    String m_Type;

    PropertySection *m_ParentSection;

    PropertyValueMap m_Values;
    PropertySectionMap m_ChildSections;

    static Random s_RandomGenerator;

};

template<typename T>
void PropertySection::SetValue(const String &key, const T &value)
{
    String lKey = ToLower(key);
    m_Values[lKey] = ToString(value);
}

inline PropertyValue PropertySection::GetValue(const String &key) const
{
    String lKey = ToLower(key);

    PropertyValueMap::ConstIterator iter = m_Values.find(lKey);
    if (iter == m_Values.end())
    {
        SetErrorState();
        PropertyValue out;
        return out;
    }

    return iter->second;
}

inline bool PropertySection::HasValue(const String &key) const
{
    String lKey = ToLower(key);

    PropertyValueMap::ConstIterator iter = m_Values.find(lKey);
    if (iter == m_Values.end())
    {
        return false;
    }

    return true;
}

inline bool PropertySection::GetValue(const String &key, PropertyValue &outValue) const
{
    String lKey = ToLower(key);

    PropertyValueMap::ConstIterator iter = m_Values.find(lKey);
    if (iter == m_Values.end())
    {
        return false;
    }

    outValue = iter->second;
    return true;
}

inline PropertyValue PropertySection::operator()(const String &key) const
{
    return GetValue(key);
}

#endif // __CFG_PROPERTY_SECTION_H_
