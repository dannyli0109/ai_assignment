#pragma once
#include <string>
namespace Components
{
	struct AnimalStat
	{
		float hunger;
		float thirst;
		float decayRate;
		float incrementRate;
		bool drinking = false;
		bool eating = false; 
		bool hunting = false;
		std::string decision = "";
	};
}