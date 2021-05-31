#pragma once
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