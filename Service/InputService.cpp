#include "InputService.h"

void InputService::Update()
{
	vector<InputKeyCode> keyList;

	keyList.reserve( 256 );

	for ( const auto& entry : m_keyActionMap )
	{
		keyList.push_back( entry.first );
	}

	for ( const InputKeyCode keyCode : keyList )
	{
		unordered_map<InputKeyCode , FKeyBinding>::iterator iter = m_keyActionMap.find( keyCode );
		if ( iter == m_keyActionMap.end() )
		{
			continue;
		}

		FKeyBinding binding = iter->second;

		const SHORT keyState = GetAsyncKeyState( static_cast<int>(keyCode) );
		const bool bIsPressed = ( keyState & 0x8000 ) != 0;	//MSB is 1 when the key is currently pressed

		if ( bIsPressed && !binding.wasPressed && binding.action )
		{
			binding.action();
		}

		binding.wasPressed = bIsPressed;
	}
}

void InputService::BindKeyAction( InputKeyCode keyCode , const InputAction& action )
{
	if ( !action )
	{
		m_keyActionMap.erase( keyCode );
		return;
	}

	FKeyBinding& binding = m_keyActionMap[ keyCode ];
	binding.action = action;
	binding.wasPressed = false;
}

void InputService::BindKeyAction( initializer_list< pair<const InputKeyCode , const InputAction> > actionList )
{
	for( initializer_list<pair<const InputKeyCode , const InputAction>>::iterator iter = actionList.begin(); iter != actionList.end(); ++iter )
	{
		BindKeyAction( iter->first , iter->second );
	}
}

void InputService::Clear()
{
	m_keyActionMap.clear();
}

void InputService::Clear( InputKeyCode keyCode )
{
	m_keyActionMap.erase( keyCode );
}
