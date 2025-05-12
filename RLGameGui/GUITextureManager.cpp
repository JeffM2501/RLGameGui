#include "GUITextureManager.h"

#include "rlText.h"

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

        void UnloadAll()
        {
            for (auto& [key, texture] : TextureCache)
                UnloadTexture(texture);

            TextureCache.clear();
        }
    }

    namespace FontManager
    {
        struct FontRecord
        {
            std::string FontName;
            std::unordered_map<float, rltFont> Fonts;
        };

        std::unordered_map<std::string, FontRecord> FontCache;

        rltFont LoadFont(const std::string name, float size)
        {
            std::string filePath = TextureManager::ResourceDir + "/" + name;

            return rltLoadFontTTF(filePath, size);
        }

        rltFont GetFont(const std::string& name, float size)
        {
            if (name.empty())
                return rltGetDefaultFont();

            auto fontItr = FontCache.find(name);
            if (fontItr == FontCache.end())
                fontItr = FontCache.insert_or_assign(name, FontRecord{ name }).first;

            auto& fonts = fontItr->second.Fonts;

            auto sizeItr = fonts.find(size);
            if (sizeItr == fonts.end())
                sizeItr = fonts.insert_or_assign(size, LoadFont(name, size)).first;

            return sizeItr->second;
        }

        void UnloadAll()
        {
            for (auto& [key, fontGroup] : FontCache)
            {
                for (auto& [size, font] : fontGroup.Fonts)
                {
                    rltUnloadFont(&font);
                }
            }
            FontCache.clear();
        }
    }
}