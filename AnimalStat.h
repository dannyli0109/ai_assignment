#pragma once

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
	};
}