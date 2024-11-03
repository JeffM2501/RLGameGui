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

#include "GUIScreen.h"
#include "raylib.h"

namespace RLGameGUI
{
	bool GUIScreen::IsActive() const
	{
		return Active;
	}

	void GUIScreen::Activate()
	{
		Active = true;
		DoResize();
		OnActivate();
	}

	void GUIScreen::Deactivate()
	{
		Active = false;
		OnDeactivate();
	}

	void GUIScreen::DoResize()
    {
        Resize();
        for (auto child : Children)
            child->Resize();
	}

	void GUIScreen::Update()
	{
		PostRenderCallbacks.clear();

		if (IsWindowResized())
			DoResize();

		// do input
		Vector2 mouse = GetMousePosition();

		// let everyone think
		for (auto child : Children)
			child->Update(mouse);
	}

	void GUIScreen::Render()
	{
		OnRender();
        for (auto child : Children)
            child->Render();

		for (auto& callback : PostRenderCallbacks)
			callback();
	}

    GUIElement::Ptr GUIScreen::AddElement(GUIElement::Ptr element)
    {
		element->Parent = this;
		Children.emplace_back(element);
		OnElementAdd(element);

		return element;
    }

    void GUIScreen::RegisterEventHandler(const std::string& elementId, GUIElementEvent eventType, EventHandler handler)
    {
		auto itr = EventHandlers.find(elementId);
		if (itr == EventHandlers.end())
			itr = EventHandlers.insert_or_assign(elementId, std::vector<EventHandlerInfo>()).first;

		EventHandlerInfo info;
		info.EventType = eventType;
		info.Handler = handler;
		itr->second.push_back(info);
    }

    void GUIScreen::PostEvent(GUIElement* element, GUIElementEvent eventType, void* data)
	{
		if (!element)
			return;

		auto itr = EventHandlers.find(element->Id);
		if (itr == EventHandlers.end())
			return;

		for (auto handler : itr->second)
		{
			if (handler.EventType == eventType)
				handler.Handler(*element, eventType, data);
		}
	}
}