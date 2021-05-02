#pragma once
#include <iostream>

#include "Components.h"
#include "MovementComponent.h"

#include "SceneManager.h"
#include "Scene.h"

class Command abstract
{
public:
	Command(int index) :m_ControllerIndex{ index } {};

	virtual ~Command() = default;
	virtual void Execute() const = 0;
	virtual void Release() const = 0;
	virtual void Undo() = 0;

	[[nodiscard]] bool GetIsPressed() const { return m_IsPressed; };
	void SetIsPressed(bool isPressed) { m_IsPressed = isPressed; };
protected:
	bool m_IsPressed = false;
	int m_ControllerIndex;
};

class FireCommand final : public Command
{
public:
	FireCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Fire!" << '\n'; }
	void Release() const override {};

	void Undo() override {};
};

class DuckCommand final : public Command
{
public:
	DuckCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Duck!" << '\n'; }
	void Release() const override {};
	void Undo() override {};
};

class JumpCommand final : public Command
{
public:
	JumpCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Jump!" << '\n'; }
	void Release() const override {};
	void Undo() override {};
};

class FartCommand final : public Command
{
public:
	FartCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Fart..." << '\n'; }
	void Release() const override {};
	void Undo() override {};
};

class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Exiting..." << '\n'; }
	void Release() const override {};
	void Undo() override {};
};

class DieCommand final : public Command
{
public:
	DieCommand(int index) :Command(index) {};

	void Execute() const override
	{
		std::cout << "Die!" << '\n';
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<HealthComponent>()->Die();
	}

	void Release() const override {};

	void Undo() override {};
};

class IncreasePointsCommand final : public Command
{
public:
	IncreasePointsCommand(int index) :Command(index) {};

	void Execute() const override
	{
		std::cout << "PointIncrease" << std::endl;
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
	}

	void Release() const override {};

	void Undo() override {};
};
//-------------------------------------------------------------------------------------------------------IDLE MOVEMENT-------------------------------------------------------
class MoveLeftTop final : public Command
{
public:
	MoveLeftTop(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Release() const override {};

	void Undo() override {};
};

class MoveRightTop final : public Command
{
public:
	MoveRightTop(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Release() const override {};

	void Undo() override {};
};

class MoveRightDown final : public Command
{
public:
	MoveRightDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Release() const override {};

	void Undo() override {};
};

class MoveLeftDown final : public Command
{
public:
	MoveLeftDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		//pPlayerActor.get()->GetComponent<TransformComponent>()->MoveLeftTop ? ? ? ;
	}

	void Release() const override {};

	void Undo() override {};
};
//-------------------------------------------------------------------------------------------------------JUMP MOVEMENT-------------------------------------------------------
class JumpUp final : public Command
{
public:
	JumpUp(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirections::Up);
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->KeyReleased(MoveDirections::Up);
	};

	void Undo() override {};
};

class JumpDown final : public Command
{
public:
	JumpDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirections::Down);
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->KeyReleased(MoveDirections::Down);
	};
	void Undo() override {};
};

class JumpLeft final : public Command
{
public:
	JumpLeft(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirections::Left);
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->KeyReleased(MoveDirections::Left);
	};
	void Undo() override {};
};

class JumpRight final : public Command
{
public:
	JumpRight(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->Move(MoveDirections::Right);
	}

	void Release() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor->GetComponent<MovementComponent>()->KeyReleased(MoveDirections::Right);
	};
	void Undo() override {};
};