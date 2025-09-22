#pragma once
#include "../Core/Common.h"

using InputAction = function<void()>;

class InputService
{


private:
	wstring m_inputBuffer;
	unordered_map<wstring, InputAction> m_inputActionMap;
	InputAction m_actionOnError;
};

