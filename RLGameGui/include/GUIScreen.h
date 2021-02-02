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

#include "GUIElement.h"
#include "RootElement.h"

namespace RLGameGUI
{
	class GUIScreen
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

		GUIElement::Ptr AddElement(GUIElement::Ptr element);

	protected:
		bool Active = false;
		RootElement Root;
        std::vector<GUIElement::Ptr> Elements;

		void DoResize();

		virtual void OnActivate() {}
		virtual void OnDeactivate() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}

		virtual void OnElementAdd(GUIElement::Ptr element) {}
	};
}