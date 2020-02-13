
#ifndef __RC_APPLICATION_MODLE_H_
#define __RC_APPLICATION_MODLE_H_

#include "libStandard.h"

#include "Camera.h"

#define REGISTER_MODULE(x) namespace A##x { ApplicationModuleManager::CreationFunctor<x> g_ModuleReg(#x); };

class ApplicationModule
{
public:
    ApplicationModule(const String &name);
    virtual ~ApplicationModule();

    virtual int Main(const String &commandLine);

    virtual bool Create();
    virtual int Run(const String &commandLine);

    GLFWwindow *GetWindow() { return m_Window; }
    const String &GetName() { return m_Name; }

    Camera &GetCamera() { return m_Camera; }

    virtual bool RegisterObjects();
    virtual bool Init();
    virtual void Shutdown();

    virtual void Update() {}
    virtual void Draw() {}
    virtual void DrawUI() {}

    virtual void OnResize(int width, int height);
    virtual void OnKeyPress(int key, int scancode, int action, int mods);
    virtual void OnMouseButton(int button, int action, int mods);
    virtual void OnScroll(double xoffset, double yoffset);
    virtual void OnChar(unsigned int c);

protected:
    friend class ApplicationCentral;

    virtual bool InitGUI();

    String      m_Name;
    GLFWwindow *m_Window;
    Camera      m_Camera;

private:
    virtual void RequestExit(int exitCode);

    static void ErrorCallback(int error, const char *description);
    static void ResizeCallback(GLFWwindow *window, int width, int height);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void CharCallback(GLFWwindow* window, unsigned int c);

};

class ApplicationModuleManager
{
public:
    struct BaseCreationFunctor
    {
        BaseCreationFunctor(const char *name) : Name(name) {}

        virtual ApplicationModule *Create(const String &name) = 0;

        std::string Name;
    };
    
    template <class T>
    struct CreationFunctor : public BaseCreationFunctor
    {
        CreationFunctor(const char *name) : BaseCreationFunctor(name)
        {
            ApplicationModuleManager::GetInstance().Register(this);
        }
        
        virtual ApplicationModule *Create(const String &name)
        {
            return new T(name);
        };
    };
    
    static ApplicationModuleManager &GetInstance();
    static void DestroyInstance();

    void Register(BaseCreationFunctor *pCreationFunctor);

    ApplicationModule *Create(const char *name);

private:
    static ApplicationModuleManager              *s_pInstance;
    std::map<std::string, BaseCreationFunctor *>  m_Modules;

};

#endif // __RC_APPLICATION_MODLE_H_
