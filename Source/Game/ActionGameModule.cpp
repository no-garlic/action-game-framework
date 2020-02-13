
#include "ActionGameModule.h"

ActionGameModule::ActionGameModule(const String &name)
:   ApplicationModule(name)
{
    constructor(ActionGameModule);
}

ActionGameModule::~ActionGameModule()
{
    destructor(ActionGameModule);
}

bool ActionGameModule::Init()
{
    if (!ApplicationModule::Init())
        return false;    

    m_Scene = SceneLoader::Load("scene.cfg", "world1");
    if (Scene *scene = m_Scene.GetObject())
    {
        m_Player1 = scene->Spawn("Player", "Player1");
        m_Player2 = scene->Spawn("Player", "Player2");

        if (GameObject *player = m_Player2.GetObject())
        {
            float2 position = player->GetPosition();
            position.x = 1000.0f;
            player->SetPosition(position);
        }
    }

    return true;
}

void ActionGameModule::Shutdown()
{
    m_Player1.Release();
    m_Player2.Release();
    m_Scene.Release();

    ApplicationModule::Shutdown();
}

void ActionGameModule::Update()
{
    const float moveSpeed = 4.0f;

    if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (GameObject *player = m_Player1.GetObject())
        {
            float2 position = player->GetPosition();
            position.x = Max<float>(30.0f, position.x - moveSpeed);
            player->SetPosition(position);
        }
    }

    if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (GameObject *player = m_Player1.GetObject())
        {
            float2 position = player->GetPosition();
            position.x = Min<float>(1000.0f, position.x + moveSpeed);
            player->SetPosition(position);
        }
    }

    if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (GameObject *player = m_Player2.GetObject())
        {
            float2 position = player->GetPosition();
            position.x = Max<float>(30.0f, position.x - moveSpeed);
            player->SetPosition(position);
        }
    }

    if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (GameObject *player = m_Player2.GetObject())
        {
            float2 position = player->GetPosition();
            position.x = Min<float>(1000.0f, position.x + moveSpeed);
            player->SetPosition(position);
        }
    }

    if (Scene *scene = m_Scene.GetObject())
    {
        scene->Update();
    }   
}

void ActionGameModule::Draw()
{
    if (Scene *scene = m_Scene.GetObject())
    {
        scene->AddToRenderList(m_RenderList);
    }  

    m_RenderList.Flush();
}

void ActionGameModule::DrawUI()
{
    DebugDisplay.AddText(vec2(100, 100), "hello world", Color::Brown);

    /*
    DebugDisplay.AddLine(vec2(100, 100), vec2(200, 200), Color::Red);
    DebugDisplay.AddLine(vec2(200, 200), vec2(300, 400), Color::Blue);
   
    vec2 lines[] = 
    {
        vec2(10, 10),
        vec2(20, 30),
        vec2(10, 40),
        vec2(40, 90),
        vec2(20, 100)
    };
    DebugDisplay.AddLineStrip(lines, 5, Color::Green);

    DebugDisplay.AddBox(vec2(400, 400), vec2(500, 500), Color::Magenta);
    */

    /*
    UI.BeginFrame();

    //ImGui::ShowDemoWindow();

    static bool showWindow = true;
    if (showWindow)
    {
        if (!UI.BeginWindow("Test", ImGuiWindowFlags_NoCloseButton))
        {
            showWindow = false;
            UI.EndWindow();
        }
        else
        {
            UI.Text("Hello World!");
            UI.EndWindow();
        }
    }

    UI.EndFrame();
    */
}

void ActionGameModule::OnResize(int width, int height)
{
}

void ActionGameModule::OnKeyPress(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            Application.RequestExit(0);
            break;

        case GLFW_KEY_P:
            if (Application.IsPaused())
            {
                Application.Resume();
            }
            else
            {
                Application.Pause();
            }
            break;
            
        case GLFW_KEY_L:
            if (!m_Scene.HasValidObjectID())
            {
                m_Scene = SceneLoader::Load("scene.cfg", "world1");
                if (Scene *scene = m_Scene.GetObject())
                {
                    m_Player1 = scene->Spawn("Player", "Player1");
                    m_Player2 = scene->Spawn("Player", "Player2");

                    if (GameObject *player = m_Player2.GetObject())
                    {
                        float2 position = player->GetPosition();
                        position.x = 1000.0f;
                        player->SetPosition(position);
                    }
                }
            }
            break;

        case GLFW_KEY_U:
            if (m_Scene.HasValidObjectID())
            {
                m_Scene.Release();
                m_Player1.Release();
                m_Player2.Release();
            }
            break;
        }
    }
}
