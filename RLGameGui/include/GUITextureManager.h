#pragma once

#include <string>

#include "raylib.h"

namespace RLGameGUI
{
    namespace TextureManager
    {
        void SetResourceDir(const std::string& folderPath);

        Texture2D GetTexture(const std::string& name);
    }
}