#pragma once
namespace dae
{
	class SceneObject
	{
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;

		bool GetDeleteLater() { return m_DeleteLater; };
		//
		SceneObject(const std::string& name) :m_Name{ name } {};
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;
		//
		const std::string& GetName() const { return m_Name; };

	protected:
		bool  m_DeleteLater = false;
		const std::string m_Name;
	};
}
