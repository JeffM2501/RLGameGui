#pragma once

#include <string>

#include "raylib.h"

namespace RLGameGUI
{
    namespace TextureManager
    {
        void SetResourceDir(const std::string& folderPath);

        Texture2D GetTexture(const std::string& name);

        void UnloadAll();
    }

    namespace FontManager
    {
        Font GetFont(const std::string& name, int size);

        void UnloadAll();
    }
}