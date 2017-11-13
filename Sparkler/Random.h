#ifndef ___Random_h
#define ___Random_h

#pragma warning(push)
#pragma warning(disable : 4244)

#include <boost/cstdint.hpp>
#include <boost/random.hpp>
using namespace boost;

#pragma warning(pop)

namespace Sparkler {

class Random {
public:
	static void SetSeed(uint32_t);

	static int GetInt(int min, int max);
	static float GetReal(float min, float max);
	static float GetNormalDistributionReal(float mean, float sigma);

private:
	static mt19937 generator;
};

} // namespace Sparkler

#endif // ___Random_h

