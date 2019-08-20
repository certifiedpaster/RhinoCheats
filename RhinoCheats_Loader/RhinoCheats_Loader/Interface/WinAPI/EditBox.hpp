#pragma once

#include "Control.hpp"

namespace ctrl
{
	class cEditBox : public cControl
	{
	public:

		cEditBox(HWND hwnd = NULL)
			: cControl(hwnd) {  }

		long Integer()
		{
			return strtol(Text().c_str(), nullptr, 10);
		}

		std::string Text()
		{
			char buf[512] = { NULL };
			Edit_GetText(_hwnd, buf, ARRAYSIZE(buf));

			return buf;
		}

		BOOL Text(const std::string& text)
		{
			return Edit_SetText(_hwnd, text.c_str());
		}

		BOOL Reset()
		{
			return Edit_SetText(_hwnd, "");
		}
	};
}
