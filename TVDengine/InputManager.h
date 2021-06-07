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

	//buttons
	using ControllerKey = std::pair<unsigned, ControllerButton>;
	using ControllerButtonCommandsMap = std::map<ControllerKey, std::pair<std::unique_ptr<Command>, int>>;
	
	//triggers
	using ControllerTrigger = std::pair<std::string, float>;
	using ControllerTriggerCommandMap = std::map<std::string, std::pair<std::unique_ptr<AnalogTriggerCommand>, int>>;
	
	//sticks
	using ControllerStick = std::pair<std::string, AnalogStickInput>;
	using ControllerStickCommandMap = std::map<std::string, std::pair<std::unique_ptr<AnalogStickCommand>, int>>;
	
	//keyboard
	using KeyboardKey = std::pair<unsigned, KeyboardButton>;
	using KeyboardCommandsMap = std::map<KeyboardKey, std::pair<std::unique_ptr<Command>, int>>;
	
	struct Controller 
	{
		ControllerButtonCommandsMap m_ConsoleButtonCommands;
		ControllerTriggerCommandMap m_ConsoleTriggerCommands;
		ControllerStickCommandMap m_ConsoleStickCommands;
		XINPUT_STATE m_CurrentState{};
	};
	struct Keyboard 
	{
		KeyboardCommandsMap m_ConsoleCommands;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		void ProcessInput();
		
		bool KeyboardInput();
		
		bool InputHandler();
		
		void ControllerAnalogs();
		

		//button template
		template <typename T>
		void AssignKey(ControllerButton button, int controllerIndex, int param = 0)
		{
			ControllerKey key = std::make_pair(unsigned(button), button);
			m_Controllers[controllerIndex]->m_ConsoleButtonCommands.insert(std::make_pair(key, std::make_pair(std::make_unique<T>(controllerIndex), param)));
		}
		
		//trigger template
		template <typename T>
		void AssignTrigger(std::string triggerName, int controllerIndex, int param = 0)
		{
			m_Controllers[controllerIndex]->m_ConsoleTriggerCommands.insert(std::make_pair(triggerName, std::make_pair(std::make_unique<T>(controllerIndex), param)));
		}

		//stick template
		template <typename T>
		void AssignStick(std::string stickName, int controllerIndex, int param = 0)
		{
			m_Controllers[controllerIndex]->m_ConsoleStickCommands.insert(std::make_pair(stickName, std::make_pair(std::make_unique<T>(controllerIndex), param)));
		}

		//keyboard template
		template <typename T>
		void AssignKey(KeyboardButton button, int keyboardParameter = 0)
		{
			KeyboardKey key = std::make_pair(unsigned(button), button);
			m_KeyboardButtonCommands.insert(std::make_pair(key, std::make_pair(std::make_unique<T>(keyboardParameter), 0)));
		}

		void SetIsExiting(bool exit) { m_IsExiting = exit; }

	private:
		bool IsPressed(ControllerButton button, const std::unique_ptr<Controller>& controller) const;

		XINPUT_STATE m_CurrentState;
		
		bool m_IsExiting = false;

		//buttons
		static const int amountOfButtons = 14;
		const ControllerButton m_Buttons[amountOfButtons];
		//triggers
		static const int amountOfTriggers = 2;
		ControllerTrigger m_Triggers[amountOfTriggers] = { std::make_pair("LeftTrigger" , 0.f),std::make_pair("RightTrigger" , 0.f) };
		//sticks
		static const int amountOfAnalogSticks = 2;
		ControllerStick m_Sticks[amountOfAnalogSticks] = { std::make_pair("LeftStick" , AnalogStickInput{0.f,0.f}),std::make_pair("RightStick" , AnalogStickInput{0.f,0.f}) };
		//controllers
		std::vector<std::unique_ptr<Controller>> m_Controllers;

		//keyboard
		KeyboardCommandsMap m_KeyboardButtonCommands;
	};
}
