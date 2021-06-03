#include "MiniginPCH.h"
#include "LevelFileReader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

LevelFileReader::LevelFileReader(std::string filePath)
    :m_FilePath{filePath}
{
}

std::vector<glm::vec3> LevelFileReader::ReadGetPositions()
{
    m_Positions.clear();

	std::ifstream input;
	std::string currentLine;
	std::string stringBuffer;
	float bufferX = 0;
	float bufferY = 0;


	input.open(m_FilePath, std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (std::getline(input, currentLine))
		{
			for (size_t i = 0; i < currentLine.size(); i++)
			{
				if (currentLine[i] != ',' && currentLine[i] != '\r')
				{
					stringBuffer += currentLine[i];
				}
				else if (currentLine[i] == ',')
				{
					bufferX = std::stof(stringBuffer);
					stringBuffer = "";
				}
			}
			bufferY = std::stof(stringBuffer);
			stringBuffer = "";
			m_Positions.push_back(glm::vec3{ bufferX, bufferY, 0 });
		}
	}

    return m_Positions;
}
