
#include "Animation.h"

implement_typeinfo(Animation);

Animation::Animation()
 :  Parent(),
    m_FrameRate(30),
    m_IsLooping(true),
    m_IsRunning(false),
    m_LastUpdateTime(0.0f),
    m_CurrentFrame(0)
{
    constructor(Animation);
}

Animation::~Animation()
{
    destructor(Animation);
}

void Animation::Serialize(const Serializer &serializer)
{
    Parent::Serialize(serializer);

    m_FrameRate << serializer("Looping");
    m_FrameRate << serializer("FrameRate");

    int frame = 0;
    while (frame >= 0)
    {
        String key = PrintF("Frame[%d]", frame++);
        Reference<Sprite> sprite = GetOrCreateObjectFromContentDatabase<Sprite>(serializer, key, false);
        if (sprite.IsValid())
        {
            m_Sprites.push_back(sprite);
        }
        else
        {
            frame = -1;
        }
    }
}

void Animation::Initialize()
{
    Parent::Initialize();
}

void Animation::Destroy()
{
    Parent::Destroy();
}

int Animation::GetLayer() const
{
    // TODO: some validation, for not just use the layer of the first sprite
    if (!m_Sprites.empty())
    {
        if (Sprite *sprite = m_Sprites[0].GetObject())
            return sprite->GetLayer();
    }    

    return ID_InvalidLayer;
}

void Animation::Draw(const mat4 &matrix) const
{
    if (m_Sprites.empty())
        return;

    int frame = UpdateCurrentFrame();
    const Reference<Sprite> &sprite = m_Sprites[frame];

    if (Sprite *s = sprite.GetObject())
    {
        s->Draw(matrix);
    }
}

void Animation::AddToRenderList(RenderList &renderList) const
{
    if (m_Sprites.empty())
        return;

    int frame = UpdateCurrentFrame();
    const Reference<Sprite> &sprite = m_Sprites[frame];

    if (Sprite *s = sprite.GetObject())
    {
        s->AddToRenderList(renderList);
    }
}

void Animation::Start()
{
    m_IsRunning = true;
    m_LastUpdateTime = Application.GetRunningTime();
}

void Animation::Pause()
{
    m_IsRunning = false;
}

void Animation::Resume()
{
    m_IsRunning = true;
    m_LastUpdateTime = Application.GetRunningTime();
}

void Animation::Reset()
{
    m_IsRunning = false;
    m_LastUpdateTime = Application.GetRunningTime();
    m_CurrentFrame = 0;
}

int Animation::UpdateCurrentFrame() const
{
    if (m_IsRunning)
    {
        float frameTime = 1.0f / static_cast<float>(m_FrameRate);
        float currentTime = Application.GetRunningTime();
        int numFrames = m_Sprites.size();

        if (numFrames == 0)
            return 0;

        while (currentTime > m_LastUpdateTime + frameTime)
        {
            if (m_IsLooping)
            {
                m_CurrentFrame = (m_CurrentFrame + 1) % numFrames;
            }
            else
            {
                m_CurrentFrame = Min(m_CurrentFrame + 1, numFrames - 1);
            }

            m_LastUpdateTime += frameTime;
        }
    }

    return m_CurrentFrame;
}
