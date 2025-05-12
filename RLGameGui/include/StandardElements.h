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
#include "GUIScreenIO.h"
#include "rlText.h"

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

    struct FontRecord
    {
        std::string Name;
        float Size = 20;

        rltFont GetFont();

    private:
        rltFont CachedFont = { 0 };
    };

    enum class PanelFillModes
    {
        Fill = 0,
        Tile = 1,
        NPatch = 2,
    };

    struct GUITexture
    {
        Color Tint = WHITE;
        Rectangle SourceRect = { 0,0,0,0 };
        TextureRecord Texture;
        Vector2 Offset = Vector2Zeros;
        Vector2 Scale = Vector2Ones;

        PanelFillModes Fillmode = PanelFillModes::Fill;
        Rectangle NPatchGutters = Rectangle{ 0 ,0, -1, -1 };

        GUITexture(Color tint = WHITE, Rectangle sourceRect = { 0,0,0,0 }, const std::string& textureName = std::string())
        : Tint(tint)
        , SourceRect(sourceRect)
        {
            Texture.Name = textureName;
        }

        bool Read(const rapidjson::Value& object);
        bool ReadMember(const rapidjson::Value& object, const std::string& name);
        bool Write(rapidjson::Value& object, rapidjson::Document& document);
    };

    class GUIFrame : public GUIElement
    {
    public:
        DEFINE_ELEMENT(GUIFrame)

        GUIFrame() { Renders = false; }
    };

	class GUIPanel : public GUIElement
	{
	public:
        DEFINE_ELEMENT(GUIPanel)

		Color Tint = WHITE;
        Color Outline = BLANK;
        int OutlineThickness = 0;

        GUITexture Background;

        GUIPanel() {};
        GUIPanel(Color tint) : Tint(Tint) {};
        GUIPanel(const std::string& img) { Background.Texture.Name = img; };

		typedef std::shared_ptr<GUIPanel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIPanel>(); }
        inline static Ptr Create(Color tint) { return std::make_shared<GUIPanel>(tint); }
        inline static Ptr Create(const std::string& img) { return std::make_shared<GUIPanel>(img); }

        bool Read(const rapidjson::Value& object, rapidjson::Document& document) override;
        bool Write(rapidjson::Value& object, rapidjson::Document& document) override;

	protected:
      	void OnRender() override;

        void Draw(Color fill, Color outline, const Vector2& offset, const Vector2& scale);
        void Draw(GUITexture& fill);

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

        bool Read(const rapidjson::Value& object, rapidjson::Document& document) override;
        bool Write(rapidjson::Value& object, rapidjson::Document& document) override;

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

        FontRecord TextFont;

        bool ClipToRectangle = true;

        GUILabel() {}
        GUILabel(const std::string& text) : Text(text) {}
        GUILabel(const std::string& text, const std::string& font, float size = 20)
        : Text(text)
        {
            TextFont.Name = font;
            TextFont.Size = size;
        }

        typedef std::shared_ptr<GUILabel> Ptr;
        inline static Ptr Create() { return std::make_shared<GUILabel>(); }
        inline static Ptr Create(const std::string& text) { return std::make_shared<GUILabel>(text); }
        inline static Ptr Create(const std::string& text, const std::string& font, float size = 20) { return std::make_shared<GUILabel>(text, font, size); }

        AlignmentTypes HorizontalAlignment = AlignmentTypes::Minimum;
        AlignmentTypes VerticalAlignment = AlignmentTypes::Minimum;

        inline virtual void SetText(const std::string& text) { Text = text; RelativeBounds.SetDirty(); }
        inline const std::string& GetText() { return Text; }

        bool Read(const rapidjson::Value& object, rapidjson::Document& document) override;
        bool Write(rapidjson::Value& object, rapidjson::Document& document) override;

    protected:
        void OnRender() override;
        void OnResize() override;

        std::string Text;

        Rectangle TextRect = { 0,0,0,0 };
    };

    class GUIButton : public GUIPanel
    {
    public:
        DEFINE_ELEMENT(GUIButton)

        Color TextColor = BLACK;
        FontRecord TextFont;

        GUITexture Hover = { BLANK };
        GUITexture Press = { BLANK };
        GUITexture Disable = { DARKGRAY };

        Color HoverTextColor = BLANK;
        Color PressTextColor = BLANK;
        Color DisableTextColor = GRAY;

        GUIButton() {}
        GUIButton(const std::string& text, const std::string& background = std::string()) : Text(text) { Background.Texture.Name = background; }

        typedef std::shared_ptr<GUIButton> Ptr;
        inline static Ptr Create() { return std::make_shared<GUIButton>(); }
        inline static Ptr Create(const std::string& text) { return std::make_shared<GUIButton>(text); }
        inline static Ptr Create(const std::string& text, const std::string& texture) { return std::make_shared<GUIButton>(text,texture); }

        inline virtual void SetText(const std::string& text) { Text = text; RelativeBounds.SetDirty(); }
        inline const std::string& GetText() { return Text; }

        virtual void SetButtonFrames(int framesX, int framesY, int backgroundX, int backgroundY, int hoverX = -1, int hoverY = -1, int pressX = -1, int pressY = -1, int disableX = -1, int disableY = -1);

        bool Read(const rapidjson::Value& object, rapidjson::Document& document) override;
        bool Write(rapidjson::Value& object, rapidjson::Document& document) override;

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
        FontRecord TextFont;

        GUIComboBox() 
        { 
            Setup();
        }

        GUIComboBox(const std::string& texture)
        {
            Background.Texture.Name = texture; 
            Setup();
        }

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

        bool Read(const rapidjson::Value& object, rapidjson::Document& document) override;
        bool Write(rapidjson::Value& object, rapidjson::Document& document) override;

        void OnRender() override;

        GUIButton::Ptr IncrementButton = nullptr;
        GUIButton::Ptr DecrementButton = nullptr;

        GUILabel::Ptr TextLabel = nullptr;

        Function SelectedItemChanged;
    protected:
        void Setup();

        void OnPreUpdate() override;
        void OnPostChildUpdate() override;

        virtual void OnSelectedItemChanged() { if (SelectedItemChanged) SelectedItemChanged(this); }

        std::vector<std::string> Items;

        int SelectedItem = -1;

        bool WantIncrement = false;
        bool WantDecrement = false;
    };

    class GUICheckBox : public GUIPanel
    {
    public:
        DEFINE_ELEMENT(GUICheckBox)

        typedef std::shared_ptr<GUICheckBox> Ptr;
        inline static Ptr Create() { return std::make_shared<GUICheckBox>(); }

        GUITexture CheckTexture = { DARKGRAY };
        GUITexture UncheckedTexture = { BLANK };
        Function CheckChanged;

        bool GetChecked() const { return Checked; }
        bool SetChecked(bool check);

        bool Read(const rapidjson::Value& object, rapidjson::Document& document) override;
        bool Write(rapidjson::Value& object, rapidjson::Document& document) override;

    protected:
        bool Checked = false;
        
        void OnUpdate() override;
        void OnRender() override;
        void OnClickStart() override;

        virtual void OnCheckChanged() { if (CheckChanged) CheckChanged(this); }
    };

    void RegisterStandardElements();
}
