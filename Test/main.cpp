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

	Texture2D logo = LoadTexture("resources/raylib_logo.png");
	Texture2D atlas = LoadTexture("resources/cubicmap_atlas.png");
	Texture2D nPatch = LoadTexture("resources/ninepatch_button.png");

	GUIScreen::Ptr rootScreen = GUIScreen::Create();

	GUIPanel::Ptr panel = GUIPanel::Create();
	panel->Name = "panel1";
	panel->RelativeBounds = RelativeRect(RelativeValue(1.0f, true), RelativeValue(1.0f, false), RelativeValue(0.75f, false), RelativeValue(0.75f, false), AlignmentTypes::Maximum, AlignmentTypes::Maximum, Vector2{ 10,10 });
 	panel->Background = nPatch;
 	panel->SourceRect.width = nPatch.width;
 	panel->SourceRect.height = nPatch.height * 0.25f;
 	panel->SourceRect.y = nPatch.height * 0.75f;
	panel->Padding = RelativePoint(8, 8);
	panel->Fillmode = PanelFillModes::NPatch;
	panel->NPatchGutters = Vector2{ 16, 16 };

	panel->Tint = WHITE;
// 	panel->OutlineThickness = 2;
// 	panel->Outline = DARKBLUE;
	rootScreen->AddElement(panel);

	GUIPanel::Ptr panel2 = GUIPanel::Create();
	panel2->Name = "Panel2";
	panel2->RelativeBounds = RelativeRect{ 0.0f, 0.0f, 1.0f, 0.25f };
	panel2->Tint = Color{ MAROON.r,MAROON.g,MAROON.b, 128 };
	panel2->Outline = BLACK;
	panel2->OutlineThickness = 4;

	panel->AddChild(panel2);

	GUILabel::Ptr label = GUILabel::Create("I am IRON MAN");
	label->RelativeBounds = RelativeRect{ 10, 10, 500, 40 };
	rootScreen->AddElement(label);

    GUILabel::Ptr label2 = GUILabel::Create("Centered");
	label2->RelativeBounds = RelativeRect{ 10, 20, 500, 40 };
	label2->HorizontalAlignment = AlignmentTypes::Center;
    rootScreen->AddElement(label2);

    GUILabel::Ptr label3 = GUILabel::Create("Right");
	label3->RelativeBounds = RelativeRect{ 10, 40, 500, 40 };
	label3->HorizontalAlignment = AlignmentTypes::Maximum;
    rootScreen->AddElement(label3);

    GUIImage::Ptr panel3 = GUIImage::Create();
	panel3->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(1.0f, false), RelativeValue(0.125f, true), RelativeValue(0.125f, true), AlignmentTypes::Minimum, AlignmentTypes::Maximum, Vector2{ 10,10 });
	panel3->Tint = MAROON;
	panel3->Background = logo;

	rootScreen->AddElement(panel3);

	Manager::PushScreen(rootScreen);

	while (!WindowShouldClose())
	{
		Manager::Update();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), RED);

		Manager::Render();
		EndDrawing();
	}
	UnloadTexture( logo );
	CloseWindow();
}