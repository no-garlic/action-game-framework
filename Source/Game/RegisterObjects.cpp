
#include "RegisterObjects.h"
#include "libEngine.h"

bool RegisterObjects()
{
    ObjectManager.Register<SpriteComponent>();
    ObjectManager.Register<AnimationComponent>();
    ObjectManager.Register<AIComponent>();
    ObjectManager.Register<Group>();

    return true;
}

bool InitObjects()
{
    if (const PropertySection *properties = Application.GetPropertySection("FilePath"))
    {
        for (auto pair : properties->GetValues())
        {   
            if (!FileSystem.AddDatabase(pair.first, pair.second.AsString()))
            {
                LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
                return false;
            }
        }
    }
    else
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }
    
    if (!Application.Register<Texture>("GlobalConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    if (!Application.Register<Sprite>("GlobalConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    if (!Application.Register<Animation>("GlobalConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }
    
    if (!Application.Register<RenderState>("GlobalConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    if (!Application.Register<Layer>("GlobalConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    if (!Application.Register<GameObject>("WorldConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    if (!Application.Register<Scene>("WorldConfig"))
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    ContentDatabaseBase *textureDatabase     = Application.GetContentDatabase<Texture>();
    ContentDatabaseBase *spriteDatabase      = Application.GetContentDatabase<Sprite>();
    ContentDatabaseBase *animationDatabase   = Application.GetContentDatabase<Animation>();
    ContentDatabaseBase *renderStateDatabase = Application.GetContentDatabase<RenderState>();
    ContentDatabaseBase *layerDatabase       = Application.GetContentDatabase<Layer>();
    ContentDatabaseBase *gameObjectDatabase  = Application.GetContentDatabase<GameObject>();

    if (!textureDatabase || !spriteDatabase || !animationDatabase || !renderStateDatabase || !layerDatabase || !gameObjectDatabase)
    {
        LOG_ERROR << "ApplicationModule::Init() Failed" << ENDLOG;
        return false;
    }

    if (FileDatabase *fileDatabase = FileSystem("GlobalConfig"))
    {
        FileHandleArray files = fileDatabase->ListFiles();
        for (FileHandle file : files)
        {
            const String &filename = file.GetFilename();

            if (!renderStateDatabase->Load(filename))
            {
                LOG_ERROR << "Failed to load the render states from file: " << filename << ENDLOG;
                return false;
            }
        }

        for (FileHandle file : files)
        {
            const String &filename = file.GetFilename();

            if (!layerDatabase->Load(filename))
            {
                LOG_ERROR << "Failed to load the layers from file: " << filename << ENDLOG;
                return false;
            }
        }

        for (FileHandle file : files)
        {
            const String &filename = file.GetFilename();

            if (!textureDatabase->Load(filename))
            {
                LOG_ERROR << "Failed to load the textures from file: " << filename << ENDLOG;
                return false;
            }
        }

        for (FileHandle file : files)
        {
            const String &filename = file.GetFilename();

            if (!spriteDatabase->Load(filename))
            {
                LOG_ERROR << "Failed to load the sprites from file: " << filename << ENDLOG;
                return false;
            }
        }
        
        for (FileHandle file : files)
        {
            const String &filename = file.GetFilename();

            if (!animationDatabase->Load(filename))
            {
                LOG_ERROR << "Failed to load the animations from file: " << filename << ENDLOG;
                return false;
            }
        }
    }
    else
    {
        LOG_ERROR << "Failed to find the file database: \"GlobalConfig\"" << ENDLOG;
        return false;
    }
        
    if (FileDatabase *fileDatabase = FileSystem("WorldConfig"))
    {
        FileHandleArray files = fileDatabase->ListFiles();
        for (FileHandle file : files)
        {
            const String &filename = file.GetFilename();

            if (!gameObjectDatabase->Load(filename))
            {
                LOG_ERROR << "Failed to load the game objects from file: " << filename << ENDLOG;
                return false;
            }
        }
    }
    else
    {
        LOG_ERROR << "Failed to find the file database: \"WorldConfig\"" << ENDLOG;
        return false;
    }

    return true;
}
