
#include "PropertySection.h"

Random PropertySection::s_RandomGenerator;

PropertySection::PropertySection(PropertySection *parent)
{
    constructor(PropertySection);

    m_ErrorState = false;

    m_ParentSection = parent;
}

PropertySection::PropertySection(PropertySection *parent, const String &name, const String &type)
{
    constructor(PropertySection);

    m_ErrorState = false;

    m_ParentSection = parent;
    
    m_Name = ToLower(name);
    m_Type = ToLower(type);
    
    if (m_Name.length() == 0)
    {
        m_Name = PrintF("0x%08x", s_RandomGenerator.GetInt());
    }
}

PropertySection::~PropertySection()
{
    destructor(PropertySection);

    for (auto iter = m_ChildSections.begin(); iter != m_ChildSections.end(); ++iter)
    {
        delete iter->second;
    }
    m_ChildSections.clear();
}

void PropertySection::SetErrorState() const
{
    m_ErrorState = true;
}

void PropertySection::RemoveValue(const String &key)
{
    String lKey = ToLower(key);

    PropertyValueMap::Iterator iter = m_Values.find(lKey);

    if (iter != m_Values.end())
    {
        m_Values.erase(iter);
    }
}

PropertySection *PropertySection::AddChildSection(const String &name, const String &type)
{
    String lName = ToLower(name);
    String lType = ToLower(type);

    if (lName.length() == 0)
    {
        lName = PrintF("0x%08x", s_RandomGenerator.GetInt());
    }    

    PropertySectionMap::Iterator iter = m_ChildSections.find(lName);

    if (iter != m_ChildSections.end())
    {
        SetErrorState();
        return nullptr;
    }

    PropertySection *section = new PropertySection(this, lName, lType);
    m_ChildSections.insert(MakePair(lName, section));
    return section;
}

void PropertySection::RemoveChildSection(const PropertySection *section)
{
    for (auto iter = m_ChildSections.begin(); iter != m_ChildSections.end(); ++iter)
    {
        if (iter->second == section)
        {
            SafeDelete(iter->second);
            m_ChildSections.erase(iter);
            return;
        }
    }
}

void PropertySection::RemoveChildSection(const String &name)
{
    String lName = ToLower(name);

    PropertySectionMap::Iterator iter = m_ChildSections.find(lName);
    if (iter != m_ChildSections.end())
    {
        SafeDelete(iter->second);
        m_ChildSections.erase(iter);
        return;
    }
}

PropertySection *PropertySection::GetChildSection(const String &name)
{
    String lName = ToLower(name);

    PropertySectionMap::Iterator iter = m_ChildSections.find(lName);
    if (iter != m_ChildSections.end())
    {
        return iter->second;
    }

    return nullptr;
}

const PropertySection *PropertySection::GetChildSection(const String &name) const
{
    String lName = ToLower(name);

    PropertySectionMap::ConstIterator iter = m_ChildSections.find(lName);
    if (iter != m_ChildSections.end())
    {
        return iter->second;
    }

    return nullptr;
}

PropertySection *PropertySection::GetChildSectionByType(const String &objectType, const PropertySection *searchFrom)
{
    String lType = ToLower(objectType);

    bool found = (searchFrom == nullptr);

    for (auto iter = m_ChildSections.begin(); iter != m_ChildSections.end(); ++iter)
    {
        if (iter->second->GetType() == lType)
        {
            if (found)
            {
                return iter->second;
            }
            else
            {
                if (searchFrom == iter->second)
                {
                    found = true;
                }
            }
        }
    }

    return nullptr;
}

const PropertySection *PropertySection::GetChildSectionByType(const String &objectType, const PropertySection *searchFrom) const
{
    String lType = ToLower(objectType);

    bool found = (searchFrom == nullptr);

    for (auto iter = m_ChildSections.begin(); iter != m_ChildSections.end(); ++iter)
    {
        if (iter->second->GetType() == lType)
        {
            if (found)
            {
                return iter->second;
            }
            else
            {
                if (searchFrom == iter->second)
                {
                    found = true;
                }
            }
        }
    }

    return nullptr;
}

void PropertySection::Merge(PropertySection *other)
{
    PropertyValueMap &otherValues = other->GetValues();
    PropertySectionMap &otherSections = other->GetChildSections();

    for (auto iter = otherValues.begin(); iter != otherValues.end(); ++iter)
    {
        SetValue(iter->first, iter->second.Value);
    }
    
    for (auto iter = otherSections.begin(); iter != otherSections.end(); ++iter)
    {
        PropertySection *otherSection = iter->second;
        
        PropertySection *thisSection = GetChildSection(otherSection->GetName());
        if (thisSection)
        {
            thisSection->Merge(otherSection);
        }
        else
        {
            PropertySection *newSection = AddChildSection(otherSection->GetType(), otherSection->GetName());
            newSection->Merge(otherSection);
        }
    }
}
