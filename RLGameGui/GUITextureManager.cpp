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
            std::unordered_map<int, Font> Fonts;
        };

        std::unordered_map<std::string, FontRecord> FontCache;

        Font LoadFont(const std::string name, int size)
        {
            std::string filePath = TextureManager::ResourceDir + "/" + name;

            return ::LoadFontEx(filePath.c_str(), size, nullptr, 0);
        }

        Font GetFont(const std::string& name, int size)
        {
            if (name.empty())
                return GetFontDefault();

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
                    UnloadFont(font);
                }
            }
            FontCache.clear();
        }
    }
}