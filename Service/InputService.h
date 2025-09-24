#pragma once
#include "../Core/Common.h"

class InputService
{
public:
	using InputAction = function<void()>;
	using InputKeyCode = uint16;

private:
	struct FKeyBinding
	{
		InputAction action;
		bool wasPressed = false;
	};


public:
	void Update();

	void BindKeyAction( InputKeyCode keyCode , const InputAction& action );
	void BindKeyAction( initializer_list< pair<const InputKeyCode , const InputAction> > actionList );

	void Clear();
	void Clear( InputKeyCode keyCode );


private:
	unordered_map<InputKeyCode , FKeyBinding> m_keyActionMap;

};

