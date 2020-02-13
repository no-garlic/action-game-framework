
#include "ContentDatabase.h"
#include "ApplicationCentral.h"

ContentDatabaseBase::ContentDatabaseBase()
{
    constructor(ContentDatabase);
}

ContentDatabaseBase::~ContentDatabaseBase()
{
    destructor(ContentDatabase);
}

bool ContentDatabaseBase::Init(const String &objectType, const String &databaseName)
{
    m_ObjectType = ToLower(objectType);
    m_FileDatabase = FileSystem(databaseName);
    return (m_FileDatabase != nullptr);
}

bool ContentDatabaseBase::Load(const String &filename)
{
    FileHandle fh = m_FileDatabase->MakeFileHandle(filename);
    if (!fh.Exists())
    {
        LOG_ERROR << "Failed to find the file: " << filename << " in the file database: " << m_FileDatabase->GetAbsolutePath() << ENDLOG;
        return false;
    }

    String lFilename = ToLower(fh.GetFilename());
    PropertyFile &propertyFile = m_PropertyFiles[lFilename];

    if (propertyFile.IsLoaded())
    {
        LOG_ERROR << "Cannot add duplicate texture config files to the texture database" << ENDLOG;
        return false;
    }

    if (!propertyFile.Load(fh.GetAbsoluteFilename()))
    {
        LOG_ERROR << "Failed to load the content config file: " << filename << ENDLOG;
        return false;
    }

    for (auto pair : propertyFile.GetSections())
    {
        PropertySection *section = pair.second;
        String lName = ToLower(pair.first);
        String lSection = ToLower(section->GetType());

        if (lSection != m_ObjectType)
            continue;

        ObjectID objectID = GenerateObjectID(m_ObjectType, lName);

        if (m_Blueprints.find(objectID) != m_Blueprints.end())
        {
            LOG_ERROR << "Attempting to add a duplicate object to the Content Database: " << pair.first << ENDLOG;
            return false;
        }

        m_Blueprints[objectID] = section;
    }

    return true;
}

void ContentDatabaseBase::Clear()
{
    m_Blueprints.clear();
    m_PropertyFiles.clear();
}

BaseObject *ContentDatabaseBase::GetOrCreateObject(const String &objectType, const String &objectName)
{
    if (BaseObject *object = ObjectManager.Find(objectType, objectName))
    {
        return object;
    }

    String lName = ToLower(objectName);
    ObjectID objectID = GenerateObjectID(m_ObjectType, lName);

    auto iter = m_Blueprints.find(objectID);

    if (iter != m_Blueprints.end())
    {
        BaseObject *object = ObjectManager.Create(iter->second);
        return object;
    }

    return nullptr;
}

bool ContentDatabaseBase::DuplicateBlueprint(const String &objectType, const String &name, const String &copyName)
{
    String lName = ToLower(name);
    ObjectID objectID = GenerateObjectID(m_ObjectType, lName);

    auto iter = m_Blueprints.find(objectID);
    if (iter == m_Blueprints.end())
    {
        return false;
    }

    PropertySection *source = iter->second;

    PropertySection *copy = DuplicateBlueprint(source, copyName);
    if (!copy)
    {
        return false;
    }

    lName = ToLower(copyName);
    objectID = GenerateObjectID(m_ObjectType, lName);

    m_Blueprints[objectID] = copy;
    return true;
}

PropertySection *ContentDatabaseBase::DuplicateBlueprint(PropertySection *source, const String &name)
{
    if (!source)
        return nullptr;

    if (PropertySection *section = m_DuplicatedPropertyFile.GetSection(name))
        return section;

    PropertySection *sectionCopy = m_DuplicatedPropertyFile.AddSection(source->GetType(), name);
    if (!sectionCopy)
        return nullptr;

    RecurseCopySection(source, sectionCopy);
    return sectionCopy;
}

void ContentDatabaseBase::RecurseCopySection(PropertySection *source, PropertySection *dest)
{
    PropertyValueMap &sourceValues = source->GetValues();
    for (auto valuePair : sourceValues)
    {
        dest->SetValue(valuePair.first, valuePair.second.AsString());
    }
    
    PropertySectionMap &sourceSections = source->GetChildSections();
    for (auto sectionPair : sourceSections)
    {
        PropertySection *sourceChild = sectionPair.second;
        PropertySection *destChild = dest->AddChildSection("", sourceChild->GetType());
        RecurseCopySection(sourceChild, destChild);
    } 
}

