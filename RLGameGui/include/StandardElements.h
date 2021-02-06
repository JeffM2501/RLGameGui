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

#pragma once

#include "GUIElement.h"
#include "raylib.h"

namespace RLGameGUI
{
    class GUIFrame : public GUIElement
    {
    public:
        GUIFrame() { Renders = false; }
    };

    enum class PanelFillModes
    {
        Fill,
        Tile,
        NPatch,
    };

	class GUIPanel : public GUIElement
	{
	public:
		Color Tint = WHITE;
        Color Outline = BLANK;
        int OutlineThickness = 0;

        Texture2D Background = { 0 };
        Rectangle SourceRect = { 0,0,0,0 };

        PanelFillModes Fillmode = PanelFillModes::Fill;

        Vector2 NPatchGutters = Vector2{ 0,0 };

        GUIPanel() {};
        GUIPanel(Color tint) : Tint(Tint) {};
        GUIPanel(Texture2D img) : Background(img) {};

		typedef std::shared_ptr<GUIPanel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIPanel>(); }
        inline static Ptr Create(Color tint) { return std::make_shared<GUIPanel>(tint); }
        inline static Ptr Create(Texture2D img) { return std::make_shared<GUIPanel>(img); }

	protected:
      	void OnRender() override;

        void Draw(Color fill, Color outline, Vector2 offset, Vector2 scale);
        void Draw(Texture2D fill, Color tint, Vector2 offset, Vector2 scale);

        NPatchInfo NPatchData = { 0 };
	};

    class GUIImage : public GUIElement
    {
    public:
        Color Tint = WHITE;
        Texture2D Background = { 0 };
        Rectangle SourceRect = { 0,0,0,0 };

        bool Clip = false;

        GUIImage() {}
        GUIImage(Texture2D img) : Background(img) {}

        typedef std::shared_ptr<GUIImage> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIImage>(); }
        inline static Ptr Create(Texture2D img) { return std::make_shared<GUIImage>(img); }

    protected:
        void OnUpdate() override;
        void OnRender() override;
        void OnPreResize() override;
        void OnResize() override;

        Rectangle RealSourceRect = { 0 };
        Rectangle RealDestRect = { 0 };
    };

    class GUILabel : public GUIElement
    {
    public:
        Color Tint = BLACK;
        Font TextFont = GetFontDefault();
        float TextSize = 20;

        GUILabel() {}
        GUILabel(const std::string& text) : Text(text) {}

        typedef std::shared_ptr<GUILabel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUILabel>(); }
        inline static Ptr Create(const std::string& text) { return std::make_shared<GUILabel>(text); }

        AlignmentTypes HorizontalAlignment = AlignmentTypes::Minimum;
        AlignmentTypes VerticalAlignment = AlignmentTypes::Minimum;

        inline virtual void SetText(const std::string& text) { Text = text; RelativeBounds.SetDirty(); }
        inline const std::string& GetText() { return Text; }

    protected:
        void OnRender() override;
        void OnResize() override;

        std::string Text;

        Rectangle TextRect = { 0,0,0,0 };
        float Spacing = 1;
    };

    class GUIButton : public GUIPanel
    {
    public:
        Color TextColor = BLACK;
        Font TextFont = GetFontDefault();
        float TextSize = 20;

        Color HoverTint = BLANK;
        Rectangle HoverSourceRect = { 0,0,0,0 };
        Texture2D HoverTexture = { 0 };
        Color HoverTextColor = BLANK;

        Color PressTint = BLANK;
        Rectangle PressSourceRect = { 0,0,0,0 };
        Texture2D PressTexture = { 0 };
        Color PressTextColor = BLANK;

        Color DisableTint = DARKGRAY;
        Rectangle DisableSourceRect = { 0,0,0,0 };
        Texture2D DisableTexture = { 0 };
        Color DisableTextColor = GRAY;

        GUIButton() {}
        GUIButton(const std::string& text) : Text(text) {}

        typedef std::shared_ptr<GUIButton> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIButton>(); }
        inline static Ptr Create(const std::string& text) { return std::make_shared<GUIButton>(text); }

        inline virtual void SetText(const std::string& text) { Text = text; RelativeBounds.SetDirty(); }
        inline const std::string& GetText() { return Text; }

    protected:
        void OnResize() override;
        void OnRender() override;

        std::string Text;

        Rectangle TextRect = { 0,0,0,0 };
        float Spacing = 1;
    };
}
