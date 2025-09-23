#pragma once
#include "../Core/Common.h"

using InputAction = function<void()>;

class InputService
{
private:
	void ProcessAction(wstring command);

public:
	void ProcessTextInput();

	void BindActionOnInputError(const InputAction& action);
	void BindAction(const wstring& command, const InputAction& action);
	void BindAction(initializer_list<pair<const wstring, const InputAction>> actionList);

	void Clear();
	void Clear(const wstring& command);

	const wstring& GetInputBuffer() const;

private:
	wstring m_inputBuffer;
	unordered_map<wstring, InputAction> m_inputActionMap;
	InputAction m_actionOnError;
};

