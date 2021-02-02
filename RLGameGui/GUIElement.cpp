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

#include "GUIElement.h"

namespace RLGameGUI
{
	void GUIElement::Update()
	{
		OnUpdate();
		for (auto child : Children)
			child->Update();
	}

	void GUIElement::Resize()
	{
		if (Parent != nullptr)
		{
			Rectangle& parrentRect = Parent->GetScreenRect();
			ScreenRect.x = parrentRect.x + RelativeRect.x;
			ScreenRect.y = parrentRect.y + RelativeRect.y;
			ScreenRect.width = RelativeRect.width;
			ScreenRect.height = RelativeRect.height;
		}

        OnResize();
        for (auto child : Children)
            child->Resize();
	}

    GUIElement::Ptr GUIElement::AddChild(GUIElement::Ptr child)
    {
		child->Parent = this;
    }

    void GUIElement::Render()
	{
		OnRender();
		for (auto child : Children)
			child->Render();
	}

	Rectangle& GUIElement::GetScreenRect()
	{
		return ScreenRect;
	}

}