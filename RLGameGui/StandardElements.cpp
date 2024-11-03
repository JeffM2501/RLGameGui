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

#include "StandardElements.h"
#include "raylib.h"
#include "raymath.h"
#include "GUITextureManager.h"

using namespace rapidjson;

namespace RLGameGUI
{
    Texture2D TextureRecord::GetTexture()
    {
        if (Texture.id != 0)
            return Texture;

        Texture = TextureManager::GetTexture(Name);
        return Texture;
    }

    Font FontRecord::GetFont()
    {
        if (IsFontValid(CachedFont))
            return CachedFont;

        CachedFont = FontManager::GetFont(Name, Size);
        return CachedFont;
    }

    void GUIPanel::Draw(Color fill, Color outline, const Vector2& offset, const Vector2& scale)
    {
        Rectangle rect = GetScreenRect();
        rect.x += offset.x;
        rect.y += offset.y;
        rect.width += scale.x;
        rect.height += scale.y;

        if (fill.a != 0)
        {
            DrawRectangleRec(rect, fill);
        }
        if (outline.a != 0 && OutlineThickness > 0)
        {
			rect.height += 1;
			DrawRectangleLinesEx(rect, float(OutlineThickness), outline);
        }
    }

    void DrawTextureTiled(const Texture2D& fill, const Rectangle& source, const Rectangle& rect, Color& tint)
    {
        BeginScissorMode(int(rect.x), int(rect.y), int(rect.width), int(rect.height));

        int hCount = int((source.width / rect.width) + 0.5f);
        int vCount = int((source.height / rect.height) + 0.5f);

        for (int v = 0; v < vCount; v++)
        {
            for (int h = 0; h < hCount; h++)
            {
                DrawTexturePro(fill, source, Rectangle{ h * source.width,v * source.height, source.width,source.height }, Vector2Zeros, 0, tint);
            }
        }

        EndScissorMode();
    }

    void GUIPanel::Draw(const Texture2D &fill, Color tint, const Rectangle& source, const Vector2& offset, const Vector2& scale)
    {
        Rectangle rect = GetScreenRect();

        rect.x += offset.x;
        rect.y += offset.y;
        rect.width += scale.x;
        rect.height += scale.y;

        if (SourceRect.width == 0)
            SourceRect = Rectangle{ 0,0,(float)fill.width,(float)fill.height };

        if (Fillmode == PanelFillModes::Tile)
        {
            DrawTextureTiled(fill, source, rect, tint);
        }
        else if (Fillmode == PanelFillModes::Fill)
        {
            DrawTexturePro(fill, source, rect, Vector2Zeros, 0, tint);
        }
        else if (Fillmode == PanelFillModes::NPatch)
        {
            if (NPatchData.source.width == 0)
            {
                NPatchData.left = NPatchData.right = (int)NPatchGutters.x;
                NPatchData.top = NPatchData.bottom = (int)NPatchGutters.y;

                if (NPatchGutters.x == 0)
                    NPatchData.layout = NPATCH_THREE_PATCH_HORIZONTAL;
                else if (NPatchGutters.y == 0)
                    NPatchData.layout = NPATCH_THREE_PATCH_VERTICAL;
                else
                    NPatchData.layout = NPATCH_NINE_PATCH;
            } 
            NPatchData.source = source;

            DrawTextureNPatch(fill, NPatchData, rect, Vector2Zeros, 0, tint);
        }
    }

    bool GUIPanel::Read(const Value& object, Document& document)
    {
        GUIElement::Read(object, document);

        GUIScreenReader::ReadColor(object, "tint", Tint);
        GUIScreenReader::ReadColor(object, "outline", Outline);

        GUIScreenReader::ReadMember(object, "outline_thickness", OutlineThickness);

        GUIScreenReader::ReadMember(object, "background", Background.Name);
        GUIScreenReader::ReadRectangle(object, "source_rect", SourceRect);

        int fill = 0;
        if (GUIScreenReader::ReadMember(object, "fill_mode", fill) && fill <= int(PanelFillModes::NPatch))
        {
            Fillmode = PanelFillModes(fill);
        }

        GUIScreenReader::ReadVector2(object, "npatch_gutters", NPatchGutters);

        return true;
    }

    bool GUIPanel::Write(Value& object, Document& document)
    {
        GUIElement::Write(object, document);

        auto& alloc = document.GetAllocator();

        GUIScreenWriter::WriteColor(object, "tint", Tint, document);
        GUIScreenWriter::WriteColor(object, "outline", Outline, document);

        object.AddMember("outline_thickness", OutlineThickness, alloc);

        if (Background.Valid())
            object.AddMember("background", Value(Background.Name.c_str(), alloc), alloc);
        
        GUIScreenWriter::WriteRectangle(object, "source_rect", SourceRect, document);

        object.AddMember("fill_mode", int(Fillmode), alloc);

        GUIScreenWriter::WriteVector2(object, "npatch_gutters", NPatchGutters, document);

        return true;
    }

	void GUIPanel::OnRender()
	{
        if (!Background.Valid())
            Draw(Tint, Outline, Vector2Zeros, Vector2Zeros);
        else
            Draw(Background.GetTexture(), Tint, SourceRect, Vector2Zeros, Vector2Zeros);
	}

    void GUIImage::OnRender()
    {
		if (!Background.Valid())
			DrawRectangleRec(GetScreenRect(), Tint);
		else
			DrawTexturePro(Background.GetTexture(), RealSourceRect, RealDestRect, Vector2Zeros, 0, Tint);
    }

    void GUIImage::OnUpdate()
    {
       
    }

    void GUIImage::OnPreResize()
    {
        if (RelativeBounds.Size.IsZero())
        {
            RelativeBounds.Size = RelativePoint(Background.GetTexture().width, Background.GetTexture().height);
        }

        if (SourceRect.width == 0)
        {
            SourceRect.width = (float)Background.GetTexture().width;
            SourceRect.height = (float)Background.GetTexture().height;
        }
    }

    void GUIImage::OnResize()
    {
        if (!Background.Valid())
            return;

        RealSourceRect = SourceRect;
        RealDestRect = GetScreenRect();

        if (Clip)
        {
            if (RealSourceRect.width < RealDestRect.width)
                RealDestRect.width = RealSourceRect.width;
            else if (RealSourceRect.width > RealDestRect.width)
                RealSourceRect.width = RealDestRect.width;

            if (RealSourceRect.height < RealDestRect.height)
                RealDestRect.height = RealDestRect.height;
            else if (RealSourceRect.height > RealDestRect.height)
                RealSourceRect.height = RealDestRect.height;
        }
    }

    bool GUIImage::Read(const Value& object, Document& document)
    {
        GUIElement::Read(object, document);

        GUIScreenReader::ReadColor(object, "tint", Tint);
        GUIScreenReader::ReadMember(object, "background", Background.Name);
        GUIScreenReader::ReadRectangle(object, "source_rect", SourceRect);
        GUIScreenReader::ReadMember(object, "background", Background.Name);
        return true;
    }

    bool GUIImage::Write(Value& object, Document& document)
    {
        GUIElement::Write(object, document);

        auto& alloc = document.GetAllocator();

        GUIScreenWriter::WriteColor(object, "tint", Tint, document);
       
        if (Background.Valid())
            object.AddMember("background", Value(Background.Name.c_str(), alloc), alloc);

        GUIScreenWriter::WriteRectangle(object, "source_rect", SourceRect, document);

        return true;
    }

    static Rectangle ResizeTextBox(int& textSize, const std::string& text, Font& textFont, float& spacing, Rectangle& screenRect, RLGameGUI::AlignmentTypes hAlign, RLGameGUI::AlignmentTypes vAlign)
    {
        int defaultFontSize = 10;   // Default Font chars height in pixel
        if (textSize < defaultFontSize)
            textSize = defaultFontSize;

        spacing = float(textSize) / defaultFontSize;

        Font fontToUse = textFont;
        if (textFont.texture.id == 0)
            fontToUse = GetFontDefault();

        Vector2 size = MeasureTextEx(fontToUse, text.c_str(), float(textSize), spacing);

        Rectangle textRect = { 0,0,0,0 };

        textRect.width = size.x + spacing * 2;
        switch (hAlign)
        {
        case RLGameGUI::AlignmentTypes::Maximum:
            textRect.x = (screenRect.x + screenRect.width) - size.x;
            break;

        case RLGameGUI::AlignmentTypes::Center:
            textRect.x = (screenRect.x + screenRect.width * 0.5f) - size.x * 0.5f;
            break;

        default:
            textRect.x = screenRect.x;
            break;
        }

        textRect.height = size.y;
        switch (vAlign)
        {
        case RLGameGUI::AlignmentTypes::Maximum:
            textRect.y = (screenRect.y + screenRect.height) - size.y;
            break;

        case RLGameGUI::AlignmentTypes::Center:
            textRect.y = (screenRect.y + screenRect.height * 0.5f) - size.y * 0.5f;
            break;

        default:
            textRect.y = screenRect.y;
            break;
        }

        return textRect;
    }

    void GUILabel::OnResize()
    {
        TextRect = ResizeTextBox(TextFont.Size, Text, TextFont.GetFont(), Spacing, ScreenRect, HorizontalAlignment, VerticalAlignment);
    }

    void DrawTextRect(const Font& fontToUse, const std::string& text, const Rectangle& rect, int size, float spacing, Color tint)
    {
        BeginScissorMode(int(rect.x), int(rect.y), int(rect.width), int(rect.height));
        DrawTextEx(fontToUse, text.c_str(), Vector2{ rect.x, rect.y }, float(size), spacing, tint);
        EndScissorMode();
    }

    void GUILabel::OnRender()
    {
        DrawTextRect(TextFont.GetFont(), Text.c_str(), TextRect, TextFont.Size, Spacing, Tint);
    }

    bool GUILabel::Read(const Value& object, Document& document)
    {
        GUIElement::Read(object, document);

        GUIScreenReader::ReadColor(object, "tint", Tint);

        GUIScreenReader::ReadMember(object, "text_font", TextFont.Name);
        GUIScreenReader::ReadMember(object, "text_size", TextFont.Size);

        GUIScreenReader::ReadMember(object, "text", Text);

        GUIScreenReader::ReadAllignmentType(object, "horizontal_allignment", HorizontalAlignment);
        GUIScreenReader::ReadAllignmentType(object, "vertical_allignment", VerticalAlignment);

        return true;
    }

    bool GUILabel::Write(Value& object, Document& document)
    {
        GUIElement::Write(object, document);
        auto& alloc = document.GetAllocator();

        GUIScreenWriter::WriteColor(object, "tint", Tint, document);
        if (!TextFont.Name.empty())
            object.AddMember("text_font", Value(TextFont.Name.c_str(), alloc), alloc);

        object.AddMember("text_size", TextFont.Size, alloc);

        object.AddMember("text", Value(Text.c_str(), alloc), alloc);

        GUIScreenWriter::WriteAllignmentType(object, "horizontal_allignment", HorizontalAlignment, document);
        GUIScreenWriter::WriteAllignmentType(object, "vertical_allignment", VerticalAlignment, document);

        return true;
    }

    void GUIButton::SetButtonFrames(int framesX, int framesY, int backgroundX, int backgroundY, int hoverX, int hoverY, int pressX, int pressY, int disableX, int disableY )
    {
        float xGrid = (float)Background.GetTexture().width / (float)framesX;
        float yGrid = (float)Background.GetTexture().height / (float)framesY;

        if (backgroundX >= 0 && backgroundY >= 0)
        {
            SourceRect.x = backgroundX * xGrid;
            SourceRect.y = backgroundY * yGrid;
            SourceRect.width = xGrid;
            SourceRect.height = yGrid;
        }

        if (hoverX >= 0 && hoverY >= 0)
        {
            HoverSourceRect.x = hoverX * xGrid;
            HoverSourceRect.y = hoverY * yGrid;
            HoverSourceRect.width = xGrid;
            HoverSourceRect.height = yGrid;
        }

        if (pressX >= 0 && pressY >= 0)
        {
            PressSourceRect.x = pressX * xGrid;
            PressSourceRect.y = pressY * yGrid;
            PressSourceRect.width = xGrid;
            PressSourceRect.height = yGrid;
        }

        if (disableX >= 0 && disableY >= 0)
        {
            DisableSourceRect.x = disableX * xGrid;
            DisableSourceRect.y = disableY * yGrid;
            DisableSourceRect.width = xGrid;
            DisableSourceRect.height = yGrid;
        }
    }

    void GUIButton::OnResize()
    {
        TextRect = ResizeTextBox(TextFont.Size, Text, TextFont.GetFont(), Spacing, ScreenRect, AlignmentTypes::Center, AlignmentTypes::Center);
    }

    void GUIButton::OnRender()
    {
        Color color = Tint;
        Color labelColor = TextColor;
        TextureRecord* tx = &Background;
        Rectangle sourceRect = SourceRect;

        Vector2 offset = Vector2Zeros;
        Vector2 scale = Vector2Zeros;

        if (Disabled)
        {
            color = DisableTint;
            labelColor = DisableTextColor;

            if (DisableTexture.Valid())
                tx = &DisableTexture;

            if (DisableSourceRect.width > 0)
                sourceRect = DisableSourceRect;
        }
        else
        {
            if (Clicked)
            {
                offset = PressOffset;
                scale = PressScale;

                if (PressTint.a > 0)
                    color = PressTint;

                if (PressTextColor.a > 0)
                    labelColor = PressTextColor;

                if (PressTexture.Valid())
                    tx = &PressTexture;

                if (PressSourceRect.width > 0)
                    sourceRect = PressSourceRect;
            }
            else if (Hovered)
            {
                offset = HoverOffset;
                scale = HoverScale;

                if (HoverTint.a > 0)
                    color = HoverTint;

                if (HoverTextColor.a > 0)
                    labelColor = HoverTextColor;

                if (HoverTexture.Valid())
                    tx = &HoverTexture;

                if (HoverSourceRect.width > 0)
                    sourceRect = HoverSourceRect;
            }
        }

        if (!tx->Valid())
            Draw(color, Outline, offset, scale);
        else
            Draw(tx->GetTexture(), color, sourceRect, offset, scale);

        if (!Text.empty())
        {
            Rectangle rect = TextRect;
            rect.x += offset.x;
            rect.y += offset.y;

            DrawTextRect(TextFont.GetFont(), Text.c_str(), rect, TextFont.Size, Spacing, labelColor);
        }
    }

    bool GUIButton::Read(const Value& object, Document& document)
    {
        GUIPanel::Read(object, document);

        GUIScreenReader::ReadMember(object, "text", Text);

        GUIScreenReader::ReadColor(object, "text_color", TextColor);

        GUIScreenReader::ReadMember(object, "text_font", TextFont.Name);
        GUIScreenReader::ReadMember(object, "text_size", TextFont.Size);

        GUIScreenReader::ReadColor(object, "hover_text_color", HoverTextColor);
        GUIScreenReader::ReadColor(object, "hover_tint", HoverTint);
        GUIScreenReader::ReadRectangle(object, "hover_source_rect", HoverSourceRect);
        GUIScreenReader::ReadMember(object, "hover_texture", HoverTexture.Name);

        GUIScreenReader::ReadColor(object, "press_text_color", PressTextColor);
        GUIScreenReader::ReadColor(object, "press_tint", PressTint);
        GUIScreenReader::ReadRectangle(object, "press_source_rect", PressSourceRect);
        GUIScreenReader::ReadMember(object, "press_texture", PressTexture.Name);

        GUIScreenReader::ReadColor(object, "disable_text_color", DisableTextColor);
        GUIScreenReader::ReadColor(object, "disable_tint", DisableTint);
        GUIScreenReader::ReadRectangle(object, "disable_source_rect", DisableSourceRect);
        GUIScreenReader::ReadMember(object, "disable_texture", DisableTexture.Name);

        GUIScreenReader::ReadVector2(object, "hover_offset", HoverOffset);
        GUIScreenReader::ReadVector2(object, "hover_scale", HoverScale);
        GUIScreenReader::ReadVector2(object, "press_offset", PressOffset);
        GUIScreenReader::ReadVector2(object, "press_scale", PressScale);

        return true;
    }

    bool GUIButton::Write(Value& object, Document& document)
    {
        GUIPanel::Write(object, document);

        auto& alloc = document.GetAllocator();

        object.AddMember("text", Value(Text.c_str(), alloc), alloc);

        GUIScreenWriter::WriteColor(object, "text_color", TextColor, document);

        if (!TextFont.Name.empty())
            object.AddMember("text_font", Value(TextFont.Name.c_str(), alloc), alloc);
        object.AddMember("text_size", TextFont.Size, alloc);

        GUIScreenWriter::WriteColor(object, "hover_text_color", HoverTextColor, document);
        GUIScreenWriter::WriteColor(object, "hover_tint", HoverTint, document);
        GUIScreenWriter::WriteRectangle(object, "hover_source_rect", HoverSourceRect, document);
        if (HoverTexture.Valid())
            object.AddMember("hover_texture", Value(HoverTexture.Name.c_str(), alloc), alloc);

        GUIScreenWriter::WriteColor(object, "press_text_color", PressTextColor, document);
        GUIScreenWriter::WriteColor(object, "press_tint", PressTint, document);
        GUIScreenWriter::WriteRectangle(object, "press_source_rect", PressSourceRect, document);
        if (HoverTexture.Valid())
            object.AddMember("press_texture", Value(PressTexture.Name.c_str(), alloc), alloc);

        GUIScreenWriter::WriteColor(object, "disable_text_color", DisableTextColor, document);
        GUIScreenWriter::WriteColor(object, "disable_tint", DisableTint, document);
        GUIScreenWriter::WriteRectangle(object, "disable_source_rect", DisableSourceRect, document);
        if (HoverTexture.Valid())
            object.AddMember("disable_texture", Value(DisableTexture.Name.c_str(), alloc), alloc);

        GUIScreenWriter::WriteVector2(object, "hover_offset", HoverOffset, document);
        GUIScreenWriter::WriteVector2(object, "hover_scale", HoverScale, document);
        GUIScreenWriter::WriteVector2(object, "press_offset", PressOffset, document);
        GUIScreenWriter::WriteVector2(object, "press_scale", PressScale, document);

        return true;
    }

    std::vector<std::string>::const_iterator GUIComboBox::Begin()
    {
        return Items.cbegin();
    }

    std::vector<std::string>::const_iterator GUIComboBox::End()
    {
        return Items.cend();
    }

    std::vector<std::string>::const_iterator GUIComboBox::Erase(const std::vector<std::string>::const_iterator itr)
    {
        return Items.erase(itr);
    }

    void GUIComboBox::Add(const std::string& item)
    {
        Items.emplace_back(item);
    }

    void GUIComboBox::Clear()
    {
        Items.clear();
    }

    int GUIComboBox::GetSelectedItemIndex()
    {
        return SelectedItem;
    }

    void GUIComboBox::SetSelectedItemIndex(int item)
    {
        if (item < 0 || item >= int(Items.size()))
            SelectedItem = -1;

        SelectedItem = item;
    }

    const std::string* GUIComboBox::GetItem(int item)
    {
        if (item < 0 || item >= int(Items.size()))
            return nullptr;

        return &(Items[item]);
    }

    bool GUIComboBox::Read(const Value& object, Document& document)
    {
        GUIPanel::Read(object, document);

        GUIScreenReader::ReadColor(object, "text_color", TextColor);

        GUIScreenReader::ReadMember(object, "text_font", TextFont.Name);
        GUIScreenReader::ReadMember(object, "text_size", TextFont.Size);

        auto itemsItr = object.FindMember("item");

        Items.clear();

        if (itemsItr != object.MemberEnd() && itemsItr->value.IsArray())
        {
            for (auto& item : itemsItr->value.GetArray())
            {
                if (item.IsString())
                {
                    Items.push_back(item.GetString());
                }
            }
        }

        return true;
    }

    bool GUIComboBox::Write(Value& object, Document& document)
    {
        GUIPanel::Write(object, document);

        auto& alloc = document.GetAllocator();

        GUIScreenWriter::WriteColor(object, "text_color", TextColor, document);
        if (!TextFont.Name.empty())
            object.AddMember("text_font", Value(TextFont.Name.c_str(), alloc), alloc);
        object.AddMember("text_size", TextFont.Size, alloc);

        Value valueList(kArrayType);

        for (auto& item : Items)
        {
            valueList.PushBack(Value(item.c_str(), alloc), alloc);
        }

        object.AddMember("items", valueList, alloc);

        return true;
    }
}


