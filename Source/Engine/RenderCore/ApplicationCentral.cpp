
#include "ApplicationCentral.h"
#include "DebugDisplay.h"

ApplicationCentral *ApplicationCentral::s_pInstance = NULL;

Array<ApplicationCentral::CreationFunctor *> ApplicationCentral::s_CreationFunctors;

FileStream g_LogFile;

ApplicationCentral::ApplicationCentral()
:   m_pModule(NULL),
    m_Initialised(false),
    m_Destructed(false),
    m_Paused(false),
    m_RequestExit(false),
    m_RequestExitNextFrame(false),
    m_IsFullScreen(false),
    m_ExitCode(0),
    m_FrameTime(0.0f),
    m_RunTime(0.0f),
    m_FrameRate(0),
    m_FrameCount(0),
    m_IntervalFrames(0),
    m_IntervalTime(0.0f)
{
    constructor(ApplicationCentral);
}

ApplicationCentral::~ApplicationCentral()
{
    destructor(ApplicationCentral);

    if (!m_Destructed)
    {
        DestructResources();
    }
}

ApplicationCentral &ApplicationCentral::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new ApplicationCentral();
        atexit(&ApplicationCentral::DestroyInstance);
    }

    return *s_pInstance;
}

void ApplicationCentral::DestroyInstance()
{
    delete s_pInstance;
    s_pInstance = NULL;
}

bool ApplicationCentral::Init(ApplicationModule *pModule)
{
    if (m_Initialised)
        return false;

    m_pModule = pModule;

    bool configLoaded = false;
    if (m_Properties.Load("Data\\Config\\Engine.cfg"))
    {
        configLoaded = true;
        m_DataPath = "";
    }

    if (!configLoaded)
    {
        LOG_ERROR << "Failed to load the engine config file" << ENDLOG;
        return false;
    }

    String moduleConfigName = "Data\\Config\\";
    moduleConfigName += pModule->GetName();
    moduleConfigName += ".cfg";

    if (!m_Properties.Merge(moduleConfigName))
    {
        LOG_ERROR << "Failed to process the module config file" << ENDLOG;
        return false;
    }

    FileSystem.SetBasePath(m_DataPath);

    if (!CreateLogFile(pModule))
    {
        LOG_ERROR << "Failed to create the log file" << ENDLOG;
        return false;
    }
    
    m_Timer.Reset();
    m_Timer.Start();

    m_Initialised = true;
    return true;
}

void LogFileCallbackFunction(const char *pMsg)
{
    g_LogFile << pMsg << std::endl;
}

bool ApplicationCentral::CreateLogFile(ApplicationModule *pModule)
{
    bool isLogging = true;
    String logFilename = "";

    PropertySection *section = m_Properties.GetSection("LogFile");
    if (section)
    {
        bool enabled = section->GetValue("Enabled");
        logFilename = section->GetValue("Filename");

        if ((enabled == false) || AppArgs.HasOption("-nolog"))
            isLogging = false;
    }

    if (AppArgs.HasOption("-log"))
    {
        isLogging = true;
        const StringArray &optionArgs = AppArgs.GetOptionArgs("-log");
        if (!optionArgs.empty())
        {
            logFilename = optionArgs[0];
        }
    }

    if (logFilename.empty())
        return true;

    if (isLogging)
    {
        g_LogFile.open(logFilename.c_str(), std::ios_base::out);
        if (!g_LogFile.is_open())
            return false;

        LOG_CALLBACK(LogFileCallbackFunction);
    }

    return true;
}

bool ApplicationCentral::RunFrame()
{
    if (!m_Initialised)
        return false;

    m_RequestExit = m_RequestExitNextFrame;

    if (m_RequestExit)
    {
        DestructResources();
        return false;
    }

    m_RunTime = m_Timer.GetTime(&m_FrameTime);

    m_IntervalFrames++;
    m_IntervalTime += m_FrameTime;

    if (m_IntervalTime > 1.0f)
    {
        m_FrameRate = static_cast<unsigned int>(static_cast<float>(m_IntervalFrames) / m_IntervalTime);

        m_IntervalFrames = 0;
        m_IntervalTime = 0.0f;
    }

    RunOneProcess();

    m_FrameCount++;

    return !m_RequestExit;
}

ContentDatabaseBase *ApplicationCentral::GetContentDatabase(const String &objectType) const
{
    auto iter = m_ContentDatabases.find(objectType);
    if (iter == m_ContentDatabases.end())
        return nullptr;
    return iter->second;
}

void ApplicationCentral::DestructResources()
{
    if (!m_Initialised)
        return;

    if (!m_Destructed)
    {
        m_Destructed = true;

        if (m_pModule)
        {
            Shutdown();
        }
    }

    for (auto iter = m_ContentDatabases.begin(); iter != m_ContentDatabases.end(); ++iter)
    {
        delete iter->second;
    }
    m_ContentDatabases.clear();

    DestroyAllCreationFunctors();
}

void ApplicationCentral::Shutdown()
{
    m_pModule->Shutdown();
    delete m_pModule;
    m_pModule = nullptr;
}

void ApplicationCentral::RunOneProcess()
{
    PROFILE_START(PROFILE_FRAME);
    {
        PROFILE_START(PROFILE_UPDATE);
        {
            if (!m_Paused)
            {
                m_pModule->GetCamera().Apply(m_pModule->GetWindow());
                m_pModule->GetCamera().Clear();
                m_pModule->Update();
            }
        }
        PROFILE_STOP(PROFILE_UPDATE);

        PROFILE_START(PROFILE_DRAW);
        {
            m_pModule->Draw();
        }
        PROFILE_STOP(PROFILE_DRAW);

        PROFILE_START(PROFILE_DRAW_UI);
        {
            m_pModule->DrawUI();
        }
        PROFILE_STOP(PROFILE_DRAW_UI);

        DebugDisplay.Draw();
    }
    PROFILE_STOP(PROFILE_FRAME);

    PROFILE_RESET_ALL;
}

void ApplicationCentral::AddCreationFunctor(CreationFunctor *pFunc)
{
    s_CreationFunctors.push_back(pFunc);
}

bool ApplicationCentral::CreateAllCreationFunctors()
{
    bool result = true;
    for (uint i = 0; i < s_CreationFunctors.size(); i++)
    {
        ApplicationCentral::CreationFunctor *pFunc = s_CreationFunctors[i];

        if (!pFunc->Create())
        {
            LOG_ERROR << "Failed to create: " << pFunc->Name << ENDLOG;
            result = false;
        }
    }

    return result;
}

bool ApplicationCentral::DestroyAllCreationFunctors()
{
    bool result = true;
    for (uint i = 0; i < s_CreationFunctors.size(); i++)
    {
        ApplicationCentral::CreationFunctor *pFunc = s_CreationFunctors[i];

        if (!pFunc->Destroy())
        {
            LOG_ERROR << "Failed to destroy: " << pFunc->Name << ENDLOG;
            result = false;
        }
    }

    s_CreationFunctors.clear();
    return result;
}
