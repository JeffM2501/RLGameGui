/**********************************************************************************************
*
*   RLGameGUi * A game gui for raylib
*
*   LICENSE: MIT
*
*   Copyright (c) 2024 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include <string>
#include <functional>
#include <memory>

#include "GUIElement.h"
#include "rapidjson/document.h"
#include "raylib.h"

namespace RLGameGUI
{
    class GUIElement;
    class GUIScreen;

    namespace GUIElementFactory
    {
        void Register(const std::string& typeName, std::function<std::shared_ptr<GUIElement>()> callback);
        std::shared_ptr<GUIElement> Create(const std::string& typeName);
    }

    namespace GUIScreenReader
    {
        std::shared_ptr<GUIScreen> Read(const std::string& file);
        std::shared_ptr<GUIScreen> ReadJson(const char* data);

        bool ReadColor(const rapidjson::Value& object, const std::string& name, Color& color);
        bool ReadRectangle(const rapidjson::Value& object, const std::string& name, Rectangle& rect);
        bool ReadVector2(const rapidjson::Value& object, const std::string& name, Vector2& vector);

        bool ReadAllignmentType(const rapidjson::Value& object, const std::string& name, AlignmentTypes& alligment);

        template<class T>
        inline bool ReadMember(const rapidjson::Value& object, const std::string& name, T& value)
        {
            auto valueObjet = object.FindMember(name.c_str());
            if (valueObjet != object.MemberEnd() && valueObjet->value.Is<T>())
            {
                value = valueObjet->value.Get<T>();
                return true;
            }
            return false;
        }

        template<>
        inline bool ReadMember(const rapidjson::Value& object, const std::string& name, unsigned char& value)
        {
            auto valueObjet = object.FindMember(name.c_str());
            if (valueObjet != object.MemberEnd() && valueObjet->value.IsInt())
            {
                value = unsigned char(valueObjet->value.GetInt());
                return true;
            }
            return false;
        }

        template<>
        inline bool ReadMember(const rapidjson::Value& object, const std::string& name, std::string& value)
        {
            auto valueObjet = object.FindMember(name.c_str());
            if (valueObjet != object.MemberEnd() && valueObjet->value.IsString())
            {
                value = valueObjet->value.GetString();
                return true;
            }
            return false;
        }
    }

    namespace GUIScreenWriter
    {
        bool Write(const std::string& file, GUIScreen* screen);

        bool WriteColor(rapidjson::Value& object, const std::string& name, Color color, rapidjson::Document& document);
        bool WriteRectangle(rapidjson::Value& object, const std::string& name, Rectangle rect, rapidjson::Document& document);
        bool WriteVector2(rapidjson::Value& object, const std::string& name, Vector2 vector, rapidjson::Document& document);

        bool WriteAllignmentType(rapidjson::Value& object, const std::string& name, AlignmentTypes alligment, rapidjson::Document& documnet);
    }
}