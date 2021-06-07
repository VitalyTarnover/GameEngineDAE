#pragma once
#include <glm\vec3.hpp>
#include <vector>

class LevelFileReader final
{
public:
	LevelFileReader(const std::string& filePath);
	~LevelFileReader() = default;

	LevelFileReader(const LevelFileReader&) = delete;
	LevelFileReader(LevelFileReader&&) noexcept = delete;
	LevelFileReader& operator=(const LevelFileReader&) = delete;
	LevelFileReader& operator=(LevelFileReader&&) noexcept = delete;


	std::vector<glm::vec3> ReadGetPositions();
private:
	std::vector<glm::vec3> m_Positions;
	std::string m_FilePath;
};

