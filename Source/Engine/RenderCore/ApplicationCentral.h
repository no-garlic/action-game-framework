
#ifndef __RC_APPLICATION_CENTRAL_H_
#define __RC_APPLICATION_CENTRAL_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"

#include "ApplicationModule.h"
#include "OpenGLResolver.h"
#include "Texture.h"
#include "Sprite.h"
#include "ContentDatabase.h"

#define Application ApplicationCentral::GetInstance()
#define ObjectManager ApplicationCentral::GetInstance().GetObjectManager()
#define OpenGLResolver ApplicationCentral::GetInstance().GetOpenGLResolver()

class ApplicationCentral
{
public:
    static ApplicationCentral &GetInstance();
    static void DestroyInstance();

private:
    ApplicationCentral();
    ~ApplicationCentral();

public:
    bool Init(ApplicationModule *pModule);
    bool IsInitialised() const { return m_Initialised; }
    void Shutdown();

    ApplicationModule *GetModule() { return m_pModule; }

    _OpenGLResolver &GetOpenGLResolver() { return m_OpenGLResolver; }
    _ObjectManager &GetObjectManager() { return m_ObjectManager; }
    
    template <class T>
    ContentDatabase<T> *Register(const String &databaseName)
    {
        m_ObjectManager.Register<T>();

        String objectType = T::s_TypeInfo.m_pName;   

        if (ContentDatabaseBase *existingDatabase = GetContentDatabase(objectType))
            return (ContentDatabase<T> *) existingDatabase;

        ContentDatabase<T> *contentDatabase = new ContentDatabase<T>();
        
        if (contentDatabase->Init(databaseName))
        {
            m_ContentDatabases[objectType] = contentDatabase;
            return contentDatabase;
        }

        delete contentDatabase;
        return nullptr;
    }

    ContentDatabaseBase *GetContentDatabase(const String &objectType) const;

    template <class T>
    ContentDatabase<T> *GetContentDatabase()
    {
        String objectType = T::s_TypeInfo.m_pName;
        return (ContentDatabase<T> *) GetContentDatabase(objectType);
    }

    bool RunFrame();
    
    inline void RequestExit(int exitCode);
    inline int  GetExitCode() { return m_ExitCode; }
    
    inline bool IsPaused();
    inline void Pause();
    inline void Resume();
    
    inline float GetRunningTime();
    inline float GetFrameTime();
    inline unsigned int GetFrameRate();
    inline uqword GetFrameCount();

    const PropertyFile &GetProperties() { return m_Properties; }
    const PropertySection *GetPropertySection(const String &sectionName) { return m_Properties.GetSection(sectionName); }

    struct CreationFunctor
    {
        CreationFunctor(const String &name) : Name(name) {}
        virtual bool Create() = 0;
        virtual bool Destroy() = 0;
        String Name;
    };

    static void AddCreationFunctor(CreationFunctor *pFunc);
    static bool CreateAllCreationFunctors();
    static bool DestroyAllCreationFunctors();

private:
    void RunOneProcess();
    bool InitialiseRenderTarget();

    void DestructResources();

    bool CreateLogFile(ApplicationModule *pModule);

    static ApplicationCentral   *s_pInstance;      
    bool                  m_Initialised;           
    bool                  m_Destructed;            
    bool                  m_IsFullScreen;          
    ApplicationModule    *m_pModule;               
    bool                  m_RequestExit;           
    bool                  m_RequestExitNextFrame;  
    int                   m_ExitCode;              
    Timer                 m_Timer;                 
    bool                  m_Paused;                
    float                 m_FrameTime;             
    float                 m_RunTime;               
    uint                  m_FrameRate;             
    uqword                m_FrameCount;            
    uint                  m_IntervalFrames;        
    float                 m_IntervalTime;          
    String                m_WindowTitle;           
    String                m_DataPath;              
    _OpenGLResolver       m_OpenGLResolver;        
    _ObjectManager        m_ObjectManager;         
    PropertyFile          m_Properties;     

    Map<String, ContentDatabaseBase *> m_ContentDatabases;

    static Array<CreationFunctor *> s_CreationFunctors;
};

inline void ApplicationCentral::RequestExit(int exitCode)
{
    m_pModule->RequestExit(exitCode);
    m_RequestExitNextFrame = true;
    m_ExitCode = exitCode;
}

inline void ApplicationCentral::Pause()
{
    m_Paused = true;
    m_Timer.Stop();
}

inline void ApplicationCentral::Resume()
{
    m_Paused = false;
    m_Timer.Start();
}

inline bool ApplicationCentral::IsPaused()
{
    return m_Paused;
}

inline float ApplicationCentral::GetRunningTime()
{
    return m_RunTime;
}

inline float ApplicationCentral::GetFrameTime()
{
    return m_FrameTime;
}

inline unsigned int ApplicationCentral::GetFrameRate()
{
    return m_FrameRate;
}

inline uqword ApplicationCentral::GetFrameCount()
{
    return m_FrameCount;
}

template<class T>
Reference<T> GetOrCreateObjectFromContentDatabase(const String &objectName, bool reportError = true)
{
    Reference<T> object;

    ContentDatabase<T> *database = Application.GetContentDatabase<T>();
    if (!database)
    {
        if (reportError)
        {
            LOG_ERROR << "Unable to find the content database for the object type: " << T::s_TypeInfo.m_pName << ENDLOG;
        }
        return object;
    }

    object = database->GetOrCreateObject(objectName);
    if (object.IsNotValid())
    {
        if (reportError)
        {
            LOG_ERROR << "Unable to find the " << T::s_TypeInfo.m_pName << ": " << objectName << " in the content database" << ENDLOG;
        }
    }
    return object;        
}

template<class T>
Reference<T> GetOrCreateObjectFromContentDatabase(const Serializer &serializer, const String &name, bool reportError = true)
{
    Reference<T> object;

    String objectName = serializer(name);
    if (objectName.length() == 0)
    {
        if (reportError)
        {
            LOG_ERROR << "Property [" << name << "] not found while trying to create object type: " << T::s_TypeInfo.m_pName << ENDLOG;        
        }
        return object;
    }

    return GetOrCreateObjectFromContentDatabase<T>(objectName, reportError);
}

#endif // __RC_APPLICATION_CENTRAL_H_

