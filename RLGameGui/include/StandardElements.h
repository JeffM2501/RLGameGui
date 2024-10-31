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

#include "GUIElement.h"
#include "raylib.h"
#include "raymath.h"

namespace RLGameGUI
{
    struct TextureRecord
    {
        std::string Name;
        bool Valid() const { return !Name.empty(); }
        Texture2D GetTexture();

    private:
        Texture2D Texture = { 0 };
    };

    class GUIFrame : public GUIElement
    {
    public:
        DEFINE_ELEMENT(GUIFrame)

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
        DEFINE_ELEMENT(GUIPanel)

		Color Tint = WHITE;
        Color Outline = BLANK;
        int OutlineThickness = 0;

        TextureRecord Background;
        Rectangle SourceRect = { 0,0,0,0 };

        PanelFillModes Fillmode = PanelFillModes::Fill;

        Vector2 NPatchGutters = Vector2{ 0,0 };

        GUIPanel() {};
        GUIPanel(Color tint) : Tint(Tint) {};
        GUIPanel(const std::string& img) { Background.Name = img; };

		typedef std::shared_ptr<GUIPanel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIPanel>(); }
        inline static Ptr Create(Color tint) { return std::make_shared<GUIPanel>(tint); }
        inline static Ptr Create(const std::string& img) { return std::make_shared<GUIPanel>(img); }

	protected:
      	void OnRender() override;

        void Draw(Color fill, Color outline, const Vector2& offset, const Vector2& scale);
        void Draw(const Texture2D& fill, Color tint, const Rectangle& sourceRect, const Vector2 &offset, const Vector2& scale);

        NPatchInfo NPatchData = { 0 };
	};

    class GUIImage : public GUIElement
    {
    public:
        DEFINE_ELEMENT(GUIImage)

        Color Tint = WHITE;
        TextureRecord Background;
        Rectangle SourceRect = { 0,0,0,0 };

        bool Clip = false;

        GUIImage() {}
        GUIImage(const std::string& img) { Background.Name = img; }

        typedef std::shared_ptr<GUIImage> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIImage>(); }
        inline static Ptr Create(const std::string& img) { return std::make_shared<GUIImage>(img); }

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
        DEFINE_ELEMENT(GUILabel)

        Color Tint = BLACK;
        Font TextFont = GetFontDefault();
        float TextSize = 20;

        GUILabel() {}
        GUILabel(const std::string& text) : Text(text) {}
        GUILabel(const std::string& text, const Font& font, int size = 16) : Text(text), TextFont(font), TextSize((float)size) {}

        typedef std::shared_ptr<GUILabel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUILabel>(); }
        inline static Ptr Create(const std::string& text) { return std::make_shared<GUILabel>(text); }
        inline static Ptr Create(const std::string& text, const Font& font, int size = 16) { return std::make_shared<GUILabel>(text, font, size); }

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
        DEFINE_ELEMENT(GUIButton)

        Color TextColor = BLACK;
        Font TextFont = GetFontDefault();
        float TextSize = 20;

        Color HoverTint = BLANK;
        Rectangle HoverSourceRect = { 0,0,0,0 };
        TextureRecord HoverTexture;
        Color HoverTextColor = BLANK;

        Color PressTint = BLANK;
        Rectangle PressSourceRect = { 0,0,0,0 };
        TextureRecord PressTexture;
        Color PressTextColor = BLANK;

        Color DisableTint = DARKGRAY;
        Rectangle DisableSourceRect = { 0,0,0,0 };
        TextureRecord DisableTexture;
        Color DisableTextColor = GRAY;

        Vector2 HoverOffset = Vector2Zeros;
        Vector2 HoverScale = Vector2Zeros;
        Vector2 PressOffset = Vector2Zeros;
        Vector2 PressScale = Vector2Zeros;

        GUIButton() {}
        GUIButton(const std::string& text, const std::string& background = std::string()) : Text(text) { Background.Name = background; }

        typedef std::shared_ptr<GUIButton> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIButton>(); }
        inline static Ptr Create(const std::string& text) { return std::make_shared<GUIButton>(text); }
        inline static Ptr Create(const std::string& text, const std::string& texture) { return std::make_shared<GUIButton>(text,texture); }

        inline virtual void SetText(const std::string& text) { Text = text; RelativeBounds.SetDirty(); }
        inline const std::string& GetText() { return Text; }

        virtual void SetButtonFrames(int framesX, int framesY, int backgroundX, int backgroundY, int hoverX = -1, int hoverY = -1, int pressX = -1, int pressY = -1, int disableX = -1, int disableY = -1);

    protected:
        void OnResize() override;
        void OnRender() override;

        std::string Text;

        Rectangle TextRect = { 0,0,0,0 };
        float Spacing = 1;
    };

    class GUIComboBox : public GUIPanel
    {
    public:
        DEFINE_ELEMENT(GUIComboBox)

        Color TextColor = BLACK;
        Font TextFont = GetFontDefault();
        float TextSize = 20;

        GUIComboBox() {}
        GUIComboBox(const std::string& texture) { Background.Name = texture; }

        typedef std::shared_ptr<GUIComboBox> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIComboBox>(); }
        inline static Ptr Create(const std::string& texture) { return std::make_shared<GUIComboBox>(texture); }

        std::vector<std::string>::const_iterator Begin();
        std::vector<std::string>::const_iterator End();
        std::vector<std::string>::const_iterator Erase(const std::vector<std::string>::const_iterator itr);
        void Add(const std::string& item);
        void Clear();

        int GetSelectedItemIndex();
        void SetSelectedItemIndex(int item);

        const std::string* GetItem(int item);

    protected:
        std::vector<std::string> Items;

        int SelectedItem = -1;
    };
}
