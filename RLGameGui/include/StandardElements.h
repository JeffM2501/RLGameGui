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

		typedef std::shared_ptr<GUIPanel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIPanel>(); }

	protected:
		void OnRender() override;

        NPatchInfo NPatchData = { 0 };
	};

    class GUIImage : public GUIElement
    {
    public:
        Color Tint = WHITE;
        Texture2D Background = { 0 };
        Rectangle SourceRect = { 0,0,0,0 };

        bool Clip = false;

        typedef std::shared_ptr<GUIImage> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIImage>(); }

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

        const std::string& GetText() const { return Text; }
        void SetText(const std::string& text);

    protected:
        void OnRender() override;
        void OnResize() override;

        std::string Text;

        Rectangle TextRect = { 0,0,0,0 };
        float Spacing = 1;
    };
}
