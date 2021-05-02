#include "MiniginPCH.h"
#include "InputManager.h"
//#include <Windows.h>

dae::InputManager::InputManager()
	:m_Buttons{ ControllerButton::ButtonA,ControllerButton::ButtonB, ControllerButton::ButtonX, ControllerButton::ButtonY,ControllerButton::ButtonUp,
	ControllerButton::ButtonRight, ControllerButton::ButtonDown, ControllerButton::ButtonLeft , ControllerButton::ButtonSelect,ControllerButton::ButtonStart ,
	ControllerButton::ButtonRightThumb,ControllerButton::ButtonLeftThumb,ControllerButton::ButtonRightShoulder,ControllerButton::ButtonLeftShoulder }
	, m_CurrentState{}
{
	//for (size_t i = 0; i < XUSER_MAX_COUNT; i++)
	//{
	//	m_Controllers.push_back(std::make_unique<Controller>());
	//}
}

void dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	// Get the state of the controller from XInput.
	XInputGetState(0, &m_CurrentState);
	//updating triggers
	m_Triggers[0].second = m_CurrentState.Gamepad.bLeftTrigger;
	m_Triggers[1].second = m_CurrentState.Gamepad.bRightTrigger;
	//updating sticks
	m_Sticks[0].second = AnalogStickInput{ (float)m_CurrentState.Gamepad.sThumbLX,(float)m_CurrentState.Gamepad.sThumbLY };
	m_Sticks[1].second = AnalogStickInput{ (float)m_CurrentState.Gamepad.sThumbRX, (float)m_CurrentState.Gamepad.sThumbRY };
}

bool dae::InputManager::IsPressed(ControllerButton button) const
{
	if (m_CurrentState.Gamepad.wButtons & int(button))
	{
		return true;
	}
	return false;
}

void dae::InputManager::BindCommands()
{
	//assign buttons
	AssignKey<DieCommand>(ControllerButton::ButtonA);
	AssignKey<IncreasePointsCommand>(ControllerButton::ButtonB);
	AssignKey<DieCommand>(ControllerButton::ButtonX, 0);
	AssignKey<IncreasePointsCommand>(ControllerButton::ButtonY, 0);
	//move
	AssignKey<JumpUp>(ControllerButton::ButtonUp);
	AssignKey<JumpDown>(ControllerButton::ButtonDown);
	AssignKey<JumpLeft>(ControllerButton::ButtonLeft);
	AssignKey<JumpRight>(ControllerButton::ButtonRight);
	//keyboard
	AssignKey<JumpUp>(KeyboardButton::W);
	AssignKey<JumpDown>(KeyboardButton::S);
	AssignKey<JumpLeft>(KeyboardButton::A);
	AssignKey<JumpRight>(KeyboardButton::D);
	//
	AssignKey<ExitCommand>(ControllerButton::ButtonSelect);
	//AssignKey<FartCommand>(ControllerButton::ButtonStart);
	//AssignKey<FartCommand>(ControllerButton::ButtonLeftThumb);
	//AssignKey<FartCommand>(ControllerButton::ButtonRightThumb);
	//AssignKey<FartCommand>(ControllerButton::ButtonLeftShoulder);
	//AssignKey<FartCommand>(ControllerButton::ButtonRightShoulder);
	//assign triggers
	AssignTrigger<AimCommand>(m_Triggers[0].first);
	AssignTrigger<ShootCommand>(m_Triggers[1].first);
	//assign sticks
	AssignStick<MoveCommand>(m_Sticks[0].first);
	AssignStick<LookCommand>(m_Sticks[1].first);
}
// TODO : change the names of the functions to controller buttons , controller analog sticks and keyboard buttons
bool dae::InputManager::InputHandler()
{
	//const int connectedControllers{ 1 };
	//for (DWORD i = 0; i < connectedControllers; i++)
	{
		for (ControllerButton button : m_Buttons)
		{
			ControllerKey key = std::make_pair(int(button), button);

			if (m_ButtonCommands.find(key) == m_ButtonCommands.end())
				continue;

			const auto& command = m_ButtonCommands.at(key);

			if (command)
			{
				if (!command->GetIsPressed())
				{
					if (IsPressed(button))
					{
						command->Execute();
						command->SetIsPressed(true);
					}
				}
				else
				{
					if (!IsPressed(button))
					{
						command->SetIsPressed(false);
					}
				}
				//check if exited the program
				if (button == ControllerButton::ButtonSelect && IsPressed(button))
				{
					return false;
				}
			}
		}
		return true;
	}
}

void dae::InputManager::ControllerAnalogs()
{
	const int connectedControllers{ 1 };
	for (DWORD i = 0; i < connectedControllers; i++)
	{
		//trigger analogs
		for (ControllerTrigger trigger : m_Triggers)
		{
			if (m_TriggerCommands.find(trigger.first) == m_TriggerCommands.end())
				continue;

			const auto& command = m_TriggerCommands.at(trigger.first);

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
			if (m_StickCommands.find(stick.first) == m_StickCommands.end())
				continue;

			const auto& command = m_StickCommands.at(stick.first);

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
		if (ev.type == SDL_QUIT)
			return false;

		if (ev.type == SDL_KEYDOWN)
		{
			KeyboardKey key = std::make_pair(int(ev.key.keysym.sym), KeyboardButton(int(ev.key.keysym.sym)));
			KeyboardCommandsMap& commandMap = m_KeyboardButtonCommands;

			if (commandMap.find(key) != commandMap.end())
			{
				const auto& command = commandMap.at(key);
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
				const auto& command = commandMap.at(key);
				command->Release();
				command->SetIsPressed(false);
			}
		}
	}

	return true;
}