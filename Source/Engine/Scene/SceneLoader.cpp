
#include "SceneLoader.h"
#include "ContentDatabase.h"
#include "ApplicationCentral.h"

Reference<Scene> SceneLoader::Load(const String &sceneFilename, const String &sceneName)
{
    Reference<Scene> scene;

    if (ContentDatabase<Scene> *sceneDatabase = Application.GetContentDatabase<Scene>())
    {
        if (sceneDatabase->Load(sceneFilename))
        {
            scene = sceneDatabase->GetOrCreateObject(sceneName);
        }

        sceneDatabase->Clear();
    }

    if (!scene.HasValidObjectID())
    {
        LOG_WARNING << "Failed to load the scene: " << sceneName << " from the the file: " << sceneFilename << ENDLOG;
    }

    return scene;
}
