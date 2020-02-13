
#ifndef __ANIM_ANIMATION_H_
#define __ANIM_ANIMATION_H_

#include "libStandard.h"
#include "libConfig.h"
#include "libObjectModel.h"
#include "libRenderCore.h"

class Animation : public Visual
{
    declare_typeinfo(Visual);

public:
    Animation();
    virtual ~Animation();

    virtual void Serialize(const Serializer &serializer);
    virtual void Initialize();
    virtual void Destroy();

    virtual void Start();
    virtual void Pause();
    virtual void Resume();
    virtual void Reset();

    int GetFrameRate() const { return m_FrameRate; }
    void SetFrameRate(int frameRate) { m_FrameRate = frameRate; }

    bool IsLooping() const { return m_IsLooping; }
    void SetLooping(bool isLooping) { m_IsLooping = isLooping; }

    bool IsRunning() const { return m_IsRunning; }

    virtual int GetLayer() const;
    virtual void Draw(const mat4 &matrix) const;
    virtual void AddToRenderList(RenderList &renderList) const;

protected:
    int UpdateCurrentFrame() const;

private:
    int  m_FrameRate;
    bool m_IsLooping;

    Array< Reference<Sprite> > m_Sprites;

    bool          m_IsRunning;
    mutable float m_LastUpdateTime;
    mutable int   m_CurrentFrame;

};

#endif // __ANIM_ANIMATION_H_
