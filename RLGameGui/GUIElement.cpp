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

        if (RelativeBounds.IsDirty())
            Resize();
	}

	void GUIElement::Resize()
	{
		OnPreResize();

		Vector2 padding = { 0,0 };
		if (Parent != nullptr)
		{
			ScreenRect = RelativeBounds.Resolve(Parent->GetContentRect());
			padding = Padding.ResolveSize(Parent->GetContentRect());
		}

		ContentRect = ScreenRect;
		ContentRect.x += padding.x;
		ContentRect.y += padding.y;
		ContentRect.width -= padding.x * 2;
		ContentRect.height -= padding.y * 2;

        OnResize();
        for (auto child : Children)
            child->Resize();
	}

    GUIElement::Ptr GUIElement::AddChild(GUIElement::Ptr child)
    {
		child->Parent = this;
		Children.emplace_back(child);
		return child;
    }

    void GUIElement::Render()
	{
		if (Hidden)
			return;

		if (Renders)
			OnRender();

		for (auto child : Children)
			child->Render();
	}

	Rectangle& GUIElement::GetScreenRect()
	{
        if (RelativeBounds.IsDirty())
            Resize();

		return ScreenRect;
	}

	Rectangle& GUIElement::GetContentRect()
	{
        if (RelativeBounds.IsDirty())
            Resize();

        return ContentRect;
	}

	float RelativeValue::ResolvePos(const Rectangle& parrentScreenRect)
	{
		Clean();

		float origin = AxisType == AxisTypes::Horizontal ? parrentScreenRect.x : parrentScreenRect.y;
		float size = AxisType == AxisTypes::Horizontal ? parrentScreenRect.width : parrentScreenRect.height;

		float pixelValue = SizeValue;
		if (SizeType == RelativeSizeTypes::Percent)
			pixelValue *= size;

		pixelValue += origin;

		return pixelValue;
	}

	float RelativeValue::ResolveSize(const Rectangle& parrentScreenRect)
    {
		Clean();

        float size = AxisType == AxisTypes::Horizontal ? parrentScreenRect.width : parrentScreenRect.height;

        float pixelValue = SizeValue;
        if (SizeType == RelativeSizeTypes::Percent)
            pixelValue *= size;

        return pixelValue;
    }

	Vector2 RelativePoint::ResolvePos(const Rectangle& parent)
	{
		Clean();
		return Vector2{ X.ResolvePos(parent), Y.ResolvePos(parent) };
	}

	Vector2 RelativePoint::ResolveSize(const Rectangle& parent)
	{
		Clean();
		return Vector2{ X.ResolveSize(parent), Y.ResolveSize(parent) };
	}

	float GetAllginedValue(float value, AlignmentTypes Alignment, float size, float offset)
	{
		if (Alignment == AlignmentTypes::Maximum)
		{
			value -= size;
			value -= offset;
		}
		else if (Alignment == AlignmentTypes::Center)
		{
			value -= size * 0.5f;
			value += offset;
		}
		else
			value += offset;

		return value;
	}

    Rectangle RelativeRect::Resolve(const Rectangle& parent)
    {
		Vector2 pixelSize = Size.ResolveSize(parent);
		Vector2 pixelOrigin = Origin.ResolvePos(parent);

		pixelOrigin.x = GetAllginedValue(pixelOrigin.x, HorizontalAlignment, pixelSize.x, Offset.x);
		pixelOrigin.y = GetAllginedValue(pixelOrigin.y, VerticalAlignment, pixelSize.y, Offset.y);

		return Rectangle{ pixelOrigin.x, pixelOrigin.y, pixelSize.x, pixelSize.y };
    }
}