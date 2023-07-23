#pragma once

#include <vector>
#include "SpriteSheetInfo.h"
#include "AnimationParameters.h"

namespace Frankenstein
{
	namespace Utility
	{
		struct Utilities
		{
			static bool ReadXmlFile(const char* fileName, SpriteSheetParameters& parameters);
			static bool ReadCsvFile(const char* fileName, std::vector<AnimationParameters>& parameters);
		};
	}
}