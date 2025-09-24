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
		const steady_clock::time_point now = steady_clock::now(); 

		if ( bIsPressed && !binding.wasPressed && binding.action )
		{
			const bool bWasPressed = binding.wasPressed;
			const duration<float> elapsed = now - binding.lastPressedTime;
			constexpr float INPUT_TRIGGER_TIME = 0.1f; //seconds

			if ( !bWasPressed || elapsed.count() >= INPUT_TRIGGER_TIME ) //0.1초에 한번만 반응
			{
				binding.action();
				binding.lastPressedTime = now;
			}

		}

		binding.wasPressed = bIsPressed;

		iter = m_keyActionMap.find( keyCode );
		if(iter != m_keyActionMap.end() )
		{
			iter->second = binding;
		}
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
	binding.lastPressedTime = steady_clock::now();
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
