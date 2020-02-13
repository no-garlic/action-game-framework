
#ifndef __GAME_MODULE_H_
#define __GAME_MODULE_H_

#include "libEngine.h"

class ActionGameModule : public ApplicationModule
{
public:
    ActionGameModule(const String &name);
    virtual ~ActionGameModule();

    virtual bool Init();
    virtual void Shutdown();
    virtual void Update();
    virtual void Draw();
    virtual void DrawUI();

    virtual void OnResize(int width, int height);
    virtual void OnKeyPress(int key, int scancode, int action, int mods);

private:
    RenderList            m_RenderList;
    Reference<Scene>      m_Scene;
    Reference<GameObject> m_Player1;
    Reference<GameObject> m_Player2;

};

#endif // __GAME_MODULE_H_

