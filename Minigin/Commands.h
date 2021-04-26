#pragma once
#include <iostream>
#include "Components.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MovementComponent.h"

class Command abstract
{
public:
	Command(int index) :m_ControllerIndex{ index } {};

	virtual ~Command() = default;
	virtual void Execute() const = 0;
	virtual void Undo() = 0;

	[[nodiscard]] bool GetIsPressed() const { return m_IsPressed; };
	void SetIsPressed(bool isPressed) { m_IsPressed = isPressed; };
protected:
	bool m_IsPressed = false;
	int m_ControllerIndex;
};

class MoveLeftDown final : public Command
{
public:
	MoveLeftDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftDown);
		//pPlayerActor.get()->GetComponent<MovementComponent>()->Move(MoveDirections::MoveDownLeft);
	}
	void Undo() override {};
};

class MoveRightUp final : public Command
{
public:
	MoveRightUp(int index) :Command(index) {};

	void Execute() const override 
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformRightUp);
		pPlayerActor.get()->GetComponent<MovementComponent>()->Move(MoveDirections::MoveUpRight);
	}

	void Undo() override {};
};

class MoveLeftUp final : public Command
{
public:
	MoveLeftUp(int index) :Command(index) {};

	void Execute() const override 
	{ 
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftUp);
	}

	void Undo() override {};
};

class MoveRightDown final : public Command
{
public:
	MoveRightDown(int index) :Command(index) {};

	void Execute() const override
	{
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformRightDown);
	}
	void Undo() override {};
};

class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) {};

	void Execute() const override { std::cout << "Exiting..." << '\n'; }
	void Undo() override {};
};

class DieCommand final : public Command
{
public:
	DieCommand(int index) :Command(index) {};

	void Execute() const override
	{
		//std::cout << "Die!" << '\n';
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<HealthComponent>()->Die();
	}

	void Undo() override {};
};

class IncreasePointsCommand final : public Command
{
public:
	IncreasePointsCommand(int index) :Command(index) {};

	void Execute() const override
	{
		//std::cout << "PointIncrease" << std::endl;
		auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		pPlayerActor.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);

	}

	void Undo() override {};
};
