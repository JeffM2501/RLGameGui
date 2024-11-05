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

#include <string>
#include <vector>
#include <memory>
#include <functional>


#include "raylib.h"
#include "rapidjson/document.h"

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
		Minimum = 0,
		Maximum = 1,
		Center = 2
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

        RelativeValue(int value, bool horizontal = true)
        {
            SizeType = RelativeSizeTypes::Pixel;
			SizeValue = (float)value;
            AxisType = horizontal ? AxisTypes::Horizontal : AxisTypes::Vertical;
        }

		float ResolvePos(const Rectangle& parrentScreenRect, bool isXAxis);
		float ResolveSize(const Rectangle& parrentScreenRect);

		inline bool IsDirty() const { return Dirty; }
		inline void Clean() { Dirty = false; }
		inline void SetDirty() { Dirty = true; }

        virtual bool Read(const rapidjson::Value& object, rapidjson::Document& document);
        virtual bool Write(rapidjson::Value& object, rapidjson::Document& document);

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
	
        virtual bool Read(const rapidjson::Value& object, rapidjson::Document& document);
        virtual bool Write(rapidjson::Value& object, rapidjson::Document& document);
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

        virtual bool Read(const rapidjson::Value& object, rapidjson::Document& document);
        virtual bool Write(rapidjson::Value& object, rapidjson::Document& document);
	};

	enum class GUIElementEvent
	{
		None,
		Hover,
		Click,
		Changed,
	};

#define DEFINE_ELEMENT(T) \
	static inline const char* TypeName() {return #T;} \
	inline const char* GetTypeName() const override { return #T; } \
	static inline void Register()  \
	{ \
		GUIElementFactory::Register(#T, []() \
			{\
				return std::make_shared<T>(); \
			});\
	}

	class GUIElement
	{
	public:
		virtual ~GUIElement() = default;

		std::string Name;

		std::string Id;

		virtual const char* GetTypeName() const { return nullptr; }

		void Update(Vector2 mousePosition);
		void Render();
		void Resize();

		typedef std::shared_ptr<GUIElement> Ptr;
		typedef std::function<void(GUIElement*)> Function;

		GUIElement* Parent = nullptr;
		std::vector<GUIElement::Ptr> Children;

		virtual GUIElement::Ptr AddChild(GUIElement::Ptr child);

		RelativeRect RelativeBounds;

		bool Hidden = false;
		bool Disabled = false;

		RelativePoint Padding;

		Function ElementClicked = nullptr;

		virtual bool Read(const rapidjson::Value& object, rapidjson::Document& document);
		virtual bool Write(rapidjson::Value& object, rapidjson::Document& document);
		
		GUIElement* FindElement(const std::string& id);

		template<class T>
		T* FindElement(const std::string& id)
		{
			if (id == Id && T::TypeName() == GetTypeName())
				return (T*)this;

            for (auto& child : Children)
            {
                T* element = child->FindElement<T>(id);
                if (element)
                    return element;
            }

            return nullptr;
		}

        virtual const Rectangle& GetScreenRect();
        virtual const Rectangle& GetContentRect();

	protected:

		bool Renders = true;
		
		bool Hovered = false;
		bool Clicked = false;

		virtual void OnPreUpdate() {}
		virtual void OnUpdate() {}
        virtual void OnPostChildUpdate() {}
		virtual void OnRender() {}
		virtual void OnPreResize() {}
		virtual void OnResize() {}
		virtual void OnAddChild(GUIElement::Ptr child) {}

		virtual void OnHoverStart() {}
		virtual void OnHoverEnd() {}

		virtual void OnClickStart() {}
		virtual void OnClickEnd() {}
		virtual void OnClickCancel() {}

		virtual void PostEvent(GUIElement* element, GUIElementEvent eventType, void* data);

		Rectangle ScreenRect = { 0,0,0,0 };
		Rectangle ContentRect = { 0,0,0,0 };
	};
}