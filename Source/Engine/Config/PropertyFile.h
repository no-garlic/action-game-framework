
#ifndef __CFG_PROPERTY_FILE_H_
#define __CFG_PROPERTY_FILE_H_

#include "libStandard.h"
#include "PropertySection.h"

#define PROPERTY_FILE_INDENT_SIZE ((uint) 4)

class PropertyFile
{
public:
    PropertyFile();
    ~PropertyFile();

    bool Load(const String &filename);
    bool ReLoad() { return Load(m_Filename); }

    bool IsLoaded() const { return m_Loaded; }

    bool Merge(const String &filename);

    bool Save() { return SaveAs(m_Filename); }
    bool SaveAs(const String &filename);

    const String &GetFileName() const { return m_Filename; }

    const PropertySection *GetSection(const String &name) const;
    PropertySection *GetSection(const String &name);

    PropertySectionMap &GetSections() { return m_Sections; }
    const PropertySectionMap &GetSections() const { return m_Sections; }

    PropertySection *AddSection(const String &type, const String &name);

    static uint GetTabOffset() { return s_TabOffset; }
    static void PushTab() { s_TabOffset += PROPERTY_FILE_INDENT_SIZE; }
    static void PopTab()  { s_TabOffset -= PROPERTY_FILE_INDENT_SIZE; }

private:
    PropertyFile(const PropertyFile &other);
    PropertyFile &operator=(const PropertyFile &other);

    bool LoadSection(PropertySection *section, std::fstream &file);
    bool ReadSectionData(const String &in, String &name, String &type, bool &oneLineSection);
    bool ReadSectionValue(const String &in, String &key, String &value);

    bool IsSectionStart(const String &in);
    bool IsSectionEnd(const String &in);

    String m_Filename;
    bool m_Loaded;

    PropertySectionMap m_Sections;

    static uint s_TabOffset;
};

OutputStream &operator<<(OutputStream &outputStream, const PropertyFile &propertyFile);
OutputStream &operator<<(OutputStream &outputStream, const PropertySection &propertySection);

#endif // __CFG_PROPERTY_FILE_H_
