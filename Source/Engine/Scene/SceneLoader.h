
#ifndef __SCE_SCENELOADER_H_
#define __SCE_SCENELOADER_H_

#include "libStandard.h"
#include "libObjectModel.h"
#include "Scene.h"

class SceneLoader
{
public:
    static Reference<Scene> Load(const String &sceneFilename, const String &sceneName);

};

#endif // __SCE_SCENELOADER_H_
