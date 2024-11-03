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

#include "GUIElement.h"
#include "RootElement.h"

namespace RLGameGUI
{
	using EventHandler = std::function<void(GUIElement&, GUIElementEvent,void*)>;

	class GUIScreen : public RootElement
	{
	public:
		std::string Name;

		bool IsActive() const;

		void Activate();
		void Deactivate();

		void Update();
		void Render();

		typedef std::shared_ptr<GUIScreen> Ptr;
		inline static Ptr Create() { return std::make_shared<GUIScreen>(); }

		GUIElement::Ptr AddChild(GUIElement::Ptr element) override { return AddElement(element); }
		GUIElement::Ptr AddElement(GUIElement::Ptr element);

		typedef std::function<void()> ScreenEventCallback;

		inline void AddPostRenderCallback(ScreenEventCallback callback) { PostRenderCallbacks.emplace_back(callback); }

		void RegisterEventHandler(const std::string& elmentId, GUIElementEvent eventType, EventHandler handler);

	protected:
		bool Active = false;

		void DoResize();

		virtual void OnActivate() {}
		virtual void OnDeactivate() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}

		virtual void OnElementAdd(GUIElement::Ptr element) {}

        void PostEvent(GUIElement* element, GUIElementEvent eventType, void* data) override;

		std::vector<ScreenEventCallback> PostRenderCallbacks;

		struct EventHandlerInfo
		{
			GUIElementEvent EventType;
			EventHandler	Handler;
		};
		std::unordered_map<std::string, std::vector<EventHandlerInfo>> EventHandlers;
	};
}