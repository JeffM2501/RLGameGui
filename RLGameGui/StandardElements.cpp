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
	void GUIPanel::OnRender()
	{
		DrawRectangleRec(GetScreenRect(), Tint);
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

    void GUIImage::OnResize()
    {
        if (Background.id == 0)
            return;

        if (RelativeBounds.Size.IsZero())
        {
            RelativeBounds.Size = RelativePoint(Background.width, Background.height);
            Resize();
            return;
        }

        if (SourceRect.width == 0)
        {
            SourceRect.width = (float)Background.width;
            SourceRect.height = (float)Background.height;
        }

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


    void GUILabel::SetText(const std::string& text)
    {
        if (text == Text)
            return;
        Text = text;

        OnResize();
    }

    void GUILabel::OnResize()
    {
        int defaultFontSize = 10;   // Default Font chars height in pixel
        if (TextSize < defaultFontSize)
            TextSize = (float)defaultFontSize;

        Spacing = TextSize / defaultFontSize;

        Font fontToUse = TextFont;
        if (TextFont.texture.id == 0)
            fontToUse = GetFontDefault();

        Vector2 size = MeasureTextEx(fontToUse, Text.c_str(), TextSize, Spacing);

        TextRect.width = size.x + Spacing * 2;
        switch (HorizontalAllignment)
        {
        case RLGameGUI::AllignmentTypes::Maximum:
            TextRect.x = (ScreenRect.x + ScreenRect.width) - size.x;
            break;

        case RLGameGUI::AllignmentTypes::Center:
            TextRect.x = (ScreenRect.x + ScreenRect.width * 0.5f) - size.x * 0.5f;
            break;

        default:
            TextRect.x = ScreenRect.x;
            break;
        }

        TextRect.height = size.y;
        switch (VerticalAllignment)
        {
        case RLGameGUI::AllignmentTypes::Maximum:
            TextRect.y = (ScreenRect.y + ScreenRect.height) - size.y;
            break;

        case RLGameGUI::AllignmentTypes::Center:
            TextRect.y = (ScreenRect.y + ScreenRect.height * 0.5f) - size.y * 0.5f;
            break;

        default:
            TextRect.y = ScreenRect.y;
            break;
        }
    }

    void GUILabel::OnRender()
    {
        Font fontToUse = TextFont;
        if (TextFont.texture.id == 0)
            fontToUse = GetFontDefault();

        DrawTextRec(fontToUse, Text.c_str(), TextRect, TextSize, Spacing, false, Tint);
    }
}