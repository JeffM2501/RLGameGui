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

#include "raylib.h"
#include "RLGameGui.h"
#include "StandardElements.h"
#include "GUITextureManager.h"

#include "GUIScreenIO.h"

using namespace RLGameGUI;

int main( int argc, char** argv)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(1000, 600, "GUI Test");
	SetTargetFPS(300);

	TextureManager::SetResourceDir("resources");
	RegisterStandardElements();

	Texture background = TextureManager::GetTexture("hex.png");
	Color backgroundColor = GetColor(0x1F252D);

	std::string logo = ("raylib_logo.png");
	std::string panelBG = ("KeyValueBackground.png");

	std::string imageButton = ("ButtonBackground.png");
	std::string imageButtonHover = ("ButtonBackground.hover.png");
	std::string imageButtonDisabled = ("button_square_depth_border.png");
	std::string imageButtonPressed = ("ButtonBackground.active.png");
	
	float fontSize = 26;
	std::string textFont = "fonts/BebasNeue Book.otf";

	Color textColor = RAYWHITE;

	GUIScreen::Ptr rootScreen = GUIScreen::Create();

    GUIPanel::Ptr panel = GUIPanel::Create();
    panel->Name = "Panel1";
    panel->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(0.0f, false), RelativeValue(0.75f, false), RelativeValue(0.75f, false), AlignmentTypes::Maximum, AlignmentTypes::Maximum, Vector2{ 10,10 });
    panel->Background.Texture.Name = panelBG;
    panel->Padding = RelativePoint(16, 16);
    panel->Background.Fillmode = PanelFillModes::NPatch;
    panel->Background.NPatchGutters = Rectangle{ 16, 16, 16, 16 };

    panel->Tint = WHITE;
    rootScreen->AddElement(panel);

    GUIPanel::Ptr panel2 = GUIPanel::Create();
    panel2->Name = "Panel2";
    panel2->RelativeBounds = RelativeRect{ 0.0f, 0.0f, 1.0f, 0.25f };
    panel2->Tint = GRAY;
    panel2->Outline = BLACK;
    panel2->OutlineThickness = 4;

    panel->AddChild(panel2);

    GUIPanel::Ptr comboPanel = GUIPanel::Create();
    comboPanel->Name = "Panel3";
    comboPanel->RelativeBounds = RelativeRect{ 0.0f, 0.25f, 1.0f, 0.75f };
    comboPanel->Tint = BLANK;
    comboPanel->Outline = BLANK;
    comboPanel->OutlineThickness = 0;

    panel->AddChild(comboPanel);

    GUILabel::Ptr testLabel = GUILabel::Create("Test Label");
    testLabel->RelativeBounds = RelativeRect{ 10, 10, 500, 40 };
    testLabel->Padding.X.SizeValue = 10;
    testLabel->TextFont.Name = textFont;
    testLabel->TextFont.Size = fontSize;
    testLabel->Tint = textColor;
    comboPanel->AddChild(testLabel);
	
	GUIComboBox::Ptr testCombo = GUIComboBox::Create();
	testCombo->Name = "ComboBox";
	testCombo->RelativeBounds.Origin.X = RelativeValue(0.0f, true);
    testCombo->RelativeBounds.Origin.Y = RelativeValue(55, false);

    testCombo->RelativeBounds.Size.X = RelativeValue(1.0f, true);
    testCombo->RelativeBounds.Size.Y = RelativeValue(55, false);

	testCombo->Background.Fillmode = PanelFillModes::NPatch;
	testCombo->Background.NPatchGutters = Rectangle{ 16,16,16,16 };
	testCombo->Background.Texture.Name = "KeyValueBackground.png";

	testCombo->IncrementButton->Background.Texture.Name = "pip_up.png";
	testCombo->IncrementButton->Press.Texture.Name = "pip_down.png";
    testCombo->IncrementButton->Disable.Texture.Name = "pip_down.png";
    testCombo->IncrementButton->Disable.Tint = DARKGRAY;
	testCombo->IncrementButton->RelativeBounds.Offset.x = 10;
	testCombo->IncrementButton->RelativeBounds.Size.X = RelativeValue(0.5f,false);
    testCombo->IncrementButton->RelativeBounds.Size.Y = RelativeValue(0.5f,false);

    testCombo->DecrementButton->Background.Texture.Name = "pip_up.png";
    testCombo->DecrementButton->Press.Texture.Name = "pip_down.png";
	testCombo->DecrementButton->Disable.Texture.Name = "pip_down.png";
	testCombo->DecrementButton->Disable.Tint = DARKGRAY;
	testCombo->DecrementButton->RelativeBounds.Offset.x = 10;
    testCombo->DecrementButton->RelativeBounds.Size.X = RelativeValue(0.5f,false);
    testCombo->DecrementButton->RelativeBounds.Size.Y = RelativeValue(0.5f, false);

	testCombo->TextLabel->Tint = textColor;
	testCombo->TextLabel->TextFont.Name = textFont;
	testCombo->TextLabel->TextFont.Size = fontSize;
	testCombo->TextLabel->Tint = textColor;

	testCombo->Add("Item 1");
	testCombo->Add("Item 2");
	testCombo->Add("Item 3");
	comboPanel->AddChild(testCombo);

	GUICheckBox::Ptr checkbox = GUICheckBox::Create();
	checkbox->Name = "Checkbox";
	checkbox->RelativeBounds.Origin.X = RelativeValue(0.0f, true);
	checkbox->RelativeBounds.Origin.Y = RelativeValue(155, false);

	checkbox->RelativeBounds.Size.X = RelativeValue(32, false);
	checkbox->RelativeBounds.Size.Y = RelativeValue(32, false);

	checkbox->Background.Fillmode = PanelFillModes::NPatch;
	checkbox->Background.NPatchGutters = Rectangle{ 16,16,16,16 };
	checkbox->Background.Texture.Name = "KeyValueBackground.png";

	checkbox->CheckTexture.Tint = WHITE;
	checkbox->CheckTexture.Texture.Name = "pip_up.png";

	GUILabel::Ptr checkboxLabel = GUILabel::Create("Checkbox", textFont, fontSize);

	checkboxLabel->Tint = WHITE;
	checkboxLabel->RelativeBounds.Origin.X = RelativeValue(1.3f, true);
	checkboxLabel->RelativeBounds.Origin.Y = RelativeValue(0.2f, false);

	checkboxLabel->RelativeBounds.Size.X = RelativeValue(200, true);
	checkboxLabel->RelativeBounds.Size.Y = RelativeValue(0.0f, false);
	checkboxLabel->RelativeBounds.HorizontalAlignment = AlignmentTypes::Minimum;
	checkboxLabel->VerticalAlignment = AlignmentTypes::Minimum;

	checkbox->AddChild(checkboxLabel);

	comboPanel->AddChild(checkbox);

    GUILabel::Ptr label = GUILabel::Create("I am IRON MAN", textFont, fontSize);
    label->RelativeBounds = RelativeRect{ 10, 10, 500, 40 };
    label->Padding.X.SizeValue = 10;
    label->Tint = textColor;
    rootScreen->AddElement(label);

    GUILabel::Ptr label2 = GUILabel::Create("Centered", textFont, fontSize);
    label2->Tint = textColor;
    label2->RelativeBounds = RelativeRect{ 0, 40, 500, 40 };
    label2->HorizontalAlignment = AlignmentTypes::Center;
    rootScreen->AddElement(label2);
  
    GUILabel::Ptr label3 = GUILabel::Create("Right", textFont, fontSize);
    label3->Tint = textColor;
    label3->Id = "DynamicLabel";
    label3->RelativeBounds = RelativeRect{ 0, 60, 500, 40 };
    label3->HorizontalAlignment = AlignmentTypes::Maximum;
    rootScreen->AddElement(label3);

    GUIImage::Ptr panel3 = GUIImage::Create();
    panel3->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(0.0f, false), RelativeValue(256), RelativeValue(256), AlignmentTypes::Minimum, AlignmentTypes::Maximum, Vector2{ 10,10 });
    panel3->Tint = WHITE;
    panel3->Background.Name = logo;

	GUIButton::Ptr button = GUIButton::Create(std::string(), imageButton);
	button->Id = "Clickable Button";
	button->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(0.0f, false), RelativeValue(150, true), RelativeValue(50, true), AlignmentTypes::Maximum, AlignmentTypes::Minimum, Vector2{ 10,10 });
	button->TextColor = WHITE;
	button->TextFont.Name = textFont;
	button->TextFont.Size = fontSize;
	button->SetText("Button");
	button->Background.Fillmode = PanelFillModes::NPatch;
	button->Background.NPatchGutters = Rectangle{ 16, 16, 16,16 };
	button->Hover = button->Background;
	button->Hover.Texture.Name = imageButtonHover;
	button->Disable = button->Background;
	button->Disable.Texture.Name = imageButtonDisabled;
	button->Disable.Tint = GRAY;
	button->Press = button->Background;
	button->Press.Texture.Name = imageButtonPressed;
	rootScreen->AddElement(button);

	GUILabel* dynamicButton = rootScreen->FindElement<GUILabel>("DynamicLabel");

	// register a click event handler
	rootScreen->RegisterEventHandler("Clickable Button", GUIElementEvent::Click, [&dynamicButton](GUIElement&, GUIElementEvent, void*) {if (dynamicButton) dynamicButton->SetText("Clicked"); });
    rootScreen->AddElement(panel3);

	Manager::PushScreen(rootScreen);

	while (!WindowShouldClose())
	{
		Manager::Update();

		BeginDrawing();
		ClearBackground(backgroundColor);

		float scale = 4;
		float offset = (float)GetTime() * 200;
		DrawTexturePro(background, Rectangle{ offset, offset,GetScreenWidth() * scale,GetScreenHeight() * scale }, Rectangle{ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, Vector2{ 0,0 }, 0, Color{ 255,255,255,64 });
		Manager::Render();

		DrawFPS(int(panel3->GetScreenRect().x), int(panel3->GetScreenRect().y) - 20);
		EndDrawing();
	}
	UnloadTexture(background);

	TextureManager::UnloadAll();
	FontManager::UnloadAll();
	CloseWindow();
	return 0;
}