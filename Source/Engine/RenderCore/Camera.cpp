
#include "Camera.h"
#include "ApplicationCentral.h"
#include "ApplicationModule.h"

Camera::Camera()
{
    constructor(Camera);

    m_WindowWidth          = RC_CAMERA_WIDTH;
    m_WindowHeight         = RC_CAMERA_HEIGHT;
    m_ViewportWidth        = RC_CAMERA_WIDTH;
    m_ViewportHeight       = RC_CAMERA_HEIGHT;
    m_ViewportAspectRatioX = RC_CAMERA_ASPECT_RATIO;
    m_ViewportAspectRatioY = 1.0f / m_ViewportAspectRatioX;

    CalculateProjection();
}

Camera::~Camera()
{
    destructor(Camera);
}

void Camera::CalculateProjection()
{
    m_ProjectionLeft   =  0.0f;
    m_ProjectionRight  =  m_WindowWidth / (m_WindowWidth / RC_CAMERA_WIDTH);
    m_ProjectionBottom =  0.0f;
    m_ProjectionTop    =  m_WindowHeight / (m_WindowHeight / RC_CAMERA_HEIGHT);
    m_ProjectionNear   =  1.0f;
    m_ProjectionFar    = -1.0f;
}

void Camera::Apply(GLFWwindow *window)
{
    glfwGetFramebufferSize(window, &m_WindowWidth, &m_WindowHeight);

    m_ViewportWidth  = m_WindowWidth;
    m_ViewportHeight = m_WindowHeight;
    m_ViewportAspectRatioX = m_ViewportWidth / (float) m_ViewportHeight;
    m_ViewportAspectRatioY = 1.0f / m_ViewportAspectRatioX;
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    CalculateProjection();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_ProjectionLeft, m_ProjectionRight, m_ProjectionBottom, m_ProjectionTop, m_ProjectionNear, m_ProjectionFar);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Camera::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

