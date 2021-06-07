#pragma once
#include <iostream>

#include "Components.h"
#include "QbertMovementComponent.h"
#include "BaseCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "EnemyManager.h"
#include "QbertSceneManager.h"



class LoadSinglePlayerCommand : public Command
{
public:
	LoadSinglePlayerCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadSinglePlayerCommand() override = default;
	void Execute() const override {
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == GameMode::MainMenu)
			QbertSceneManager::GetInstance().LoadSinglePlayer();

	};
	void Release() const override {};
	void Undo() override {};
};


class LoadCoopCommand : public Command
{
public:
	LoadCoopCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadCoopCommand() override = default;
	void Execute() const override {
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == GameMode::MainMenu)
			QbertSceneManager::GetInstance().LoadCoop();

	};
	void Release() const override {};
	void Undo() override {};
};


class LoadVersusCommand : public Command
{
public:
	LoadVersusCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadVersusCommand() override = default;
	void Execute() const override {
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() == GameMode::MainMenu)
			QbertSceneManager::GetInstance().LoadVersus();

	};
	void Release() const override {};
	void Undo() override {};
};


class LoadMainMenuCommand : public Command
{
public:
	LoadMainMenuCommand(int controllerIndex) : Command(controllerIndex) { m_ControllerIndex = controllerIndex; };
	~LoadMainMenuCommand() override = default;
	void Execute() const override {
		if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() != GameMode::MainMenu)
			QbertSceneManager::GetInstance().LoadMainMenu();

	};
	void Release() const override {};
	void Undo() override {};
};




class ExitCommand final : public Command
{
public:
	ExitCommand(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override { dae::InputManager::GetInstance().SetIsExiting(true); }
	void Release() const override {};
	void Undo() override {};
};


class JumpUp final : public Command
{
public:
	JumpUp(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Up);
	}

	void Release() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Up);
	};

	void Undo() override {};
};

class JumpDown final : public Command
{
public:
	JumpDown(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Down);
	}

	void Release() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Down);
	};
	void Undo() override {};
};

class JumpLeft final : public Command
{
public:
	JumpLeft(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Left);
	}

	void Release() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Left);
	};
	void Undo() override {};
};

class JumpRight final : public Command
{
public:
	JumpRight(int index) :Command(index) { m_ControllerIndex = index; };

	void Execute() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Right);
	}

	void Release() const override
	{
		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(m_ControllerIndex);
		if (pPlayer)pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Right);
	};
	void Undo() override {};
};




//class JumpUpP2 final : public Command
//{
//public:
//	JumpUpP2(int index) :Command(index) {};
//
//	void Execute() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if(pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Up);
//	}
//
//	void Release() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Up);
//	};
//
//	void Undo() override {};
//};
//
//class JumpDownP2 final : public Command
//{
//public:
//	JumpDownP2(int index) :Command(index) {};
//
//	void Execute() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Down);
//	}
//
//	void Release() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Down);
//	};
//	void Undo() override {};
//};
//
//class JumpLeftP2 final : public Command
//{
//public:
//	JumpLeftP2(int index) :Command(index) {};
//
//	void Execute() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Left);
//	}
//
//	void Release() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Left);
//	};
//	void Undo() override {};
//};
//
//class JumpRightP2 final : public Command
//{
//public:
//	JumpRightP2(int index) :Command(index) {};
//
//	void Execute() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->Move(MoveDirections::Right);
//	}
//
//	void Release() const override
//	{
//		auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
//		if (pPlayer) pPlayer->GetComponent<QbertMovementComponent>()->KeyReleased(MoveDirections::Right);
//	};
//	void Undo() override {};
//};