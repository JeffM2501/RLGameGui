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

#include <string>
#include <vector>
#include <memory>

#include "raylib.h"

namespace RLGameGUI
{
	enum class RelativeSizeTypes
	{
        Pixel,
        Percent,
	};

	enum class AxisTypes
	{
		Horizontal,
		Vertical,
	};

	enum class AlignmentTypes
	{
		Minimum,
		Maximum,
		Center
	};

	class RelativeValue
	{
	public:
		RelativeSizeTypes SizeType = RelativeSizeTypes::Pixel;
		float SizeValue = 0;

		AxisTypes AxisType = AxisTypes::Horizontal;

		RelativeValue() {}

		RelativeValue(float value, bool horizontal)
		{
			SizeType = RelativeSizeTypes::Percent;
			SizeValue = value;
			AxisType = horizontal ? AxisTypes::Horizontal : AxisTypes::Vertical;
		}

        RelativeValue(int value, bool horizontal)
        {
            SizeType = RelativeSizeTypes::Pixel;
			SizeValue = (float)value;
            AxisType = horizontal ? AxisTypes::Horizontal : AxisTypes::Vertical;
        }

		float ResolvePos(const Rectangle& parrentScreenRect);
		float ResolveSize(const Rectangle& parrentScreenRect);

		inline bool IsDirty() const { return Dirty; }
		inline void Clean() { Dirty = false; }
		inline void SetDirty() { Dirty = true; }

	protected:
		bool Dirty = false;
	};

	class RelativePoint
	{
	public:
		RelativeValue X;
		RelativeValue Y;

        inline bool IsDirty() const { return X.IsDirty() || Y.IsDirty(); }
		inline void Clean() { X.Clean(); Y.Clean(); }
        inline void SetDirty() { X.SetDirty(); Y.SetDirty(); }

		RelativePoint() : X(0.0f,true), Y(0.0f,false){}
		RelativePoint(float x, float y) : X(x, true), Y(y, false) {}
		RelativePoint(int x, int y) : X(x, true), Y(y, false) {}
        RelativePoint(RelativeValue x, RelativeValue y) : X(x), Y(y) {}

		inline bool IsZero() const { return X.SizeValue == 0 && Y.SizeValue == 0; }

        Vector2 ResolvePos(const Rectangle& parrentScreenRect);
		Vector2 ResolveSize(const Rectangle& parrentScreenRect);
	};

	class RelativeRect
	{
	public:
		RelativePoint Origin;
		RelativePoint Size;

		AlignmentTypes HorizontalAlignment = AlignmentTypes::Minimum;
		AlignmentTypes VerticalAlignment = AlignmentTypes::Minimum;
		Vector2 Offset = { 0,0 };

		RelativeRect() : Origin(0.0f, 0.0f), Size(1.0f, 1.0f) {}
		RelativeRect(RelativePoint origin, RelativePoint size) : Origin(origin), Size(size) {}
		RelativeRect(float x, float y, float width, float height) : Origin(x,y), Size(width,height) {}
		RelativeRect(int x, int y, int width, int height) : Origin(x, y), Size(width, height) {}
		RelativeRect(RelativeValue x, RelativeValue y, RelativeValue width, RelativeValue height) : Origin(x, y), Size(width, height) {}
		RelativeRect(RelativeValue x, RelativeValue y, RelativeValue width, RelativeValue height, AlignmentTypes hAllign, AlignmentTypes vAllign) : Origin(x, y), Size(width, height), HorizontalAlignment(hAllign), VerticalAlignment(vAllign) {}
        RelativeRect(RelativeValue x, RelativeValue y, RelativeValue width, RelativeValue height, AlignmentTypes hAllign, AlignmentTypes vAllign, Vector2 offset) : Origin(x, y), Size(width, height), HorizontalAlignment(hAllign), VerticalAlignment(vAllign), Offset(offset) {}

        inline bool IsDirty() const { return Origin.IsDirty() || Size.IsDirty(); }
        inline void Clean() { Origin.Clean(); Size.Clean(); }
        inline void SetDirty() { Origin.SetDirty(); Size.SetDirty(); }

		Rectangle Resolve(const Rectangle& parrentScreenRect);
	};

	class GUIElement
	{
	public:
		std::string Name;

		void Update();
		void Render();
		void Resize();

		typedef std::shared_ptr<GUIElement> Ptr;

		GUIElement* Parent = nullptr;
		std::vector<GUIElement::Ptr> Children;

		GUIElement::Ptr AddChild(GUIElement::Ptr child);

		RelativeRect RelativeBounds;

		bool Hidden = false;
		bool Disabled = false;

		RelativePoint Padding;

	protected:

		bool Renders = true;

		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnPreResize() {}
		virtual void OnResize() {}
		virtual void OnAddChild(GUIElement::Ptr child) {}

		virtual Rectangle& GetScreenRect();
		virtual Rectangle& GetContentRect();

		Rectangle ScreenRect = { 0,0,0,0 };
		Rectangle ContentRect = { 0,0,0,0 };
	};
}