#include "GUITextureManager.h"

#include <unordered_map>

namespace RLGameGUI
{
    namespace TextureManager
    {
        std::string ResourceDir;

        std::unordered_map<std::string, Texture2D> TextureCache;

        Texture2D LoadTexture(const std::string& name)
        {
            std::string filePath = ResourceDir + "/" + name;

            return ::LoadTexture(filePath.c_str());
        }

        void SetResourceDir(const std::string& folderPath)
        {
            ResourceDir = folderPath;
        }

        Texture2D GetTexture(const std::string& name)
        {
            auto itr = TextureCache.find(name);
            if (itr == TextureCache.end())
                itr = TextureCache.insert_or_assign(name, LoadTexture(name)).first;

            return itr->second;
        }
    }
}