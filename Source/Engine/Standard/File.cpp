
#include "Type.h"
#include "File.h"
#include "Algorithm.h"
#include "Log.h"
#include "Str.h"
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define FILE_SEPERATOR "/"

FileHandle::FileHandle()
 :  m_pDatabase(NULL)
{
    constructor(FileHandle);
}

FileHandle::FileHandle(const String &filename, const FileDatabase &fileDatabase)
 :  m_Filename(filename),
    m_pDatabase(&fileDatabase)
{
    constructor(FileHandle);

    m_AbsoluteFilename = fileDatabase.GetAbsolutePath() + FILE_SEPERATOR + filename;

    int pos = m_AbsoluteFilename.find_first_of("\\");
    while (pos != -1)
    {
        m_AbsoluteFilename[pos] = '/';
        pos = m_AbsoluteFilename.find_first_of("\\");
    }
}

FileHandle::FileHandle(const FileHandle &other)
{
    constructor(FileHandle);

    m_Filename = other.m_Filename;
    m_AbsoluteFilename = other.m_AbsoluteFilename;
    m_pDatabase = other.m_pDatabase;
}

FileHandle::~FileHandle()
{
    destructor(FileHandle);
}

FileHandle &FileHandle::operator=(const FileHandle &other)
{
    m_Filename = other.m_Filename;
    m_AbsoluteFilename = other.m_AbsoluteFilename;
    m_pDatabase = other.m_pDatabase;

    return *this;
}

const String &FileHandle::GetFilename() const
{
    return m_Filename;
}

const String &FileHandle::GetAbsoluteFilename() const
{
    return m_AbsoluteFilename;
}

void FileHandle::SetFilename(const String &filename)
{
    m_Filename = filename;

    if (m_pDatabase)
        m_AbsoluteFilename = m_pDatabase->GetAbsolutePath() + FILE_SEPERATOR + filename;

    int pos = m_AbsoluteFilename.find_first_of("\\");
    while (pos != -1)
    {
        m_AbsoluteFilename[pos] = '/';
        pos = m_AbsoluteFilename.find_first_of("\\");
    }
}

void FileHandle::SetFileDatbase(FileDatabase *pFileDatabase)
{
    m_pDatabase = pFileDatabase;

    if (!m_Filename.empty())
        m_AbsoluteFilename = m_pDatabase->GetAbsolutePath() + FILE_SEPERATOR + m_Filename;

    int pos = m_AbsoluteFilename.find_first_of("\\");
    while (pos != -1)
    {
        m_AbsoluteFilename[pos] = '/';
        pos = m_AbsoluteFilename.find_first_of("\\");
    }
}

const FileDatabase &FileHandle::GetFileDatabase() const
{
    return *m_pDatabase;
}

bool FileHandle::Exists() const
{
    int fd = _open(m_AbsoluteFilename.c_str(), _O_RDONLY | _O_BINARY, _S_IREAD);
    if (fd == -1)
        return false;

    _close(fd);
    return true;
}

bool FileHandle::IsValid() const
{
    if (!m_pDatabase)
        return false;

    if (m_AbsoluteFilename.empty() || m_Filename.empty())
        return false;

    return true;
}

__time32_t FileHandle::GetTimestamp() const
{
    int fd = _open(m_AbsoluteFilename.c_str(), _O_RDONLY | _O_BINARY, _S_IREAD);
    if (fd == -1)
        return -1;

    struct _stat stat;
    if (_fstat(fd, &stat) != 0)
        return -1;

    _close(fd);

    __time32_t accessTime = stat.st_mtime;

    return accessTime;
}

bool FileIsUpToDate(FileHandle src, FileHandle dest)
{
    if (!dest.Exists())
        return false;

    if (!src.Exists())
        return false;

    __time32_t srcTime = src.GetTimestamp();
    __time32_t destTime = dest.GetTimestamp();
    __time32_t timeDiff = destTime - srcTime;

    return (timeDiff >= 0);
}

FileDatabase::FileDatabase(const String &path)
    : m_Path(path)
{
    constructor(FileDatabase);

    m_AbsolutePath = FileSystem.GetBasePath();

    if (!m_Path.empty())
        m_AbsolutePath += (FILE_SEPERATOR + m_Path);
}

FileDatabase::~FileDatabase()
{
    destructor(FileDatabase);
}

FileHandleArray FileDatabase::ListFiles(const String &wildcard) const
{
    FileHandleArray allFiles;
    StringArray     allPaths;

    if (!FindFiles(m_AbsolutePath, wildcard, allFiles, allPaths))
    {
        LOG_ERROR << "Failed to find files in path: " << m_AbsolutePath << ENDLOG;
        return allFiles;
    }

    return allFiles;
}

FileHandleArray FileDatabase::ListFiles(const StringArray &wildcardArray) const
{
    FileHandleArray allFiles;
    StringArray     allPaths;

    StringArray::ConstIterator iter(wildcardArray.begin()), end(wildcardArray.end());
    for (; iter != end; ++iter)
    {
        if (!FindFiles(m_AbsolutePath, (*iter), allFiles, allPaths))
            LOG_ERROR << "Failed to find " << (*iter) << " files in path: " << m_AbsolutePath << ENDLOG;
    }

    return allFiles;
}

bool FileDatabase::FindFiles(const String &path, const String &wildcard, FileHandleArray &files, StringArray &paths) const
{
    char currDir[_MAX_PATH];
    if (_getcwd(currDir, _MAX_PATH) == NULL)
        return false;

    if (chdir(path.c_str()) != 0)
        return false;

    _finddata_t fileInfo;
    long hFile;

    if ((hFile = _findfirst(wildcard.c_str(), &fileInfo)) == -1L)
        return false;

    if (fileInfo.attrib & _A_SUBDIR)
    {
        if (String(fileInfo.name) != "." && String(fileInfo.name) != "..")
            paths.push_back(path + FILE_SEPERATOR + fileInfo.name);
    }
    else
    {
        files.push_back(FileHandle(fileInfo.name, *this));
    }

    while (_findnext(hFile, &fileInfo) == 0)
    {
        if (fileInfo.attrib & _A_SUBDIR)
        {
            if (String(fileInfo.name) != "." && String(fileInfo.name) != "..")
                paths.push_back(path + FILE_SEPERATOR + fileInfo.name);
        }
        else
        {
            files.push_back(FileHandle(fileInfo.name, *this));
        }
    }

    if (_findclose(hFile) != 0)
        return false;

    if (chdir(currDir) != 0)
        return false;

    return true;
}

StringArray FileDatabase::ListPaths(bool recursive) const
{
    StringArray allPaths;

    if (!FindPaths(m_AbsolutePath, allPaths, recursive))
    {
        LOG_ERROR << "Failed to find paths in path: " << m_AbsolutePath << ENDLOG;
        return allPaths;
    }

    return allPaths;
}

bool FileDatabase::FindPaths(const String &path, StringArray &paths, bool recursive) const
{
    StringArray newPaths;

    char currDir[_MAX_PATH];
    if (_getcwd(currDir, _MAX_PATH) == NULL)
        return false;

    if (chdir(path.c_str()) != 0)
        return false;

    _finddata_t fileInfo;
    long hFile;

    if ((hFile = _findfirst("*.*", &fileInfo)) == -1L)
        return false;

    if (fileInfo.attrib & _A_SUBDIR)
    {
        if (String(fileInfo.name) != "." && String(fileInfo.name) != "..")
            newPaths.push_back(path + FILE_SEPERATOR + fileInfo.name);
    }

    while (_findnext(hFile, &fileInfo) == 0)
    {
        if (fileInfo.attrib & _A_SUBDIR)
        {
            if (String(fileInfo.name) != "." && String(fileInfo.name) != "..")
                newPaths.push_back(path + FILE_SEPERATOR + fileInfo.name);
        }
    }

    if (_findclose(hFile) != 0)
        return false;

    if (chdir(currDir) != 0)
        return false;

    if (recursive)
    {
        StringArray subPaths;
        for (uint i = 0; i < newPaths.size(); i++)
        {
            String nextPath = newPaths[i];

            StringArray findPaths;
            if (!FindPaths(nextPath, findPaths, recursive))
                return false;

            subPaths += findPaths;
        }
        newPaths += subPaths;
    }

    paths += newPaths;
    return true;
}

FileHandle FileDatabase::MakeFileHandle(const String &filename) const
{
    return FileHandle(filename, *this);
}

bool FindFileInPath(const String &basePath, const String &filename, String &absoluteFilename, String &relativePath)
{
    WIN32_FIND_DATA wfd;
    HANDLE hFind;
    String s = basePath + "*.*";
    hFind = FindFirstFile(s.c_str(), &wfd);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            bool skip = false;
            if (strlen(wfd.cFileName) > 0 && wfd.cFileName[0] == '.')
                skip = true;

            if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !skip)
            {
                String pathNow = relativePath;
                relativePath += wfd.cFileName;
                relativePath += FILE_SEPERATOR;
                if (FindFileInPath(basePath + wfd.cFileName + FILE_SEPERATOR, filename, absoluteFilename, relativePath))
                    return true;
                relativePath = pathNow;
            }
            else
            {
                if (_stricmp(filename.c_str(), wfd.cFileName) == 0)
                {
                    absoluteFilename = basePath + wfd.cFileName;
                    // LOG_INFO << "Found File: " << absoluteFilename << ENDLOG;
                    return true;
                }
            }
        } while (FindNextFile(hFind, &wfd));
        FindClose(hFind);
    }
    return false;
}

FileHandle FileDatabase::FindFileHandle(const String &filename) const
{
    // Try to find the file in the root path of the file database first
    FileHandle handle = FileHandle(filename, *this);
    if (handle.Exists())
        return handle;

    String absoluteFilename, relativePath;
    if (FindFileInPath(m_AbsolutePath + FILE_SEPERATOR, filename, absoluteFilename, relativePath))
        handle = FileHandle(relativePath + filename, *this);

    return handle;
}

const String &FileDatabase::GetPath() const
{
    return m_Path;
}

const String &FileDatabase::GetAbsolutePath() const
{
    return m_AbsolutePath;
}

FileSystemManager::FileSystemManager()
{
}

FileSystemManager::~FileSystemManager()
{
    Map<String, FileDatabase *>::iterator iter(m_DatabaseArray.begin()), end(m_DatabaseArray.end());
    for (; iter != end; ++iter)
        delete iter->second;

    m_DatabaseArray.clear();
}

void FileSystemManager::SetBasePath(const String &basePath, bool fromCurrDir)
{
    m_Path = basePath;

    if (fromCurrDir)
    {
        char currDir[_MAX_PATH];
        if (_getcwd(currDir, _MAX_PATH) != NULL)
            m_Path = String(currDir) + FILE_SEPERATOR + basePath;
    }

    int pos = m_Path.find_first_of("\\");
    while (pos != -1)
    {
        m_Path[pos] = '/';
        pos = m_Path.find_first_of("\\");
    }

    // Get the absoute path
    Stack<String> fileStack;
    pos = m_Path.find_first_of(FILE_SEPERATOR);
    while (pos != -1)
    {
        String path = m_Path.substr(0, pos);
        if (path == "..")
        {
            if (!fileStack.empty())
                fileStack.pop();
        }
        else
        {
            fileStack.push(path);
        }

        m_Path = m_Path.substr(pos);
        if (m_Path.length() != 0 && m_Path[0] == FILE_SEPERATOR[0])
            m_Path = m_Path.substr(1);

        pos = m_Path.find_first_of(FILE_SEPERATOR);
    }

    if (!m_Path.empty())
        fileStack.push(m_Path);

    m_Path.clear();
    while (!fileStack.empty())
    {
        m_Path = fileStack.top() + FILE_SEPERATOR + m_Path;
        fileStack.pop();
    }

    if (m_Path[m_Path.length() - 1] == FILE_SEPERATOR[0])
        m_Path = m_Path.substr(0, m_Path.length() - 1);
}

const String &FileSystemManager::GetBasePath() const
{
    return m_Path;
}

FileDatabase *FileSystemManager::AddDatabase(const String &name, const String &path)
{
    String lName = ToLower(name);

    if (m_DatabaseArray.find(lName) != m_DatabaseArray.end())
        return false;

    String newPath = path;

    int pos = m_Path.find_first_of("\\");
    while (pos != -1)
    {
        m_Path[pos] = '/';
        pos = m_Path.find_first_of("\\");
    }

    // Remove any trailing slash
    if (newPath[newPath.length() - 1] == FILE_SEPERATOR[0])
        newPath = newPath.substr(0, newPath.length() - 1);

    // Add the database
    FileDatabase *pDatabase = new FileDatabase(newPath);
    m_DatabaseArray.insert(MakePair(lName, pDatabase));
    return pDatabase;
}

FileDatabase *FileSystemManager::operator()(const String &name)
{
    String lName = ToLower(name);

    Map<String, FileDatabase *>::iterator findIter = m_DatabaseArray.find(lName);
    if (findIter == m_DatabaseArray.end())
        return nullptr;

    return findIter->second;
}
