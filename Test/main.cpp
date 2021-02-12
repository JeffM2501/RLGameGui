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

#include "raylib.h"
#include "RLGameGui.h"
#include "StandardElements.h"

using namespace RLGameGUI;

void main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1000, 600, "GUI Test");

	Texture2D background = LoadTexture("resources/hex.png");
	Color backgroundColor = GetColor(0x1F252D);

	Texture2D logo = LoadTexture("resources/raylib_logo.png");
	Texture2D panelBG = LoadTexture("resources/KeyValueBackground.png");

	Texture2D imageButton = LoadTexture("resources/ButtonBackground.png");
	Texture2D imageButtonHover = LoadTexture("resources/ButtonBackground.hover.png");
	Texture2D imageButtonDisabled = LoadTexture("resources/ButtonBackground.disabled.png");
	Texture2D imageButtonPressed = LoadTexture("resources/ButtonBackground.active.png");
	
	int fontSize = 26;
	Font textFont = LoadFontEx("resources/fonts/BebasNeue Book.otf", fontSize, NULL, 0);

	Color textColor = RAYWHITE;

	GUIScreen::Ptr rootScreen = GUIScreen::Create();

	GUIPanel::Ptr panel = GUIPanel::Create();
	panel->Name = "panel1";
	panel->RelativeBounds = RelativeRect(RelativeValue(1.0f, true), RelativeValue(1.0f, false), RelativeValue(0.75f, false), RelativeValue(0.75f, false), AlignmentTypes::Maximum, AlignmentTypes::Maximum, Vector2{ 10,10 });
	panel->Background = panelBG;
	panel->Padding = RelativePoint(16, 16);
	panel->Fillmode = PanelFillModes::NPatch;
	panel->NPatchGutters = Vector2{ 16, 16 };

	panel->Tint = WHITE;
	rootScreen->AddElement(panel);

	GUIPanel::Ptr panel2 = GUIPanel::Create();
	panel2->Name = "Panel2";
	panel2->RelativeBounds = RelativeRect{ 0.0f, 0.0f, 1.0f, 0.25f };
	panel2->Tint = GRAY;
	panel2->Outline = BLACK;
	panel2->OutlineThickness = 4;

	panel->AddChild(panel2);

	GUILabel::Ptr label = GUILabel::Create("I am IRON MAN",textFont,fontSize);
	label->RelativeBounds = RelativeRect{ 0, 10, 500, 40 };
	label->Tint = textColor;
	rootScreen->AddElement(label);

	GUILabel::Ptr label2 = GUILabel::Create("Centered", textFont, fontSize);
	label2->Tint = textColor;
	label2->RelativeBounds = RelativeRect{ 0, 20, 500, 40 };
	label2->HorizontalAlignment = AlignmentTypes::Center;
	rootScreen->AddElement(label2);

	GUILabel::Ptr label3 = GUILabel::Create("Right", textFont, fontSize);
	label3->Tint = textColor;
	label3->RelativeBounds = RelativeRect{ 0, 40, 500, 40 };
	label3->HorizontalAlignment = AlignmentTypes::Maximum;
	rootScreen->AddElement(label3);

	GUIImage::Ptr panel3 = GUIImage::Create();
	panel3->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(1.0f, false), RelativeValue(0.125f, true), RelativeValue(0.125f, true), AlignmentTypes::Minimum, AlignmentTypes::Maximum, Vector2{ 10,10 });
	panel3->Tint = WHITE;
	panel3->Background = logo;
	rootScreen->AddElement(panel3);

	GUIButton::Ptr button = GUIButton::Create(imageButton);
	button->RelativeBounds = RelativeRect(RelativeValue(1.0f, true), RelativeValue(0.0f, false), RelativeValue(150, true), RelativeValue(50, true), AlignmentTypes::Maximum, AlignmentTypes::Minimum, Vector2{ 10,10 });
	button->TextColor = WHITE;
	button->TextFont = textFont;
	button->TextSize = (float)fontSize;
	button->SetText("Button");
	button->Fillmode = PanelFillModes::NPatch;
	button->NPatchGutters = Vector2{ 16, 16 };
	button->HoverTexture = imageButtonHover;
	button->DisableTexture = imageButtonDisabled;
	button->PressTexture = imageButtonPressed;

	button->ElementClicked = [&label3](GUIElement*) {label3->SetText("Clicked"); };

	rootScreen->AddElement(button);

	Manager::PushScreen(rootScreen);

	while (!WindowShouldClose())
	{
		Manager::Update();

		BeginDrawing();
		ClearBackground(backgroundColor);

		float scale = 4;
		float offset = (float)GetTime() * 200;
		DrawTexturePro(background, Rectangle{ offset, offset,GetScreenWidth() * scale,GetScreenHeight() * scale }, Rectangle{ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, Vector2{ 0,0 }, 0, Color{ 255,255,255,64 });
		//  DrawTextureTiled(background, Rectangle{ 0,0,(float)background.width,(float)background.height }, Rectangle{ 0,0,(float)GetScreenWidth(),(float)GetScreenHeight() }, Vector2{ 0,0 }, 0, 0, WHITE);

	  //	DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), RED);

		Manager::Render();
		EndDrawing();
	}
	UnloadTexture(logo);
	CloseWindow();
}