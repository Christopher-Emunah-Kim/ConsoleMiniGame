#pragma once
#include "../Core/Common.h"

class InputService
{
private:
	void ProcessAction(wstring command);
	bool ProcessKeyAction(uint16 keyCode );

public:
	using InputAction = function<void()>;
	using InputKeyCode = uint16;

	static constexpr uint8 KEY_PREFIX_NONE = 0x00;
	static constexpr uint8 KEY_PREFIX_EXTENDED = 0xE0;

	static InputKeyCode MakeKeyCode( uint8 prefix , uint8 key );

	void ProcessTextInput();

	void BindActionOnInputError(const InputAction& action);
	void BindAction(const wstring& command, const InputAction& action);
	void BindAction(initializer_list<pair<const wstring, const InputAction>> actionList);

	void BindKeyAction( const InputKeyCode& keyCode , const InputAction& action );
	void BindKeyAction( initializer_list< pair<const InputKeyCode , const InputAction> > actionList );

	void Clear();
	void Clear(const wstring& command);
	void Clear( InputKeyCode keyCode );

	const wstring& GetInputBuffer() const;

private:
	wstring m_inputBuffer;
	unordered_map<wstring, InputAction> m_inputActionMap;
	unordered_map<InputKeyCode , InputAction> m_keyActionMap;
	InputAction m_actionOnError;
};

