
#include "PropertyFile.h"

uint PropertyFile::s_TabOffset = 0;

#define READ_LINE(f,x)                                          \
    std::getline(f, x);                                         \
    TrimString(line);                                           \
    while (x.empty() ||                                         \
           x.at(0) == '#' ||                                    \
          (x.length() > 1 && x.at(0) == '/' && x.at(1) == '/')  \
    )                                                           \
    {                                                           \
        std::getline(f, x);                                     \
        TrimString(line);                                       \
        if (f.eof() || f.bad() || f.fail())                     \
            return true;                                        \
    }                                                           \
    //LOG_INFO << line << ENDLOG;                                 

PropertyFile::PropertyFile()
{
    constructor(PropertyFile);

    m_Loaded = false;
}

PropertyFile::~PropertyFile()
{
    destructor(PropertyFile);
    
    for (auto iter = m_Sections.begin(); iter != m_Sections.end(); ++iter)
    {
        delete iter->second;
    }
    m_Sections.clear();
}

bool PropertyFile::Load(const String &filename)
{
    if (filename.empty())
        return false;

    if (m_Loaded)
        return false;

    s_TabOffset = 0;
    m_Filename = filename;
    m_Sections.clear();

    std::fstream file;
    file.open(filename.c_str(), std::ios_base::in);
    if (!file.is_open())
        return false;

    while (!file.eof())
    {
        String line;
        READ_LINE(file, line);

        bool oneLineSection = false;
        String name, type;
        if (!ReadSectionData(line, name, type, oneLineSection))
            return false;

        if (oneLineSection)
        {
            PropertySection *section = AddSection(type, name);
        }
        else
        {
            READ_LINE(file, line);
            if (!IsSectionStart(line))
                return false;

            PropertySection *section = AddSection(type, name);
            if (!LoadSection(section, file))
                return false;
        }

        if (file.fail() || file.bad())
            return false;
    }

    m_Loaded = true;

    file.close();
    return true;
}

bool PropertyFile::Merge(const String &filename)
{
    PropertyFile file;
    if (!file.Load(filename))
    {
        return false;
    }
    
    PropertySectionMap &propertySections = file.GetSections();
    for (auto iter = propertySections.begin(); iter != propertySections.end(); ++iter)
    {
        PropertySection *otherSection = iter->second;

        PropertySection *thisSection = GetSection(otherSection->GetName());
        if (thisSection)
        {
            thisSection->Merge(otherSection);
        }
        else
        {
            PropertySection *newSection = AddSection(otherSection->GetType(), otherSection->GetName());
            newSection->Merge(otherSection);
        }
    }

    return true;
}

bool PropertyFile::SaveAs(const String &filename)
{
    if (filename.empty())
        return false;

    m_Filename = filename;
    s_TabOffset = 0;

    std::fstream file;
    file.open(filename.c_str(), std::ios_base::out);
    if (!file.is_open())
        return false;

    file << (*this);

    file << std::endl;
    file.close();
    return true;
}

const PropertySection *PropertyFile::GetSection(const String &name) const
{
    String lName = ToLower(name);
    PropertySectionMap::ConstIterator iter = m_Sections.find(lName);
    if (iter == m_Sections.end())
    {
        return nullptr;
    }

    return iter->second;
}

PropertySection *PropertyFile::GetSection(const String &name)
{
    String lName = ToLower(name);
    PropertySectionMap::ConstIterator iter = m_Sections.find(lName);
    if (iter == m_Sections.end())
    {
        return nullptr;
    }

    return iter->second;
}

PropertySection *PropertyFile::AddSection(const String &type, const String &name)
{
    PropertySectionMap::iterator iter = m_Sections.find(name);
    if (iter != m_Sections.end())
    {
        return nullptr;
    }

    String lName = ToLower(name);
    String lType = ToLower(type);

    PropertySection *section = new PropertySection(nullptr, lName, lType);
    m_Sections.insert(std::make_pair(lName, section));

    return section;
}

bool PropertyFile::LoadSection(PropertySection *section, std::fstream &file)
{
    while (!file.eof())
    {
        String line;
        READ_LINE(file, line);

        if (IsSectionEnd(line))
            return true;

        String key, value;
        if (ReadSectionValue(line, key, value))
        {
            section->SetValue(key, value);
            continue;
        }

        bool oneLineSection = false;
        String name, type;
        if (ReadSectionData(line, name, type, oneLineSection))
        {
            if (oneLineSection)
            {
                PropertySection *child = section->AddChildSection(name, type);
            }
            else
            {
                READ_LINE(file, line);
                if (!IsSectionStart(line))
                    return false;

                PropertySection *child = section->AddChildSection(name, type);

                if (!child)
                    return false;

                if (!LoadSection(child, file))
                    return false;
            }
        }
        else
        {
            return false;
        }

        if (file.fail() || file.bad())
            return false;
    }

    return true;
}

bool PropertyFile::ReadSectionValue(const String &in, String &key, String &value)
{
    String s = in;
    TrimString(s);

    int pos1 = (int) s.find_first_of("=");
    int pos2 = (int) s.find_first_of(";");

    if (pos1 >= 0 && pos2 >= 0)
    {
        key = s.substr(0, pos1);
        value = s.substr(pos1 + 1, pos2 - (pos1 + 1));

        TrimString(key);
        ToLower(key);

        TrimString(value);

        return true;
    }

    else if (pos1 < 0 && pos2 >= 0)
    {
        return false;
    }

    return false;
}

bool PropertyFile::ReadSectionData(const String &in, String &name, String &type, bool &oneLineSection)
{
    String s = in;
    TrimString(s);

    int pos1 = (int) s.find_first_of("(");
    int pos2 = (int) s.find_first_of(")");
    int pos3 = (int) s.find_first_of(";");

    if (pos1 < 0 || pos2 < 0)
        return false;

    type = s.substr(0, pos1);
    name = s.substr(pos1 + 1, pos2 - (pos1 + 1));

    TrimString(type);
    TrimString(name);

    oneLineSection = false;
    if (pos3 == (pos2 + 1) && pos2 > pos1)
    {
        oneLineSection = true;
    }

    ToLower(type);
    ToLower(name);

    return true;
}

bool PropertyFile::IsSectionStart(const String &in)
{
    String s = in;
    TrimString(s);
    return (s == "{");
}

bool PropertyFile::IsSectionEnd(const String &in)
{
    String s = in;
    TrimString(s);
    return (s == "}");
}

OutputStream &operator<<(OutputStream &outputStream, const PropertyFile &propertyFile)
{
    const PropertySectionMap &sections = propertyFile.GetSections();

    for (auto iter = sections.begin(); iter != sections.end(); ++iter)
    {
        PropertySection *section = iter->second;
        outputStream << std::endl;
        outputStream << *(section);
    }

    return outputStream;
}

OutputStream &operator<<(OutputStream &outputStream, const PropertySection &propertySection)
{
    String offset = PadString(PropertyFile::GetTabOffset(), ' ');

    outputStream << offset << propertySection.GetType() << "(" << propertySection.GetName() << ")" << std::endl;
    outputStream << offset << "{" << std::endl;

    const PropertyValueMap &values = propertySection.GetValues();
    for (auto iter = values.begin(); iter != values.end(); ++iter)
    {
        outputStream << offset << PadString(PROPERTY_FILE_INDENT_SIZE, ' ') << iter->first << " = " << iter->second.Value << ";" << std::endl;
    }

    const PropertySectionMap &sections = propertySection.GetChildSections();
    if (!sections.empty())
    {
        if (!propertySection.GetValues().empty())
        {
            outputStream << std::endl;
        }

        PropertyFile::PushTab();

        bool first = true;
        for (auto iter = sections.begin(); iter != sections.end(); ++iter)
        {
            PropertySection *section = iter->second;

            if (!first)
                outputStream << std::endl;
            
            first = false;
            outputStream << *(section);
        }

        PropertyFile::PopTab();
    }

    outputStream << offset << "}" << std::endl;

    return outputStream;
}

