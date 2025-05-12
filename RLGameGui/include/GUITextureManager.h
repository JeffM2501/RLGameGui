#pragma once

#include <string>
#include "raylib.h"
#include "rlText.h"

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
        rltFont GetFont(const std::string& name, float size);

        void UnloadAll();
    }
}