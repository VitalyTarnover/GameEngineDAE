#pragma once

#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include "BaseCommand.h"
#include "BaseAnalogStickCommand.h"
#include "BaseAnalogTriggerCommand.h"
#include <vector>
#include <map>
#include "Singleton.h"
#include <SDL.h>

#pragma comment(lib, "XInput.lib")

namespace dae
{
	enum class KeyboardButton
	{
		W = SDLK_w,
		A = SDLK_a,
		S = SDLK_s,
		D = SDLK_d,
		I = SDLK_i,
		J = SDLK_j,
		K = SDLK_k,
		L = SDLK_l,
		P = SDLK_p,
		One = SDLK_1,
		Two = SDLK_2,
		Three = SDLK_3,
		SPACE = SDLK_SPACE,
		ESC = SDLK_ESCAPE
	};

	enum class ControllerButton
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		ButtonSelect = XINPUT_GAMEPAD_BACK,
		ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
		ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
		ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		ButtonLeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		ButtonRightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		ButtonLeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
		ButtonRightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		ButtonStart = XINPUT_GAMEPAD_START,
	};
	//----------------------------------------------------------------------------------------------
	//for buttons
	using ControllerKey = std::pair<unsigned, ControllerButton>;
	using ControllerButtonCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
	//for triggers
	using ControllerTrigger = std::pair<std::string, float>;
	using ControllerTriggerCommandMap = std::map<std::string, std::unique_ptr<AnalogTriggerCommand>>;
	//for sticks
	using ControllerStick = std::pair<std::string, AnalogStickInput>;
	using ControllerStickCommandMap = std::map<std::string, std::unique_ptr<AnalogStickCommand>>;
	//for keyboard
	using KeyboardKey = std::pair<unsigned, KeyboardButton>;
	using KeyboardCommandsMap = std::map<KeyboardKey, std::unique_ptr<Command>>;
	//----------------------------------------------------------------------------------------------
	struct Controller //TODO : use this instead of the three seperatly
	{
		ControllerButtonCommandsMap m_ConsoleButtonCommands;
		ControllerTriggerCommandMap m_ConsoleTriggerCommands;
		ControllerStickCommandMap m_ConsoleStickCommands;
		XINPUT_STATE m_CurrentState{};
	};
	struct Keyboard //TODO : this as well like above
	{
		KeyboardCommandsMap m_ConsoleCommands;
	};

	//----------------------------------------------------------------------------------------------
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		//Functions
		void ProcessInput();
		bool KeyboardInput();
		bool InputHandler();
		void ControllerAnalogs();
		//void BindCommands();

		//----------------------------------------------------------------------------------------------
		//button template
		template <typename T>
		void AssignKey(ControllerButton button, int controllerIndex = 0)
		{
			ControllerKey key = std::make_pair(unsigned(button), button);
			m_ButtonCommands.insert(std::make_pair(key, std::make_unique<T>(controllerIndex)));
		}
		template <typename T>
		void AssignKey(ControllerButton button, std::unique_ptr<T> command)//untested
		{
			ControllerKey key = std::make_pair(unsigned(button), button);
			m_ButtonCommands.insert(std::make_pair(key, std::move(command)));
		}
		//trigger template
		template <typename T>
		void AssignTrigger(std::string triggerName, int controllerIndex = 0)
		{
			m_TriggerCommands.insert(std::make_pair(triggerName, std::make_unique<T>(controllerIndex)));
		}
		//stick template
		template <typename T>
		void AssignStick(std::string stickName, int controllerIndex = 0)
		{
			m_StickCommands.insert(std::make_pair(stickName, std::make_unique<T>(controllerIndex)));
		}
		//keyboard template
		template <typename T>
		void AssignKey(KeyboardButton button, int keyboardParameter = 0)
		{
			KeyboardKey key = std::make_pair(unsigned(button), button);
			m_KeyboardButtonCommands.insert(std::make_pair(key, std::make_unique<T>(keyboardParameter)));
		}
		//----------------------------------------------------------------------------------------------

		void SetExiting(bool exit) { m_Exiting = exit; };

	private:
		//Methods
		bool IsPressed(ControllerButton button) const;

		bool m_Exiting = false;

		//Data Members
		XINPUT_STATE m_CurrentState;
		//std::vector<std::unique_ptr<Controller>> m_Controllers;
		//for buttons
		ControllerButtonCommandsMap m_ButtonCommands;
		static const int amountOfButtons = 14;
		const ControllerButton m_Buttons[amountOfButtons];
		//for triggers
		ControllerTriggerCommandMap m_TriggerCommands;
		static const int amountOfTriggers = 2;
		ControllerTrigger m_Triggers[amountOfTriggers] = { std::make_pair("LeftTrigger" , 0.f),std::make_pair("RightTrigger" , 0.f) };
		//for sticks
		ControllerStickCommandMap m_StickCommands;
		static const int amountOfAnalogSticks = 2;
		ControllerStick m_Sticks[amountOfAnalogSticks] = { std::make_pair("LeftStick" , AnalogStickInput{0.f,0.f}),std::make_pair("RightStick" , AnalogStickInput{0.f,0.f}) };
		//for keyboard
		KeyboardCommandsMap m_KeyboardButtonCommands;
		//controllers
		//std::vector<std::unique_ptr<Controller>> m_Controllers;
	};
}
