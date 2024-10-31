#include "GUIScreenIO.h"
#include <unordered_map>
#include "GUIElement.h"
#include "raylib.h"

#include "GUIScreen.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" // for stringify JSON

using namespace rapidjson;

namespace RLGameGUI
{
    namespace GUIElementFactory
    {
        std::unordered_map<std::string, std::function<std::unique_ptr<GUIElement>()>> Factories;

        void Register(const std::string& typeName, std::function<std::unique_ptr<GUIElement>()> callback)
        {
            Factories.insert_or_assign(typeName, callback);
        }

        std::shared_ptr<GUIElement> Create(const std::string& typeName)
        {
            auto itr = Factories.find(typeName);
            if (itr == Factories.end())
                return std::make_unique<GUIElement>();

            return itr->second();
        }
    }

    namespace GUIScreenReader
    {
        std::shared_ptr<GUIScreen> Read(const std::string& file)
        {
            char* fileData = LoadFileText(file.c_str());
            std::shared_ptr<GUIScreen> screen = ReadJson(fileData);

            UnloadFileText(fileData);
            return screen;
        }

        void ReadElements(GUIElement* container, Value& elementArray, Document& document)
        {
            for (auto& element : elementArray.GetArray())
            {
                if (!element.IsObject())
                {
                    auto name = element.GetObject().FindMember("typename");
                    if (!name->value.IsString())
                        continue;

                    auto elementObject = GUIElementFactory::Create(name->value.GetString());

                    if (elementObject->Read(element, document))
                    {
                        auto children = element.FindMember("children");
                        if (children->value.IsArray())
                        {
                            ReadElements(elementObject.get(), children->value, document);
                        }

                        container->AddChild(std::move(elementObject));
                    }
                }
            }
        }

        std::shared_ptr<GUIScreen> ReadJson(const char* data)
        {

            std::shared_ptr<GUIScreen> screen = std::make_shared<GUIScreen>();

            Document document;
            document.Parse(data);

            if (!document.IsObject())
                return screen;

            auto rootItr = document.FindMember("root");
            if (!rootItr->value.IsObject())
                return screen;

            auto rootType = rootItr->value.FindMember("root_type");
            if (!rootType->value.IsString())
                return screen;

            auto elementArary = rootType->value.FindMember("elements");
            if (elementArary->value.IsArray())
                return screen;


            ReadElements(screen.get(), elementArary->value, document);

            return screen;
        }
    }

    namespace GUIScreenWriter
    {
        void WriteElement(GUIElement& element, Value& arrayValue, Document& document)
        {
            auto& allocator = document.GetAllocator();

            Value elementValue(Type::kObjectType);

            Value name(Type::kStringType);
            name.SetString(element.GetTypeName(), allocator);
            elementValue.AddMember("typename", name, allocator);

            if (element.Write(elementValue, document))
            {
                if (!element.Children.empty())
                {
                    Value childArray(kArrayType);
                    for (auto child : element.Children)
                    {
                        WriteElement(*child.get(), childArray, document);
                    }

                    elementValue.AddMember("children", childArray, allocator);
                }
            }
            arrayValue.PushBack(elementValue, allocator);
        }

        bool Write(const std::string& file, GUIScreen* screen)
        {
            Document document;

            auto& allocator = document.GetAllocator();

            Value rootItem(kObjectType);
            rootItem.AddMember("root_type", "screen", allocator);

            Value rootArray(kArrayType);
            for (auto& child : screen->Children)
            {
                WriteElement(*child, rootArray, document);
            }

            rootItem.AddMember("elements", rootArray, allocator);
            document.SetObject();
            document.AddMember("root", rootItem, allocator);

            StringBuffer sb;
            PrettyWriter<StringBuffer> writer(sb);
            document.Accept(writer);

            SaveFileText(file.c_str(), (char*)sb.GetString());

            return true;
        }
    }
}