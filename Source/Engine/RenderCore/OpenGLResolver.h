
#ifndef __RC_OPENGL_RESOLVER_H_
#define __RC_OPENGL_RESOLVER_H_

#include "libStandard.h"

class _OpenGLResolver
{
public:
    _OpenGLResolver();
    ~_OpenGLResolver();

    bool ResolveToken(const String &token, uint &outValue);

private:
    void LoadTable();

    Map<String, uint> m_Data;

};

#endif // __RC_OPENGL_RESOLVER_H_
