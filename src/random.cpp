#include <random>
#include "Random.hpp"

namespace Birb
{
	Random::Random()
	{
		srand(time(0));
	}

	int Random::RandomInt(const int& min, const int& max)
	{
		if (max < min)
			return min;

		return rand() % (max + 1 - min) + min;
	}

	float Random::RandomFloat(const float& min, const float& max)
	{
		if (max < min)
			return min;

		float random = (static_cast<float> (rand())) / static_cast<float> (RAND_MAX);
		float range = max - min;
		return (random * range) + min;
	}

	void Random::ShuffleArray(int* values, const int& size)
	{
		Random rand;
		int newIndex;
		int placeHolder;
		for (int i = 0; i < size; ++i)
		{
			newIndex = rand.RandomInt(0, size - 1);
			if (newIndex != i)
			{
				placeHolder = values[i];
				values[i] = values[newIndex];
				values[newIndex] = placeHolder;
			}
		}
	}

	void Random::ShuffleArray(float* values, const int& size)
	{
		Random rand;
		int newIndex;
		float placeHolder;
		for (int i = 0; i < size; ++i)
		{
			do
			{
				newIndex = rand.RandomInt(0, size - 1);
			} while (newIndex == i);

			placeHolder = values[i];
			values[i] = values[newIndex];
			values[newIndex] = placeHolder;
		}
	}

	void Random::ShuffleArray(double* values, const int& size)
	{
		Random rand;
		int newIndex;
		double placeHolder;
		for (int i = 0; i < size; ++i)
		{
			do
			{
				newIndex = rand.RandomInt(0, size - 1);
			} while (newIndex == i);

			placeHolder = values[i];
			values[i] = values[newIndex];
			values[newIndex] = placeHolder;
		}
	}

	void Random::ShuffleArray(std::string* values, const int& size)
	{
		Random rand;
		int newIndex;
		std::string placeHolder;
		for (int i = 0; i < size; ++i)
		{
			do
			{
				newIndex = rand.RandomInt(0, size - 1);
			} while (newIndex == i);

			placeHolder = values[i];
			values[i] = values[newIndex];
			values[newIndex] = placeHolder;
		}
	}
}
