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

	GUIScreen::Ptr rootScreen = GUIScreen::Create();

	GUIPanel::Ptr panel = GUIPanel::Create();
	panel->RelativeBounds = RelativeRect(RelativeValue(1.0f, true), RelativeValue(1.0f, false), RelativeValue(0.5f, true), RelativeValue(0.5f, true), AllignmentTypes::Maximum, AllignmentTypes::Maximum, Vector2{ 10,10 });
	panel->Tint = GRAY;
	rootScreen->AddElement(panel);

    GUIImage::Ptr panel2 = GUIImage::Create();
	panel2->RelativeBounds = RelativeRect{ 10, 10, 0, 0 };
	panel2->Tint = BLUE;
	panel2->Background = logo;

	panel->AddChild(panel2);

	GUILabel::Ptr label = GUILabel::Create("I am IRON MAN");
	label->RelativeBounds = RelativeRect{ 10, 10, 500, 40 };
	rootScreen->AddElement(label);


    GUIImage::Ptr panel3 = GUIImage::Create();
	panel3->RelativeBounds = RelativeRect(RelativeValue(0.0f, true), RelativeValue(1.0f, false), RelativeValue(0.125f, true), RelativeValue(0.125f, true), AllignmentTypes::Minimum, AllignmentTypes::Maximum, Vector2{ 10,10 });
	panel3->Tint = MAROON;
	panel3->Background = logo;

	rootScreen->AddElement(panel3);

	Manager::PushScreen(rootScreen);

	while (!WindowShouldClose())
	{
		Manager::Update();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		Manager::Render();
		EndDrawing();
	}
	UnloadTexture( logo );
	CloseWindow();
}