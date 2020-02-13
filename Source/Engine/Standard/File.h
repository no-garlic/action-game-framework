
#ifndef __STD_FILE_H_
#define __STD_FILE_H_

#include "Type.h"
#include "Str.h"
#include "Singleton.h"

class FileHandle;
class FileDatabase;
class FileSystemManager;

typedef SingletonHolder<FileSystemManager, CreateUsingNew<FileSystemManager> > FileSystemManagerSingleton;
#define FileSystem FileSystemManagerSingleton::Instance()

class FileHandle
{
public:
    FileHandle();
    FileHandle(const String &filename, const FileDatabase &fileDatabase);
    FileHandle(const FileHandle &other);
    ~FileHandle();

    bool IsValid() const;
    
    FileHandle &operator=(const FileHandle &other);
    
    const String &GetFilename() const;
    const String &GetAbsoluteFilename() const;

    void SetFilename(const String &filename);
    void SetFileDatbase(FileDatabase *pFileDatabase);

    const FileDatabase &GetFileDatabase() const;
    
    bool Exists() const;
    
    __time32_t GetTimestamp() const;

private:
    const FileDatabase *m_pDatabase;
    String              m_Filename;
    String              m_AbsoluteFilename;

};

inline std::ostream &operator<<(std::ostream &stream, const FileHandle &handle)
{
    stream << handle.GetAbsoluteFilename();
    return stream;
}

typedef Array<FileHandle> FileHandleArray;

bool FileIsUpToDate(FileHandle src, FileHandle dest);
bool FindFileInPath(const String &basePath, const String &filename, String &absoluteFilename, String &relativePath);

class FileDatabase
{
private:
    friend class FileSystemManager;

    FileDatabase(const String &path);
    virtual ~FileDatabase();

public:
    FileHandleArray ListFiles(const String &wildcard = "*.*") const;
    FileHandleArray ListFiles(const StringArray &wildcardArray) const;
    StringArray     ListPaths(bool recursive) const;

    const String &GetPath() const;
    const String &GetAbsolutePath() const;

    FileHandle MakeFileHandle(const String &filename) const;
    FileHandle FindFileHandle(const String &filename) const;

private:
    bool FindFiles(const String &path, const String &wildcard, FileHandleArray &files, StringArray &paths) const;
    bool FindPaths(const String &path, StringArray &paths, bool recursive) const;

    String m_Path;
    String m_AbsolutePath;

};

class FileSystemManager
{
private:
    friend struct CreateUsingNew<FileSystemManager>;

    FileSystemManager();
    ~FileSystemManager();

public:
    void SetBasePath(const String &basePath, bool fromCurrDir = true);
    const String &GetBasePath() const;

    FileDatabase *AddDatabase(const String &name, const String &path);
    FileDatabase *operator()(const String &name);

private:
    String                      m_Path;
    Map<String, FileDatabase *> m_DatabaseArray;

};

#endif // __STD_FILE_H_
