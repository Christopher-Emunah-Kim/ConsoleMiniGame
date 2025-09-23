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

void InputService::ProcessTextInput()
{
	while (_kbhit())
	{
		wchar_t wch = _getwch();

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

void InputService::Clear()
{
	m_inputActionMap.clear();
	m_actionOnError = nullptr;
}

void InputService::Clear(const wstring& command)
{
	m_inputActionMap.erase(command);
}

const wstring& InputService::GetInputBuffer() const
{
	return m_inputBuffer;
}
