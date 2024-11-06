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

#include "GUIElement.h"
#include "GUIScreenIO.h"

using namespace rapidjson;

namespace RLGameGUI
{
	void GUIElement::Update(Vector2 mousePostion)
	{
		OnPreUpdate();

        if (RelativeBounds.IsDirty())
            Resize();

		if (Disabled)
			mousePostion.x = -1;

		if (mousePostion.x < 0 || !CheckCollisionPointRec(mousePostion, GetScreenRect()))
		{
			if (Hovered)
			{
				OnHoverEnd();
				PostEvent(this, GUIElementEvent::Hover, (void*)(0));
			}
			Hovered = false;

			if (Clicked)
				OnClickCancel();
			Clicked = false;
		}
		else
		{
			if (!Hovered)
			{
				Hovered = true;
				OnHoverStart();
				PostEvent(this, GUIElementEvent::Hover, (void*)(1));
			}

			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				if (!Clicked)
				{
					Clicked = true;
					OnClickStart();
				}
			}
			else if (Clicked)
			{
				Clicked = false;
				OnClickEnd();
				PostEvent(this, GUIElementEvent::Click, nullptr);
				if (ElementClicked != nullptr)
					ElementClicked(this);
			}
		}

		OnUpdate();
        if (RelativeBounds.IsDirty())
            Resize();

		for (auto child : Children)
			child->Update(mousePostion);

		OnPostChildUpdate();
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

		if (Renders)
			OnPostRender();
	}

	const Rectangle& GUIElement::GetScreenRect()
	{
        if (RelativeBounds.IsDirty())
            Resize();

		return ScreenRect;
	}

	const Rectangle& GUIElement::GetContentRect()
	{
        if (RelativeBounds.IsDirty())
            Resize();

        return ContentRect;
	}

	void GUIElement::PostEvent(GUIElement * element, GUIElementEvent eventType, void* data)
	{
		if (Parent)
			Parent->PostEvent(element, eventType, data);
	}

	bool GUIElement::Read(const rapidjson::Value& object, rapidjson::Document& document)
	{
		auto name = object.FindMember("name");
		if (name != object.MemberEnd() && name->value.IsString())
			Name = name->value.GetString();
        
		auto id = object.FindMember("id");
        if (id != object.MemberEnd() && id->value.IsString())
            Id = id->value.GetString();

		auto hidden = object.FindMember("hidden");
		if (hidden != object.MemberEnd() && hidden->value.IsBool())
			Hidden = hidden->value.GetBool();

        auto disabled = object.FindMember("disabled");
        if (disabled != object.MemberEnd() && disabled->value.IsBool())
			Disabled = disabled->value.GetBool();

        auto bounds = object.FindMember("relative_bounds");
		if (bounds != object.MemberEnd() && bounds->value.IsObject())
			RelativeBounds.Read(bounds->value, document);

        auto padding = object.FindMember("padding");
        if (padding != object.MemberEnd() && padding->value.IsObject())
            Padding.Read(padding->value, document);

		return true;
	}

    bool GUIElement::Write(rapidjson::Value& object, rapidjson::Document& document)
	{
		auto& alloc = document.GetAllocator();

		object.AddMember("name", Value(Name.c_str(), alloc), alloc);
		object.AddMember("id", Value(Id.c_str(), alloc), alloc);
		object.AddMember("hidden", Hidden, alloc);
		object.AddMember("disabled", Disabled, alloc);

		Value bounds(kObjectType);
		RelativeBounds.Write(bounds, document);
        object.AddMember("relative_bounds", bounds, alloc);

		Value padding(kObjectType);
		Padding.Write(padding, document);
        object.AddMember("padding", padding, alloc);

		return true;
	}

	GUIElement* GUIElement::FindElement(const std::string& id)
	{
		if (id == Id)
			return this;

		for (auto& child : Children)
		{
			auto* element = child->FindElement(id);
			if (element)
				return element;
		}

		return nullptr;
	}

	float RelativeValue::ResolvePos(const Rectangle& parrentScreenRect, bool isXAis)
	{
		Clean();

		float origin = isXAis ? parrentScreenRect.x : parrentScreenRect.y;
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

	bool RelativeValue::Read(const rapidjson::Value& object, rapidjson::Document& document)
	{
		auto percent = object.FindMember("percent");
		if (percent != object.MemberEnd() && percent->value.IsBool())
			SizeType = percent->value.GetBool() ? RelativeSizeTypes::Percent : RelativeSizeTypes::Pixel;

        auto horizontal = object.FindMember("horizontal");
        if (horizontal != object.MemberEnd() && horizontal->value.IsBool())
			AxisType = horizontal->value.GetBool() ? AxisTypes::Horizontal : AxisTypes::Vertical;

        auto value = object.FindMember("value");
        if (value != object.MemberEnd() && value->value.IsFloat())
			SizeValue = value->value.GetFloat() ;

		return true;
	}

	bool RelativeValue::Write(rapidjson::Value& object, rapidjson::Document& document)
	{
        auto& alloc = document.GetAllocator();

		object.AddMember("percent", SizeType == RelativeSizeTypes::Percent, alloc);
		object.AddMember("horizontal", AxisType == AxisTypes::Horizontal, alloc);
		object.AddMember("value", SizeValue, alloc);

		return true;
	}

	Vector2 RelativePoint::ResolvePos(const Rectangle& parent)
	{
		Clean();
		return Vector2{ X.ResolvePos(parent, true), Y.ResolvePos(parent, false) };
	}

	Vector2 RelativePoint::ResolveSize(const Rectangle& parent)
	{
		Clean();
		return Vector2{ X.ResolveSize(parent), Y.ResolveSize(parent) };
	}

    bool RelativePoint::Read(const rapidjson::Value& object, rapidjson::Document& document)
    {
        auto x = object.FindMember("x");
		if (x != object.MemberEnd() && x->value.IsObject())
			X.Read(x->value, document);

        auto y = object.FindMember("y");
        if (y != object.MemberEnd() && y->value.IsObject())
            Y.Read(y->value, document);

		return true;
    }

    bool RelativePoint::Write(rapidjson::Value& object, rapidjson::Document& document)
    {
        auto& alloc = document.GetAllocator();

		Value x(Type::kObjectType);
		X.Write(x, document);
		object.AddMember("x", x, alloc);
		
		Value y(Type::kObjectType);
		Y.Write(y, document);
		object.AddMember("y", y, alloc);
		return true;
    }

	float GetAllginedValue(float value, AlignmentTypes Alignment, float size, float parrentSize, float offset)
	{
		if (Alignment == AlignmentTypes::Maximum)
		{
			value += parrentSize;
			value -= size;
			value -= offset;
		}
		else if (Alignment == AlignmentTypes::Center)
		{
			value += parrentSize * 0.5f;
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

		pixelOrigin.x = GetAllginedValue(pixelOrigin.x, HorizontalAlignment, pixelSize.x, parent.width, Offset.x);
		pixelOrigin.y = GetAllginedValue(pixelOrigin.y, VerticalAlignment, pixelSize.y, parent.height, Offset.y);

		return Rectangle{ pixelOrigin.x, pixelOrigin.y, pixelSize.x, pixelSize.y };
    }

    bool RelativeRect::Read(const rapidjson::Value& object, rapidjson::Document& document)
    {
        auto& alloc = document.GetAllocator();

        auto origin = object.FindMember("origin");
		if (origin != object.MemberEnd() && origin->value.IsObject())
		{
			Origin.Read(origin->value, document);
		}

        auto size = object.FindMember("size");
		if (size != object.MemberEnd() && size->value.IsObject())
		{
			Size.Read(size->value, document);
		}

		GUIScreenReader::ReadVector2(object, "offset", Offset);

		GUIScreenReader::ReadAllignmentType(object, "horizontal_allignment", HorizontalAlignment);
        GUIScreenReader::ReadAllignmentType(object, "vertical_allignment", VerticalAlignment);

        return true;
    }

    bool RelativeRect::Write(rapidjson::Value& object, rapidjson::Document& document)
    {
        auto& alloc = document.GetAllocator();

        Value origin(Type::kObjectType);
        Origin.Write(origin, document);

        Value size(Type::kObjectType);
        Size.Write(size, document);

        object.AddMember("origin", origin, alloc);
        object.AddMember("size", size, alloc);

		Value offset(Type::kObjectType);
		offset.AddMember("x", Offset.x, alloc);
		offset.AddMember("y", Offset.y, alloc);
		object.AddMember("offset", offset, alloc);
		
		GUIScreenWriter::WriteAllignmentType(object, "horizontal_allignment", HorizontalAlignment, document);
		GUIScreenWriter::WriteAllignmentType(object, "vertical_allignment", VerticalAlignment, document);
		
		return true;
	}
}