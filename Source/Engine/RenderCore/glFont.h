
#ifndef __RC_GL_FONT_H_
#define __RC_GL_FONT_H_

#include "libStandard.h"

typedef int  GLfont;
typedef vec3 GLcolor;

GLfont    glGenFont(const char *filename, GLint pixelHeight, GLuint imageSize);
GLboolean glDeleteFont(GLfont font);

void      glFontColor3(GLcolor color);
void      glFontColor3f(GLfloat red, GLfloat green, GLfloat blue);
void      glFontColor3fv(const GLfloat *v);

void      glFontDrawText(GLfont font, const char *text, GLfloat x, GLfloat y);
void      glFontDrawFontTexture(GLfont font, GLfloat x, GLfloat y);

#endif // __RC_GL_FONT_H_
