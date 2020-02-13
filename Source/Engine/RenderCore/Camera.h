
#ifndef __RC_CAMERA_H_
#define __RC_CAMERA_H_

#include "libStandard.h"

#define RC_CAMERA_WIDTH         1280.0f
#define RC_CAMERA_HEIGHT        720.0f
#define RC_CAMERA_ASPECT_RATIO  (RC_CAMERA_WIDTH / RC_CAMERA_HEIGHT)

class Camera
{
public:
    Camera();
    ~Camera();

    void Apply(GLFWwindow *window);
    void Clear();

    inline int GetWindowWidth()             { return m_WindowWidth; }
    inline int GetWindowHeight()            { return m_WindowHeight; }

    inline int GetViewportWidth()           { return m_ViewportWidth; }
    inline int GetViewportHeight()          { return m_ViewportHeight; }
    inline int GetViewportAspectRatioX()    { return m_ViewportAspectRatioX; }
    inline int GetViewportAspectRatioY()    { return m_ViewportAspectRatioY; }

    inline float GetProjectionLeft()        { return m_ProjectionLeft; }
    inline float GetProjectionRight()       { return m_ProjectionRight; }
    inline float GetProjectionBottom()      { return m_ProjectionBottom; }
    inline float GetProjectionTop()         { return m_ProjectionTop; }
    inline float GetProjectionNear()        { return m_ProjectionNear; }
    inline float GetProjectionFar()         { return m_ProjectionFar; }

    inline float GetProjectionWidth()       { return RC_CAMERA_WIDTH; }
    inline float GetProjectionHeight()      { return RC_CAMERA_HEIGHT; }
    inline float GetProjectionAspectRatio() { return RC_CAMERA_ASPECT_RATIO; }

private:
    void CalculateProjection();

    int   m_WindowWidth;
    int   m_WindowHeight;

    int   m_ViewportWidth;
    int   m_ViewportHeight;
    float m_ViewportAspectRatioX;
    float m_ViewportAspectRatioY;

    float m_ProjectionLeft;
    float m_ProjectionRight;
    float m_ProjectionBottom;
    float m_ProjectionTop;
    float m_ProjectionNear;
    float m_ProjectionFar;

};

#endif // __RC_CAMERA_H_
