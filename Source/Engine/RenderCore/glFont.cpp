
#include "glFont.h"
#include "libRenderCore.h"

struct glFont
{
    glFont() { constructor(glFont); }
    ~glFont() { destructor(glFont); }

    int m_ImageDataSize;
    GLuint m_OpenGLTexture;
    stbtt_bakedchar m_CharacterData[96];
};

struct glFontGlobal
{
    glFontGlobal() : m_Color(float3(1.0f)) {}
    float3 m_Color;
};

typedef Map<GLfont, glFont *> glFonts;

static glFonts      g_Fonts;
static glFontGlobal g_FontGlobal;

String glGetSystemFontFile(const String &faceName)
{
    static const LPCSTR fontRegistryPath = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

    HKEY hKey;
    LONG result;
    String wsFaceName(faceName.begin(), faceName.end());

    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS)
    {
        return "";
    }

    DWORD maxValueNameSize, maxValueDataSize;
    result = RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0);
    if (result != ERROR_SUCCESS)
    {
        return "";
    }

    DWORD valueIndex = 0;
    LPSTR valueName = new CHAR[maxValueNameSize];
    LPBYTE valueData = new BYTE[maxValueDataSize];
    DWORD valueNameSize, valueDataSize, valueType;
    String wsFontFile;

    do
    {
        wsFontFile.clear();
        valueDataSize = maxValueDataSize;
        valueNameSize = maxValueNameSize;

        result = RegEnumValue(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);

        valueIndex++;

        if (result != ERROR_SUCCESS || valueType != REG_SZ)
        {
            continue;
        }

        String wsValueName(valueName, valueNameSize);

        if (_strnicmp(wsFaceName.c_str(), wsValueName.c_str(), wsFaceName.length()) == 0)
        {
            wsFontFile.assign((LPCSTR)valueData, valueDataSize);
            break;
        }
    }
    while (result != ERROR_NO_MORE_ITEMS);

    delete[] valueName;
    delete[] valueData;

    RegCloseKey(hKey);

    if (wsFontFile.empty())
    {
        return "";
    }

    CHAR winDir[MAX_PATH];
    GetWindowsDirectory(winDir, MAX_PATH);

    StringStream ss;
    ss << winDir << "\\Fonts\\" << wsFontFile;
    wsFontFile = ss.str();

    return String(wsFontFile.begin(), wsFontFile.end());
}

glFont *glGetFont(GLfont font)
{
    auto found = g_Fonts.find(font);
    if (found == g_Fonts.end())
    {
        return nullptr;
    }

    return found->second;
}

GLfont glGenFont(const char *filename, GLint pixelHeight, GLuint imageSize)
{
    String fileToLoad(filename);

    if (!EndsWith(fileToLoad, ".ttf"))
        fileToLoad.append(".ttf");

    bool found = false;

    if (FileDatabase *fontFiles = FileSystem("Fonts"))
    {
        FileHandle fh = fontFiles->FindFileHandle(fileToLoad);
        if (fh.IsValid() && fh.Exists())
        {
            fileToLoad = fh.GetAbsoluteFilename();
            found = true;
        }
    }
    if (!found)
    {
        String s = glGetSystemFontFile(filename);
        if (!s.empty())
        {
            fileToLoad = s;
        }
    }

    glFont *font = new glFont;
    font->m_OpenGLTexture = GL_NONE;
    font->m_ImageDataSize = imageSize;

    ubyte *imageData = (ubyte *) malloc(sizeof(ubyte) * imageSize * imageSize);
    ubyte *ttfData   = (ubyte *) malloc(sizeof(ubyte) * (1<<20));

    bool success = true;
    if (FILE *file = fopen(fileToLoad.c_str(), "rb"))
    {
        fread(ttfData, 1, (1<<20), file);
        int count = stbtt_BakeFontBitmap(ttfData, 0, pixelHeight, imageData, imageSize, imageSize, 32, 96, font->m_CharacterData);
        if (count <= 0)
        {
            success = false;
            if (count)
            {
                LOG_ERROR << "Failed to fit " << 96 + count << " font characters in the font file: " << fileToLoad << ENDLOG;
            }
            else
            {
                LOG_ERROR << "Failed to fit any font characters in the font file: " << fileToLoad << ENDLOG;
            }
        }
    }
    else
    {
        success = false;
        LOG_ERROR << "Failed to open the font file: " << fileToLoad << ENDLOG;
    }
    
    glGenTextures(1, &font->m_OpenGLTexture);
    glBindTexture(GL_TEXTURE_2D, font->m_OpenGLTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, imageSize, imageSize, 0, GL_ALPHA, GL_UNSIGNED_BYTE, imageData);

    if (font->m_OpenGLTexture == GL_NONE)
    {
        LOG_ERROR << "Failed to generate the font texture for font file: " << fileToLoad << ENDLOG;
    }
    else
    {
        g_Fonts[font->m_OpenGLTexture] = font;
    }

    free(imageData);
    free(ttfData);

    return (GLfont) font->m_OpenGLTexture;
}

GLboolean glDeleteFont(GLfont font)
{
    if (font != GL_NONE)
    {
        if (glFont *f = glGetFont(font))
        {
            if (f->m_OpenGLTexture != GL_NONE)
            {
                glDeleteTextures(1, &f->m_OpenGLTexture);
            }

            g_Fonts.erase(font);
            delete f;
            return GL_TRUE;
        }
    }
    return GL_FALSE;
}

void glFontColor3(GLcolor color)
{
    glFontColor3f(color[0], color[1], color[2]);
}

void glFontColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
    g_FontGlobal.m_Color = float3(red, green, blue);
}

void glFontColor3fv(const GLfloat *v)
{
    glFontColor3f(v[0], v[1], v[2]);
}

void glFontDrawText(GLfont font, const char *text, GLfloat x, GLfloat y)
{
    if (glFont *f = glGetFont(font))
    {    
        glBindTexture(GL_TEXTURE_2D, f->m_OpenGLTexture);
        glEnable(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float xPos = x;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_QUADS);
        while (*text)
        {
            if (*text >= 32 && *text < 128)
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(f->m_CharacterData, f->m_ImageDataSize, f->m_ImageDataSize, *text - 32, &x, &y, &q, 1);

                float w = q.x1 - q.x0;
                float h = q.y1 - q.y0;

                float yOffset = y - q.y1;

                float x0 = xPos;
                float x1 = xPos + w;
                float y0 = y + yOffset;
                float y1 = y + yOffset + h;
            
                xPos += f->m_CharacterData[*text - 32].xadvance;

                glColor3fv(&g_FontGlobal.m_Color[0]); glTexCoord2f(q.s0, q.t1); glVertex2f(round(x0), round(y0));
                glColor3fv(&g_FontGlobal.m_Color[0]); glTexCoord2f(q.s1, q.t1); glVertex2f(round(x1), round(y0));
                glColor3fv(&g_FontGlobal.m_Color[0]); glTexCoord2f(q.s1, q.t0); glVertex2f(round(x1), round(y1));
                glColor3fv(&g_FontGlobal.m_Color[0]); glTexCoord2f(q.s0, q.t0); glVertex2f(round(x0), round(y1));
            }
            ++text;
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }
}

void glFontDrawFontTexture(GLfont font, GLfloat x, GLfloat y)
{
    if (glFont *f = glGetFont(font))
    {
        glBindTexture(GL_TEXTURE_2D, f->m_OpenGLTexture);
        glEnable(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float u1 = 0;
        float u2 = 1;
        float v1 = 0;
        float v2 = 1;
        
        float left   = x;
        float right  = left + f->m_ImageDataSize;
        float bottom = y;
        float top    = bottom + f->m_ImageDataSize;
        
        glBegin(GL_QUADS);
            glTexCoord2f(u1, v2); glColor3f(1.f, 1.f, 1.f); glVertex3f(left,  bottom, 0.0f);
            glTexCoord2f(u2, v2); glColor3f(1.f, 1.f, 1.f); glVertex3f(right, bottom, 0.0f);
            glTexCoord2f(u2, v1); glColor3f(1.f, 1.f, 1.f); glVertex3f(right, top,    0.0f);        
            glTexCoord2f(u1, v1); glColor3f(1.f, 1.f, 1.f); glVertex3f(left,  top,    0.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }
}

