#ifndef __STD_TYPE_H_
#define __STD_TYPE_H_

#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0400
#endif

#ifndef WINVER 
#define WINVER 0x0501
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
#define STRICT
#endif

#ifdef FINAL_RELEASE
#define STATS 0
#else
#define STATS 1
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>

#include <gl/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/compatibility.hpp>
using namespace glm;

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <stb_image.h>
#include <stb_truetype.h>

#include <vector>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <queue>

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <mmsystem.h>

#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

#define Array               std::vector
#define Map                 std::map
#define SortedSet           std::set
#define MultiMap            std::multimap
#define List                std::list
#define Stack               std::stack
#define Queue               std::queue
#define InputStream         std::istream
#define OutputStream        std::ostream
#define FileStream          std::fstream
#define MakePair            std::make_pair
#define Iterator            iterator
#define ConstIterator       const_iterator
#define Max                 glm::max
#define Min                 glm::min

typedef unsigned char       ubyte;
typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned long       ulong;
typedef unsigned int        uword;
typedef unsigned int        crc32;
typedef unsigned long       dword;
typedef unsigned long       udword;
typedef unsigned long long  uqword;
typedef unsigned int        ObjectID;

#define ID_InvalidObject    ObjectID(0)

#endif // __STD_TYPE_H_
