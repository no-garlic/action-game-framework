
#include "ApplicationModule.h"
#include "ApplicationCentral.h"

#include "libAnimation.h"
#include "libScene.h"
#include "libComponents.h"

extern bool RegisterObjects();
extern bool InitObjects();

ApplicationModuleManager* ApplicationModuleManager::s_pInstance = NULL;

ApplicationModule::ApplicationModule(const String &name)
:   m_Name(name)
{
    constructor(ApplicationModule);
    ApplicationCentral::GetInstance();
}

ApplicationModule::~ApplicationModule()
{
    destructor(ApplicationModule);
}

int ApplicationModule::Main(const String &commandLine)
{
    if (!Create())
        return -1;

    return Run(commandLine);
}

bool ApplicationModule::Create()
{
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit())
    {
        return false;
    }

    if (!Application.Init(this))
        return false;

    const PropertySection *displaySettings = Application.GetPropertySection("Display");
    if (!displaySettings)
        return false;

    int2 size           = displaySettings->GetValue("Size");
    int2 position       = displaySettings->GetValue("Position");
    bool fullscreen     = displaySettings->GetValue("FullScreen");
    String windowTitle  = displaySettings->GetValue("WindowTitle");

    GLFWmonitor *monitor = nullptr;
    if (fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    m_Window = glfwCreateWindow(size.x, size.y, windowTitle.c_str(), monitor, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        return false;
    }

    if (position.x || position.y)
    {
        glfwSetWindowPos(m_Window, position.x, position.y);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);
    
    glfwSwapInterval(1);

    glfwSetKeyCallback(m_Window, KeyCallback);
    glfwSetWindowSizeCallback(m_Window, ResizeCallback);
    glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
    glfwSetScrollCallback(m_Window, ScrollCallback);
    glfwSetCharCallback(m_Window, CharCallback);

    if (!InitGUI())
        return false;

    if (!RegisterObjects())
        return false;

    if (!Init())
        return false;

    return true;
}

bool ApplicationModule::InitGUI()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, false);
    ImGui_ImplOpenGL2_Init();

    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    return true;
}

int ApplicationModule::Run(const String &commandLine)
{
    bool run = (m_Window != nullptr && !glfwWindowShouldClose(m_Window));
    while (run)
    {
        run = Application.RunFrame();

        if (run)
        {
            if (ImDrawData *gui = ImGui::GetDrawData())
            {
                ImGui_ImplOpenGL2_RenderDrawData(gui);
            }
        }

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        run = (run && m_Window != nullptr && !glfwWindowShouldClose(m_Window));

        if (!run)
        {
            if (m_Window)
            {
                ImGui_ImplOpenGL2_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();

                glfwDestroyWindow(m_Window);
                m_Window = nullptr;
            }

            glfwTerminate();
        }
    }

    return Application.GetExitCode();
}

void ApplicationModule::RequestExit(int exitCode)
{
    glfwSetWindowShouldClose(m_Window, GL_TRUE);
}

bool ApplicationModule::RegisterObjects()
{
    return ::RegisterObjects();
}

bool ApplicationModule::Init()
{
    bool result = ::InitObjects();
    DebugDisplay.Init();
    return result;
}

void ApplicationModule::Shutdown()
{
    DebugDisplay.Shutdown();
}

void ApplicationModule::OnResize(int width, int height)
{    
}

void ApplicationModule::OnKeyPress(int key, int scancode, int action, int mods)
{
}

void ApplicationModule::OnMouseButton(int button, int action, int mods)
{
}

void ApplicationModule::OnScroll(double xoffset, double yoffset)
{
}

void ApplicationModule::OnChar(unsigned int c)
{
}

void ApplicationModule::ErrorCallback(int error, const char *description)
{
    LOG_ERROR << description << ENDLOG;
}

void ApplicationModule::ResizeCallback(GLFWwindow *window, int width, int height)
{
    ApplicationModule *module = (ApplicationModule *) glfwGetWindowUserPointer(window);
    if (module)
    {
        module->OnResize(width, height);
    }
}

void ApplicationModule::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (ApplicationModule *module = (ApplicationModule *) glfwGetWindowUserPointer(window))
    {
        module->OnKeyPress(key, scancode, action, mods);
    }

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void ApplicationModule::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (ApplicationModule *module = (ApplicationModule *) glfwGetWindowUserPointer(window))
    {
        module->OnMouseButton(button, action, mods);
    }

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void ApplicationModule::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (ApplicationModule *module = (ApplicationModule *) glfwGetWindowUserPointer(window))
    {
        module->OnScroll(xoffset, yoffset);
    }

    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void ApplicationModule::CharCallback(GLFWwindow* window, unsigned int c)
{
    if (ApplicationModule *module = (ApplicationModule *) glfwGetWindowUserPointer(window))
    {
        module->OnChar(c);
    }

    ImGui_ImplGlfw_CharCallback(window, c);
}

ApplicationModuleManager &ApplicationModuleManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new ApplicationModuleManager();
        atexit(ApplicationModuleManager::DestroyInstance);
    }

    return *s_pInstance;
}

void ApplicationModuleManager::DestroyInstance()
{
    delete s_pInstance;
    s_pInstance = NULL;
}

void ApplicationModuleManager::Register(BaseCreationFunctor *pCreationFunctor)
{
    String uppercaseName = pCreationFunctor->Name;
    ToUpper(uppercaseName);

    m_Modules.insert(std::make_pair(uppercaseName, pCreationFunctor));
}

ApplicationModule *ApplicationModuleManager::Create(const char *name)
{
    String uppercaseName = ToUpper(name) + ToUpper("Module");

    std::map<std::string, ApplicationModuleManager::BaseCreationFunctor *>::iterator findIter = m_Modules.find(uppercaseName);
    if (findIter == m_Modules.end())
        return NULL;

    return findIter->second->Create(name);
}

