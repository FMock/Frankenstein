#pragma once

#include <vector>
#include "SpriteSheetInfo.h"
#include "AnimationParameters.h"
#include "FrankensteinAPI.h"

namespace Frankenstein
{
	namespace Utility
	{
		struct FRANKENSTEIN_API Utilities
		{
			static bool ReadXmlFile(const char* fileName, SpriteSheetParameters& parameters);
			static bool ReadCsvFile(const char* fileName, std::vector<AnimationParameters>& parameters);
		};
	}
}