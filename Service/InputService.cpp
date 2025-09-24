#include "InputService.h"

void InputService::ProcessAction(wstring command)
{
	unordered_map<wstring, InputAction>::iterator iter = m_inputActionMap.find(command);

	if(iter != m_inputActionMap.end())
	{
		iter->second();
	}
	else if(m_actionOnError)
	{
		m_actionOnError();
	}

	m_inputBuffer.clear();
}

bool InputService::ProcessKeyAction( uint16 keyCode )
{
	unordered_map<InputKeyCode, InputAction>::iterator iter = m_keyActionMap.find( keyCode );
	if( iter != m_keyActionMap.end() )
	{
		iter->second();
		return true;
	}
	return false;
}

//Unreal Input Mapping Style(단일키를 정수 ID로 관리)
InputService::InputKeyCode InputService::MakeKeyCode( uint8 prefix , uint8 key )
{
	return static_cast<InputKeyCode>( ( static_cast<uint16>( prefix ) << 8 ) | static_cast<uint16>( key ) );
}

void InputService::ProcessTextInput()
{
	while (_kbhit())
	{
		uint16 key = static_cast<uint8>( _getch() );

		if(key == KEY_PREFIX_NONE || key == KEY_PREFIX_EXTENDED)
		{
			uint16 extendedKey = static_cast<uint8>( _getch() );
			if(ProcessKeyAction( MakeKeyCode( static_cast<uint8>( key ) , static_cast<uint8>( extendedKey ) ) ) )
			{
				continue;
			}
			continue;
		}

		wchar_t wch = static_cast<wchar_t>(key);

		switch (wch)
		{
			case L'\r': //Enter
			{
				ProcessAction(std::move(m_inputBuffer));
			}
			break;
			case L'\b': //Backspace
			{
				if (!m_inputBuffer.empty())
				{
					m_inputBuffer.pop_back();
				}
			}
			break;

			default:
			{
				if ( ProcessKeyAction( static_cast<InputKeyCode>( wch ) ) )
				{
					break;
				}

				m_inputBuffer.push_back(wch);
			}
			break;
		}
	}
}

void InputService::BindActionOnInputError(const InputAction& action)
{
	m_actionOnError = action;
}

void InputService::BindAction(const wstring& command, const InputAction& action)
{
	m_inputActionMap[command] = action;
}

void InputService::BindAction(initializer_list<pair<const wstring, const InputAction>> actionList)
{
	for(initializer_list<pair<const wstring, const InputAction>>::iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
	{
		m_inputActionMap[iter->first] = iter->second;
	}
}

void InputService::BindKeyAction( const InputKeyCode& keyCode , const InputAction& action )
{
	m_keyActionMap[ keyCode ] = action;
}

void InputService::BindKeyAction( initializer_list<pair<const InputKeyCode , const InputAction>> actionList )
{
	for( initializer_list<pair<const InputKeyCode , const InputAction>>::iterator iter = actionList.begin(); iter != actionList.end(); ++iter )
	{
		m_keyActionMap[ iter->first ] = iter->second;
	}
}

void InputService::Clear()
{
	m_inputActionMap.clear();
	m_keyActionMap.clear();
	m_actionOnError = nullptr;
}

void InputService::Clear(const wstring& command)
{
	m_inputActionMap.erase(command);
}

void InputService::Clear( InputKeyCode keyCode )
{
	m_keyActionMap.erase( keyCode );
}

const wstring& InputService::GetInputBuffer() const
{
	return m_inputBuffer;
}
