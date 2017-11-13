#include "Math.h"

#include <cmath>
using namespace std;

namespace Sparkler {

float Norm(float angle)
{
    return angle - PI2 * floor((angle + PI) / PI2);
}

} // namespace Sparkler


