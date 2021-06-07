#include "pch.h"
#include "InputManager.h"
//#include <Windows.h>

dae::InputManager::InputManager()
	:m_Buttons{ 
	ControllerButton::ButtonA,
	ControllerButton::ButtonB, 
	ControllerButton::ButtonX, 
	ControllerButton::ButtonY,
	ControllerButton::ButtonUp,
	ControllerButton::ButtonRight, 
	ControllerButton::ButtonDown, 
	ControllerButton::ButtonLeft , 
	ControllerButton::ButtonSelect,
	ControllerButton::ButtonStart ,
	ControllerButton::ButtonRightThumb,
	ControllerButton::ButtonLeftThumb,
	ControllerButton::ButtonRightShoulder,
	ControllerButton::ButtonLeftShoulder }
	,m_CurrentState{}
{
	for (size_t i = 0; i < XUSER_MAX_COUNT; i++) m_Controllers.push_back(std::make_unique<Controller>());
}

void dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	XInputGetState(0, &m_CurrentState);

	m_Triggers[0].second = m_CurrentState.Gamepad.bLeftTrigger;
	m_Triggers[1].second = m_CurrentState.Gamepad.bRightTrigger;

	m_Sticks[0].second = AnalogStickInput{ (float)m_CurrentState.Gamepad.sThumbLX,(float)m_CurrentState.Gamepad.sThumbLY };
	m_Sticks[1].second = AnalogStickInput{ (float)m_CurrentState.Gamepad.sThumbRX, (float)m_CurrentState.Gamepad.sThumbRY };
}

bool dae::InputManager::IsPressed(ControllerButton button, const  std::unique_ptr<Controller>& controller) const
{
	if (controller->m_CurrentState.Gamepad.wButtons & int(button)) return true;
	return false;
}

bool dae::InputManager::InputHandler()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&(m_Controllers[i]->m_CurrentState), sizeof(XINPUT_STATE));
		dwResult = XInputGetState(i, &(m_Controllers[i]->m_CurrentState));

		for (ControllerButton button : m_Buttons)
		{
			ControllerKey key = std::make_pair(int(button), button);

			if (m_Controllers[i]->m_ConsoleButtonCommands.find(key) == m_Controllers[i]->m_ConsoleButtonCommands.end())
				continue;

			const auto& command = m_Controllers[i]->m_ConsoleButtonCommands.at(key).first;

			if (!command->GetIsPressed())
			{
				if (IsPressed(button, m_Controllers[i]))
				{
					command->Execute();
					command->SetIsPressed(true);
				}
			}
			else
			{
				if (!IsPressed(button, m_Controllers[i]))
				{
					command->Release();
					command->SetIsPressed(false);
				}
			}
			//if (button == ControllerButton::ButtonSelect && IsPressed(button, m_Controllers[i]))
			//{
			//	return false;
			//}
		}
	}
	return true;
}

void dae::InputManager::ControllerAnalogs()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&(m_Controllers[i]->m_CurrentState), sizeof(XINPUT_STATE));
		dwResult = XInputGetState(i, &(m_Controllers[i]->m_CurrentState));
		//trigger analogs
		for (ControllerTrigger trigger : m_Triggers)
		{
			if (m_Controllers[i]->m_ConsoleTriggerCommands.find(trigger.first) == m_Controllers[i]->m_ConsoleTriggerCommands.end())
				continue;

			const auto& command = m_Controllers[i]->m_ConsoleTriggerCommands.at(trigger.first).first;

			if (command)
			{
				if (trigger.second != 0)
				{
					command->SetTriggerValue(trigger.second);
					command->Execute();
				}
			}
		}
		//stick analogs
		for (ControllerStick stick : m_Sticks)
		{
			if (m_Controllers[i]->m_ConsoleStickCommands.find(stick.first) == m_Controllers[i]->m_ConsoleStickCommands.end())
				continue;

			const auto& command = m_Controllers[i]->m_ConsoleStickCommands.at(stick.first).first;

			if (command)
			{
				if (stick.second.x != 0.f || stick.second.y != 0.f)
				{
					command->SetAnalogStickValue(stick.second);
					command->Execute();
				}
			}
		}
	}
}

bool dae::InputManager::KeyboardInput()
{
	SDL_Event ev;

	if (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT || m_IsExiting)
			return false;

		if (ev.type == SDL_KEYDOWN)
		{
			KeyboardKey key = std::make_pair(int(ev.key.keysym.sym), KeyboardButton(int(ev.key.keysym.sym)));
			KeyboardCommandsMap& commandMap = m_KeyboardButtonCommands;

			if (commandMap.find(key) != commandMap.end())
			{
				const auto& command = commandMap.at(key).first;
				command->Execute();
				command->SetIsPressed(true);
			}
		}
		if (ev.type == SDL_KEYUP)
		{
			KeyboardKey key = std::make_pair(int(ev.key.keysym.sym), KeyboardButton(int(ev.key.keysym.sym)));
			KeyboardCommandsMap& commandMap = m_KeyboardButtonCommands;

			if (commandMap.find(key) != commandMap.end())
			{
				const auto& command = commandMap.at(key).first;
				command->Release();
				command->SetIsPressed(false);
			}
		}
	}

	return true;
}