#ifndef ___Math_h
#define ___Math_h

namespace Sparkler {

const float PI = 3.14159265358979f;
const float PI2 = PI * 2;

inline float Radian(float degree) { return degree * (PI2 / 360); }
inline float Degree(float radian) { return radian * (360 / PI2); }

inline float Square(float n) { return n * n; }

float Norm(float angle);

} // namespace Sparkler

#endif // ___Math_h


