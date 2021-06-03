#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#pragma warning(pop)

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }

		const glm::vec2& GetScale() const { return m_Scale; }

		void SetPosition(float x, float y, float z)
		{
			m_Position.x = x;
			m_Position.y = y;
			m_Position.z = z;
		}

		void SetScale(float x, float y)
		{
			m_Scale.x = x;
			m_Scale.y = y;
		}

	private:
		glm::vec3 m_Position;
		glm::vec2 m_Scale;
	};
}
