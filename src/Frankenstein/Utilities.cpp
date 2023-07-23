#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

using namespace Frankenstein::Utility;

/*
* Reads an .xml file of spritesheet info and populates SpriteSheetParameters object
*/
bool Frankenstein::Utility::Utilities::ReadXmlFile(const char* fileName, SpriteSheetParameters& parameters)
{
	char* pch;

	std::string line;
	std::vector<std::string> row;

	std::ifstream in(fileName);

	if (!in.is_open()) { return false; }

	while (getline(in, line))
	{
		row.clear();

		pch = strtok((char*)line.c_str(), " \t></");
		if (pch != NULL)
			row.push_back(pch);

		while (pch != NULL)
		{
			pch = strtok(NULL, " \t></");
			if (pch != NULL)
				row.push_back(pch);
		}

		if (row.size() < 3)
			continue;

		if (row[0] == "SpriteSheetWidth")
		{
			parameters.m_spriteSheetWidth = stoi(row[1]);
		}
		else if (row[0] == "SpriteSheetHeight")
		{
			parameters.m_spriteSheetHeight = stoi(row[1]);
		}
		else if (row[0] == "FrameWidth")
		{
			parameters.m_frameWidth = stoi(row[1]);
		}
		else if (row[0] == "FrameHeight")
		{
			parameters.m_frameHeight = stoi(row[1]);
		}
		else if (row[0] == "FileName")
		{
			parameters.m_fileName = row[1];
		}
	}
	in.close();
	return true;
}

/*
*  Reads an .csv file which contains AnimationParameter data
*  and loads each animation into a vector.
*/
bool Frankenstein::Utility::Utilities::ReadCsvFile(const char* fileName, std::vector<AnimationParameters>& animParamVect)
{
	std::ifstream file(fileName);

	if (file.is_open())
	{
		std::string line;
		// Skip the first line (header)
		std::getline(file, line);
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string token;
			AnimationParameters param;
			std::getline(ss, token, ',');
			param.animationName = token;
			std::getline(ss, token, ',');
			param.framesInAnimation = std::stoi(token);
			std::getline(ss, token, ',');
			param.rowsInAnimation = std::stoi(token);
			std::getline(ss, token, ',');
			param.startingRow = std::stoi(token);
			std::getline(ss, token, ',');
			param.startingCol = std::stoi(token);
			std::getline(ss, token, ',');
			param.animationCount = std::stoi(token);
			animParamVect.push_back(param);
		}
		return true;
	}

	return false;
}

