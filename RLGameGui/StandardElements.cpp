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

#include "rlText.h"

using namespace rapidjson;

namespace RLGameGUI
{
    void RegisterStandardElements()
    {
        GUIPanel::Register();
        GUILabel::Register();
        GUIImage::Register();
        GUICheckBox::Register();
        GUIButton::Register();
    }

    Texture2D TextureRecord::GetTexture()
    {
        if (Texture.id != 0)
            return Texture;

        Texture = TextureManager::GetTexture(Name);
        return Texture;
    }

    rltFont FontRecord::GetFont()
    {
        if (!CachedFont.Ranges.empty())
            return CachedFont;

        CachedFont = FontManager::GetFont(Name, Size);
        return CachedFont;
    }

    bool GUITexture::Read(const rapidjson::Value& object)
    {
        GUIScreenReader::ReadColor(object, "tint", Tint);
        GUIScreenReader::ReadRectangle(object, "source_rect", SourceRect);
        GUIScreenReader::ReadMember(object, "texture", Texture.Name);
        GUIScreenReader::ReadVector2(object, "offset", Offset);
        GUIScreenReader::ReadVector2(object, "scale", Scale);

        int fill = 0;
        if (GUIScreenReader::ReadMember(object, "fill_mode", fill) && fill <= int(PanelFillModes::NPatch))
        {
            Fillmode = PanelFillModes(fill);
        }

        GUIScreenReader::ReadRectangle(object, "npatch_gutters", NPatchGutters);

        return true;
    }

    bool GUITexture::ReadMember(const rapidjson::Value& object, const std::string& name)
    {
        auto itr = object.FindMember(name.c_str());
        if (itr == object.MemberEnd() || !itr->value.IsObject())
            return false;

        return Read(itr->value);
    }

    bool GUITexture::Write(rapidjson::Value& object, rapidjson::Document& document)
    {
        auto alloc = document.GetAllocator();

        GUIScreenWriter::WriteColor(object, "tint", Tint, document);
        GUIScreenWriter::WriteRectangle(object, "source_rect", SourceRect, document);

        if (Texture.Valid())
            object.AddMember("texture", Value(Texture.Name.c_str(), alloc), alloc);

        GUIScreenWriter::WriteVector2(object, "offset", Offset, document);
        GUIScreenWriter::WriteVector2(object, "scale", Scale, document);

        object.AddMember("fill_mode", int(Fillmode), alloc);

        GUIScreenWriter::WriteRectangle(object, "npatch_gutters", NPatchGutters, document);

        return true;
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

    void GUIPanel::Draw(GUITexture& fill)
    {
        Rectangle rect = GetScreenRect();

        rect.x += fill.Offset.x;
        rect.y += fill.Offset.y;
        rect.width += fill.Scale.x;
        rect.height += fill.Scale.y;

        Texture2D texture = fill.Texture.GetTexture();

        if (fill.SourceRect.width == 0)
            fill.SourceRect = Rectangle{ 0,0,(float)texture.width,(float)texture.height };

        if (fill.Fillmode == PanelFillModes::Tile)
        {
            DrawTextureTiled(texture, fill.SourceRect, rect, fill.Tint);
        }
        else if (fill.Fillmode == PanelFillModes::Fill)
        {
            DrawTexturePro(texture, fill.SourceRect, rect, Vector2Zeros, 0, fill.Tint);
        }
        else if (fill.Fillmode == PanelFillModes::NPatch)
        {
            if (NPatchData.source.width == 0)
            {
                NPatchData.left = NPatchData.right = (int)fill.NPatchGutters.x;
                if (fill.NPatchGutters.width >= 0)
                    NPatchData.right = int(fill.NPatchGutters.width);

                NPatchData.top = NPatchData.bottom = (int)fill.NPatchGutters.y;
                if (fill.NPatchGutters.height >= 0)
                    NPatchData.bottom = int(fill.NPatchGutters.height);

                if (fill.NPatchGutters.x == 0)
                    NPatchData.layout = NPATCH_THREE_PATCH_HORIZONTAL;
                else if (fill.NPatchGutters.y == 0)
                    NPatchData.layout = NPATCH_THREE_PATCH_VERTICAL;
                else
                    NPatchData.layout = NPATCH_NINE_PATCH;
            } 
            NPatchData.source = fill.SourceRect;

            DrawTextureNPatch(texture, NPatchData, rect, Vector2Zeros, 0, fill.Tint);
        }
    }

    bool GUIPanel::Read(const Value& object, Document& document)
    {
        GUIElement::Read(object, document);

        GUIScreenReader::ReadColor(object, "tint", Tint);
        GUIScreenReader::ReadColor(object, "outline", Outline);

        GUIScreenReader::ReadMember(object, "outline_thickness", OutlineThickness);

        Background.ReadMember(object, "background");

        return true;
    }

    bool GUIPanel::Write(Value& object, Document& document)
    {
        GUIElement::Write(object, document);

        auto& alloc = document.GetAllocator();

        GUIScreenWriter::WriteColor(object, "tint", Tint, document);
        GUIScreenWriter::WriteColor(object, "outline", Outline, document);

        object.AddMember("outline_thickness", OutlineThickness, alloc);

        Value background(kObjectType);
        Background.Write(background, document);
        object.AddMember("background", background, alloc);
     
        return true;
    }

	void GUIPanel::OnRender()
	{
        if (!Background.Texture.Valid())
            Draw(Tint, Outline, Vector2Zeros, Vector2Zeros);
        else
            Draw(Background);
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

    static Rectangle ResizeTextBox(float& textSize, const std::string& text, rltFont& textFont, Rectangle& screenRect, RLGameGUI::AlignmentTypes hAlign, RLGameGUI::AlignmentTypes vAlign)
    {
        float defaultFontSize = 10;   // Default Font chars height in pixel
        if (textSize < defaultFontSize)
            textSize = defaultFontSize;

        const rltFont *fontToUse = &textFont;
        if (textFont.Ranges.empty())
            fontToUse = &rltGetDefaultFont();

        Vector2 size = rltMeasureText(text, float(textSize), fontToUse);

        Rectangle textRect = { 0,0,0,0 };

        textRect.width = size.x + fontToUse->DefaultSpacing * 2;
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
        TextRect = ResizeTextBox(TextFont.Size, Text, TextFont.GetFont(), ScreenRect, HorizontalAlignment, VerticalAlignment);
    }

    void DrawTextRect(const rltFont& fontToUse, const std::string& text, const Rectangle& rect, float size, Color tint, bool clip)
    {
        if (clip)
            BeginScissorMode(int(rect.x), int(rect.y), int(rect.width), int(rect.height));
        
        Vector2 bounds = rltMeasureText(text, size, &fontToUse);
        Vector2 center = Vector2{ rect.x + (rect.width / 2.0f), rect.y + (rect.height / 2.0f) };
        Vector2 pos = center - (bounds * 0.5f);
        rltDrawText(text, size, pos, tint, &fontToUse);

        if (clip)
            EndScissorMode();
    }

    void GUILabel::OnRender()
    {
        DrawTextRect(TextFont.GetFont(), Text.c_str(), TextRect, TextFont.Size, Tint, ClipToRectangle);
    }

    bool GUILabel::Read(const Value& object, Document& document)
    {
        GUIElement::Read(object, document);

        GUIScreenReader::ReadColor(object, "tint", Tint);

        GUIScreenReader::ReadMember(object, "text_font", TextFont.Name);
        GUIScreenReader::ReadMember(object, "text_size", TextFont.Size);

        GUIScreenReader::ReadMember(object, "text", Text);

        GUIScreenReader::ReadMember(object, "clip_to_rect", ClipToRectangle);

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

        object.AddMember("clip_to_rect", ClipToRectangle, alloc);

        GUIScreenWriter::WriteAllignmentType(object, "horizontal_allignment", HorizontalAlignment, document);
        GUIScreenWriter::WriteAllignmentType(object, "vertical_allignment", VerticalAlignment, document);

        return true;
    }

    void GUIButton::SetButtonFrames(int framesX, int framesY, int backgroundX, int backgroundY, int hoverX, int hoverY, int pressX, int pressY, int disableX, int disableY )
    {
        float xGrid = (float)Background.Texture.GetTexture().width / (float)framesX;
        float yGrid = (float)Background.Texture.GetTexture().height / (float)framesY;

        if (backgroundX >= 0 && backgroundY >= 0)
        {
            Background.SourceRect.x = backgroundX * xGrid;
            Background.SourceRect.y = backgroundY * yGrid;
            Background.SourceRect.width = xGrid;
            Background.SourceRect.height = yGrid;
        }

        if (hoverX >= 0 && hoverY >= 0)
        {
            Hover.SourceRect.x = hoverX * xGrid;
            Hover.SourceRect.y = hoverY * yGrid;
            Hover.SourceRect.width = xGrid;
            Hover.SourceRect.height = yGrid;
        }

        if (pressX >= 0 && pressY >= 0)
        {
            Press.SourceRect.x = pressX * xGrid;
            Press.SourceRect.y = pressY * yGrid;
            Press.SourceRect.width = xGrid;
            Press.SourceRect.height = yGrid;
        }

        if (disableX >= 0 && disableY >= 0)
        {
            Disable.SourceRect.x = disableX * xGrid;
            Disable.SourceRect.y = disableY * yGrid;
            Disable.SourceRect.width = xGrid;
            Disable.SourceRect.height = yGrid;
        }
    }

    void GUIButton::OnResize()
    {
        TextRect = ResizeTextBox(TextFont.Size, Text, TextFont.GetFont(), ScreenRect, AlignmentTypes::Center, AlignmentTypes::Center);
    }

    void GUIButton::OnRender()
    {
        Color labelColor = TextColor;
        GUITexture* tx = &Background;

        if (Disabled)
        {
            labelColor = DisableTextColor;

            if (Disable.Texture.Valid())
                tx = &Disable;
        }
        else
        {
            if (Clicked)
            {
                if (Press.Texture.Valid() || Press.Tint.a > 0)
                    tx = &Press;

                if (PressTextColor.a > 0)
                    labelColor = PressTextColor;
            }
            else if (Hovered)
            {
                if (Hover.Texture.Valid() || Hover.Tint.a > 0)
                    tx = &Hover;

                if (HoverTextColor.a > 0)
                    labelColor = HoverTextColor;
            }
        }

        if (!tx->Texture.Valid())
            Draw(tx->Tint, Outline, tx->Offset, tx->Scale);
        else
            Draw(*tx);

        if (!Text.empty())
        {
            Rectangle rect = TextRect;
            rect.x += tx->Offset.x;
            rect.y += tx->Offset.y;

            DrawTextRect(TextFont.GetFont(), Text, rect, TextFont.Size, labelColor, true);
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
        Hover.ReadMember(object, "hover");

        GUIScreenReader::ReadColor(object, "press_text_color", PressTextColor);
        Press.ReadMember(object, "press");

        GUIScreenReader::ReadColor(object, "disable_text_color", DisableTextColor);
        Disable.ReadMember(object, "disable");

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

        Value hover(kObjectType);
        Hover.Write(hover, document);
        object.AddMember("hover", hover, alloc);

        GUIScreenWriter::WriteColor(object, "press_text_color", PressTextColor, document);
        Value press(kObjectType);
        Press.Write(hover, document);
        object.AddMember("press", press, alloc);

        Value disable(kObjectType);
        Disable.Write(disable, document);
        object.AddMember("disable", disable, alloc);

        GUIScreenWriter::WriteColor(object, "disable_text_color", DisableTextColor, document);
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
        if (SelectedItem < 0)
        {
            SelectedItem = 0;
            TextLabel->SetText(Items[SelectedItem]);
        }
    }

    void GUIComboBox::Clear()
    {
        Items.clear();
        SelectedItem = -1;
        TextLabel->SetText("");
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

        PostEvent(this, GUIElementEvent::Changed, &SelectedItem);
    }

    const std::string* GUIComboBox::GetItem(int item)
    {
        if (item < 0 || item >= int(Items.size()))
            return nullptr;

        return &(Items[item]);
    }

    void GUIComboBox::OnRender()
    {
        GUIPanel::OnRender();
    }

    void GUIComboBox::Setup()
    {
        IncrementButton = GUIButton::Create();
        IncrementButton->Serialize = false;
        IncrementButton->Name = "ComboBoxIncrementButton";
        IncrementButton->RelativeBounds = RelativeRect(RelativeValue(0.0f, false), RelativeValue(0.0f, false), RelativeValue(1.0f, false), RelativeValue(1.0f, false));
        IncrementButton->RelativeBounds.HorizontalAlignment = AlignmentTypes::Maximum;
        IncrementButton->RelativeBounds.VerticalAlignment = AlignmentTypes::Center;
        IncrementButton->Background = Background;
        IncrementButton->ElementClicked = [this](GUIElement*) {WantIncrement = true; };
        IncrementButton->Disable.Tint = DARKGRAY;
        AddChild(IncrementButton);

        DecrementButton = GUIButton::Create();
        DecrementButton->Serialize = false;
        DecrementButton->Name = "ComboBoxDecrementButton";
        DecrementButton->RelativeBounds = RelativeRect(RelativeValue(0.0f, false), RelativeValue(0.0f, false), RelativeValue(1.0f, false), RelativeValue(1.0f, false));
        DecrementButton->RelativeBounds.HorizontalAlignment = AlignmentTypes::Minimum;
        DecrementButton->RelativeBounds.VerticalAlignment = AlignmentTypes::Center;
        DecrementButton->Background = Background;
        DecrementButton->ElementClicked = [this](GUIElement*) {WantDecrement = true; };
        DecrementButton->Disable.Tint = DARKGRAY;
        AddChild(DecrementButton);

        TextLabel = GUILabel::Create();
        TextLabel->Serialize = false;
        TextLabel->Name = "ComboBoxText";
        TextLabel->TextFont.Size = 10;
        TextLabel->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(0.0f, false), RelativeValue(1.0f, true), RelativeValue(1.0f, false));
        TextLabel->HorizontalAlignment = AlignmentTypes::Center;
        TextLabel->VerticalAlignment = AlignmentTypes::Center;
        AddChild(TextLabel);
    }

    void GUIComboBox::OnPreUpdate()
    {
        WantDecrement = false;
        WantIncrement = false;

        DecrementButton->Disabled = SelectedItem <= 0;
        IncrementButton->Disabled = SelectedItem < 0 || SelectedItem == int(Items.size()) - 1;
    }

    void GUIComboBox::OnPostChildUpdate()
    {
        int currentIndex = SelectedItem;
        if (WantIncrement)
        {
            if (SelectedItem < int(Items.size()) - 1)
                SetSelectedItemIndex(SelectedItem + 1);
        }

        if (WantDecrement)
        {
            if (SelectedItem > 0)
                SetSelectedItemIndex(SelectedItem - 1);
        }

        if (SelectedItem != currentIndex && SelectedItem >= 0)
            TextLabel->SetText(Items[SelectedItem]);
    }

    bool GUIComboBox::Read(const Value& object, Document& document)
    {
        GUIPanel::Read(object, document);

        GUIScreenReader::ReadColor(object, "text_color", TextColor);

        GUIScreenReader::ReadMember(object, "text_font", TextFont.Name);
        GUIScreenReader::ReadMember(object, "text_size", TextFont.Size);

        auto itemsItr = object.FindMember("item");

        Items.clear();

        auto itr = object.FindMember("decrement_button");
        if (itr != object.MemberEnd() && itr->value.IsObject())
            DecrementButton->Read(itr->value, document);

        itr = object.FindMember("increment_button");
        if (itr != object.MemberEnd() && itr->value.IsObject())
            IncrementButton->Read(itr->value, document);

        itr = object.FindMember("text_label");
        if (itr != object.MemberEnd() && itr->value.IsObject())
            TextLabel->Read(itr->value, document);

        TextLabel->SetText("");

        if (itemsItr != object.MemberEnd() && itemsItr->value.IsArray())
        {
            for (auto& item : itemsItr->value.GetArray())
            {
                if (item.IsString())
                {
                    Add(item.GetString());
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

        Value decrementButtonObject(kObjectType);
        DecrementButton->Write(decrementButtonObject, document);
        object.AddMember("decrement_button", decrementButtonObject, alloc);

        Value incrementButtonObject(kObjectType);
        IncrementButton->Write(incrementButtonObject, document);
        object.AddMember("increment_button", incrementButtonObject, alloc);

        Value textLableObject(kObjectType);
        TextLabel->Write(textLableObject, document);
        object.AddMember("text_label", textLableObject, alloc);

        return true;
    }

    bool GUICheckBox::SetChecked(bool check)
    {
        if (check == Checked)
            return check;

        Checked = check;
        PostEvent(this, GUIElementEvent::Changed, &Checked);
        OnCheckChanged();

        return check;
    }

    bool GUICheckBox::Read(const rapidjson::Value& object, rapidjson::Document& document)
    {
        GUIPanel::Read(object, document);

        return true;
    }

    bool GUICheckBox::Write(rapidjson::Value& object, rapidjson::Document& document)
    {
        GUIPanel::Write(object, document);
    
        return true;
    }

    void GUICheckBox::OnUpdate()
    {
        GUIPanel::OnUpdate();
    }

    void GUICheckBox::OnRender()
    {
        GUIPanel::OnRender();

        if (Checked)
        {
            if (CheckTexture.Texture.Valid())
                Draw(CheckTexture);
            else
                Draw(CheckTexture.Tint, BLANK, CheckTexture.Offset, CheckTexture.Scale);
        }
        else
        {
            if (UncheckedTexture.Texture.Valid())
                Draw(UncheckedTexture);
            else
                Draw(UncheckedTexture.Tint, BLANK, UncheckedTexture.Offset, UncheckedTexture.Scale);
        }
    }

    void GUICheckBox::OnClickStart()
    {
        SetChecked(!Checked);
    }
}


