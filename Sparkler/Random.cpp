#include "Random.h"

namespace Sparkler {

mt19937 Random::generator;

void Random::SetSeed(uint32_t seed) 
{
	generator.seed(seed);
}

int Random::GetInt(int min, int max)
{
	uniform_int<mt19937> distribution(generator, min, max);
	return distribution();
}

float Random::GetReal(float min, float max)
{
	uniform_real<mt19937, float> distribution(generator, min, max);
	return distribution();
}

float Random::GetNormalDistributionReal(float mean, float sigma)
{
	normal_distribution<mt19937, float> distribution(generator, mean, sigma);
	return distribution();
}

} // namespace Sparkler


