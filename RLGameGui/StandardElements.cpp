/**********************************************************************************************
*
*   RLGameGUi * A game gui for raylib
*
*   LICENSE: MIT
*
*   Copyright (c) 2020 Jeffery Myers
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

namespace RLGameGUI
{
    static Vector2 V2Zero = { 0, 0 };
    static Vector2 V2One = { 1.0f, 1.0f };

    void GUIPanel::Draw(Color fill, Color outline, Vector2 offset, Vector2 scale)
    {
        Rectangle rect = GetScreenRect();
        rect.x += offset.x;
        rect.y += offset.y;
        rect.width += scale.x;
        rect.height += scale.y;

        if (Tint.a != 0)
        {
            DrawRectangleRec(rect, fill);
        }
        if (outline.a != 0 && OutlineThickness > 0)
        {
            rect.height += 1;
            DrawRectangleLinesEx(rect, OutlineThickness, outline);
        }
    }

    void GUIPanel::Draw(Texture2D fill, Color tint, Vector2 offset, Vector2 scale)
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
            DrawTextureTiled(fill, SourceRect, rect, V2Zero, 0, 1, tint);
        }
        else if (Fillmode == PanelFillModes::Fill)
        {
            DrawTexturePro(fill, SourceRect, rect, V2Zero, 0, tint);
        }
        else if (Fillmode == PanelFillModes::NPatch)
        {
            if (NPatchData.source.width == 0)
            {
                NPatchData.source = SourceRect;

                NPatchData.left = NPatchData.right = (int)NPatchGutters.x;
                NPatchData.top = NPatchData.bottom = (int)NPatchGutters.y;

                if (NPatchGutters.x == 0)
                    NPatchData.type = NPT_3PATCH_VERTICAL;
                else if (NPatchGutters.y == 0)
                    NPatchData.type = NPT_3PATCH_HORIZONTAL;
                else
                    NPatchData.type = NPT_9PATCH;
            }

            DrawTextureNPatch(fill, NPatchData, rect, V2Zero, 0, tint);
        }
    }

	void GUIPanel::OnRender()
	{
        if (Background.id == 0)
            Draw(Tint, Outline, V2Zero, V2Zero);
        else
            Draw(Background, Tint, V2Zero, V2Zero);
	}

    void GUIImage::OnRender()
    {
		if (Background.id == 0)
			DrawRectangleRec(GetScreenRect(), Tint);
		else
			DrawTexturePro(Background, RealSourceRect,RealDestRect, Vector2{ 0,0 }, 0, Tint);
    }

    void GUIImage::OnUpdate()
    {
       
    }

    void GUIImage::OnPreResize()
    {
        if (RelativeBounds.Size.IsZero())
        {
            RelativeBounds.Size = RelativePoint(Background.width, Background.height);
        }

        if (SourceRect.width == 0)
        {
            SourceRect.width = (float)Background.width;
            SourceRect.height = (float)Background.height;
        }
    }

    void GUIImage::OnResize()
    {
        if (Background.id == 0)
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

    static Rectangle ResizeTextBox(float& textSize, const std::string& text, Font& textFont, float& spacing, Rectangle& screenRect, RLGameGUI::AlignmentTypes hAlign, RLGameGUI::AlignmentTypes vAlign)
    {
        int defaultFontSize = 10;   // Default Font chars height in pixel
        if (textSize < defaultFontSize)
            textSize = (float)defaultFontSize;

        spacing = textSize / defaultFontSize;

        Font fontToUse = textFont;
        if (textFont.texture.id == 0)
            fontToUse = GetFontDefault();

        Vector2 size = MeasureTextEx(fontToUse, text.c_str(), textSize, spacing);

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
        TextRect = ResizeTextBox(TextSize, Text, TextFont, Spacing, ScreenRect, HorizontalAlignment, VerticalAlignment); 
    }

    void GUILabel::OnRender()
    {
        Font fontToUse = TextFont;
        if (TextFont.texture.id == 0)
            fontToUse = GetFontDefault();

        DrawTextRec(fontToUse, Text.c_str(), TextRect, TextSize, Spacing, false, Tint);
    }

    void GUIButton::OnResize()
    {
        TextRect = ResizeTextBox(TextSize, Text, TextFont, Spacing, ScreenRect, AlignmentTypes::Center, AlignmentTypes::Center);
    }

    void GUIButton::OnRender()
    {
        Color color = Tint;
        Color labelColor = TextColor;
        Texture2D tx = Background;

        Vector2 offset = V2Zero;
        Vector2 scale = V2Zero;

        if (Disabled)
        {
            color = DisableTint;
            labelColor = DisableTextColor;
        }
        else
        {
            if (Clicked)
            {
                offset.x = 2;
                offset.y = 2;
                if (PressTint.a > 0)
                    color = PressTint;

                if (PressTextColor.a > 0)
                    labelColor = PressTextColor;
            }
            else if (Hovered)
            {
                offset.x = -1;
                offset.y = -1;
                scale.x = 2;
                scale.y = 2;

                if (HoverTint.a > 0)
                    color = HoverTint;

                if (HoverTextColor.a > 0)
                    labelColor = HoverTextColor;
            }
        }

        if (tx.id == 0)
            Draw(color, Outline, offset, scale);
        else
            Draw(tx, color, offset, scale);

        if (!Text.empty())
        {
            Font fontToUse = TextFont;
            if (TextFont.texture.id == 0)
                fontToUse = GetFontDefault();

            Rectangle rect = TextRect;
            rect.x += offset.x;
            rect.y += offset.y;

            DrawTextRec(fontToUse, Text.c_str(), rect, TextSize, Spacing, false, labelColor);
        }
    }
}


