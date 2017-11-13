#ifndef ___Geometry_h
#define ___Geometry_h

#include <iostream>
#include <cmath>
using namespace std;

#define ALIGN16 //__declspec(align(16))

namespace Sparkler {

class Matrix;

class ALIGN16 Vector {
public:
	float x;
	float y;
	float z;
	
	explicit Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void Set(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	void DebugPrint() const;

	Vector& operator+=(const Vector& v);
	Vector& operator-=(const Vector& v);
	Vector& operator*=(float s);
	Vector& operator/=(float s);

	const Vector operator+(const Vector& v) const;
	const Vector operator-(const Vector& v) const;
	const Vector operator*(float s) const;
	const Vector operator/(float s) const;

	const Vector operator+() const;
	const Vector operator-() const;
	
	const Vector operator*(const Matrix& m) const;

	friend const Vector operator*(float s, const Vector& v);

	friend const Vector Unit(const Vector& v);
	
	friend float Length(const Vector& v);
	friend float LengthSquared(const Vector& v);
	friend float Distance(const Vector& v1, const Vector& v2);
	friend float DistanceSquared(const Vector& v1, const Vector& v2);

	friend float DotProduct(const Vector& v1, const Vector& v2);
	friend const Vector CrossProduct(const Vector& v1, const Vector& v2);

	friend float Angle(const Vector& v1, const Vector& v2);
	
	friend ostream& operator<<(ostream& s, const Vector& v);
};

class ALIGN16 Matrix {
public:
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
	
	Matrix();
	
	explicit Matrix(float m11, float m12, float m13, float m14, 
					float m21, float m22, float m23, float m24, 
					float m31, float m32, float m33, float m34, 
					float m41, float m42, float m43, float m44);
	
	Matrix(
		const Vector& axisX, 
		const Vector& axisY, 
		const Vector& axisZ, 
		const Vector& origin);
	
	void Set(float m11, float m12, float m13, float m14, 
			 float m21, float m22, float m23, float m24, 
			 float m31, float m32, float m33, float m34, 
			 float m41, float m42, float m43, float m44);

	void DebugPrint() const;

	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	Matrix& operator*=(const Matrix& m);
	Matrix& operator*=(float s);
	Matrix& operator/=(float s);
	
	const Matrix operator+(const Matrix& m) const;
	const Matrix operator-(const Matrix& m) const;
	const Matrix operator*(const Matrix& m) const;
	const Matrix operator*(float s) const;
	const Matrix operator/(float s) const;
	
	const Matrix operator+() const;
	const Matrix operator-() const;
	
	friend const Matrix operator*(float s, const Matrix& m);
	
	friend float Determinant();
	
	friend const Matrix Inverse(const Matrix& m);
	
	friend const Matrix Transpose(const Matrix& m);

	friend ostream& operator<<(ostream& s, const Matrix& m);
};

Matrix LookAtMatrix(const Vector& camera, const Vector& target, 
	const Vector& up = Vector(0, 1, 0));

Matrix PerspectiveMatrix(float fovH, float aspectRatio, float nearZ, float farZ);

inline Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

inline void Vector::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline Vector& Vector::operator+=(const Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vector& Vector::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline Vector& Vector::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline const Vector Vector::operator+(const Vector& v) const
{
	Vector ret(*this);
	return ret += v;
}

inline const Vector Vector::operator-(const Vector& v) const
{
	Vector ret(*this);
	return ret -= v;
}

inline const Vector Vector::operator*(float s) const
{
	Vector ret(*this);
	return ret *= s;
}

inline const Vector Vector::operator/(float s) const
{
	Vector ret(*this);
	return ret /= s;
}

inline const Vector Vector::operator+() const
{
	return Vector(x, y, z);
}

inline const Vector Vector::operator-() const
{
	return Vector(-x, -y, -z);
}

inline const Vector Vector::operator*(const Matrix& m) const
{
	float nx = x * m.m11 + y * m.m21 + z * m.m31 + m.m41;
	float ny = x * m.m12 + y * m.m22 + z * m.m32 + m.m42;
	float nz = x * m.m13 + y * m.m23 + z * m.m33 + m.m43;
	float nw =     m.m14 +     m.m24 +     m.m34 + m.m44;
	
	return Vector(nx / nw, ny / nw, nz / nw);
}

inline const Vector operator*(float s, const Vector& v)
{
	return v * s;
}

inline const Vector Unit(const Vector& v)
{
	return v / Length(v);
}

inline float Length(const Vector& v)
{
	return sqrt(LengthSquared(v));
}

inline float LengthSquared(const Vector& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

inline float Distance(const Vector& v1, const Vector& v2)
{
	return Length(v1 - v2);
}

inline float DistanceSquared(const Vector& v1, const Vector& v2)
{
	return LengthSquared(v1 - v2);
}

inline float DotProduct(const Vector& v1, const Vector& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

inline const Vector CrossProduct(const Vector& v1, const Vector& v2)
{
	return Vector(v1.y * v2.z - v1.z * v2.y, 
				  v1.z * v2.x - v1.x * v2.z, 
				  v1.x * v2.y - v1.y * v2.x);
}

inline float Angle(const Vector& v1, const Vector& v2)
{
	return acos(DotProduct(v1, v2) / (Length(v1) * Length(v2)));
}

inline ostream& operator<<(ostream& s, const Vector& v)
{
	return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

inline const Vector UnitVectorZAligned(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	
	return Vector(c, s, 0);
}

inline Matrix::Matrix()
{
	Set(1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1);
}

inline Matrix::Matrix(float m11, float m12, float m13, float m14, 
					  float m21, float m22, float m23, float m24, 
					  float m31, float m32, float m33, float m34, 
					  float m41, float m42, float m43, float m44) : 
	m11(m11), m12(m12), m13(m13), m14(m14), 
	m21(m21), m22(m22), m23(m23), m24(m24), 
	m31(m31), m32(m32), m33(m33), m34(m34), 
	m41(m41), m42(m42), m43(m43), m44(m44) {}

inline Matrix::Matrix(
	const Vector& axisX, 
	const Vector& axisY, 
	const Vector& axisZ, 
	const Vector& origin)
{
	Set( axisX.x,  axisX.y,  axisX.z, 0.0f, 
		 axisY.x,  axisY.y,  axisY.z, 0.0f, 
		 axisZ.x,  axisZ.y,  axisZ.z, 0.0f, 
		origin.x, origin.y, origin.z, 1.0f);
}

inline void Matrix::Set(float m11, float m12, float m13, float m14, 
						float m21, float m22, float m23, float m24, 
						float m31, float m32, float m33, float m34, 
						float m41, float m42, float m43, float m44)
{
	this->m11 = m11, this->m12 = m12, this->m13 = m13, this->m14 = m14;
	this->m21 = m21, this->m22 = m22, this->m23 = m23, this->m24 = m24;
	this->m31 = m31, this->m32 = m32, this->m33 = m33, this->m34 = m34;
	this->m41 = m41, this->m42 = m42, this->m43 = m43, this->m44 = m44;
}

inline Matrix& Matrix::operator+=(const Matrix& m)
{
	Set(m11 + m.m11, m12 + m.m12, m13 + m.m13, m14 + m.m14, 
		m21 + m.m21, m22 + m.m22, m23 + m.m23, m24 + m.m24, 
		m31 + m.m31, m32 + m.m32, m33 + m.m33, m34 + m.m34, 
		m41 + m.m41, m42 + m.m42, m43 + m.m43, m44 + m.m44);
	return *this;
}

inline Matrix& Matrix::operator-=(const Matrix& m)
{
	Set(m11 - m.m11, m12 - m.m12, m13 - m.m13, m14 - m.m14, 
		m21 - m.m21, m22 - m.m22, m23 - m.m23, m24 - m.m24, 
		m31 - m.m31, m32 - m.m32, m33 - m.m33, m34 - m.m34, 
		m41 - m.m41, m42 - m.m42, m43 - m.m43, m44 - m.m44);
	return *this;
}

inline Matrix& Matrix::operator*=(const Matrix& m)
{
	Set(m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41, 
		m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42, 
		m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43, 
		m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44, 
		
		m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41, 
		m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42, 
		m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43, 
		m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44, 
		
		m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41, 
		m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42, 
		m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43, 
		m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44, 
		
		m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41, 
		m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42, 
		m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43, 
		m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44);
	return *this;
}

inline Matrix& Matrix::operator*=(float s)
{
	Set(m11 * s, m12 * s, m13 * s, m14 * s, 
		m21 * s, m22 * s, m23 * s, m24 * s, 
		m31 * s, m32 * s, m33 * s, m34 * s, 
		m41 * s, m42 * s, m43 * s, m44 * s);
	return *this;
}

inline Matrix& Matrix::operator/=(float s)
{
	Set(m11 / s, m12 / s, m13 / s, m14 / s, 
		m21 / s, m22 / s, m23 / s, m24 / s, 
		m31 / s, m32 / s, m33 / s, m34 / s, 
		m41 / s, m42 / s, m43 / s, m44 / s);
	return *this;
}

inline const Matrix Matrix::operator+(const Matrix& m) const
{
	Matrix ret(*this);
	return ret += m;
}

inline const Matrix Matrix::operator-(const Matrix& m) const
{
	Matrix ret(*this);
	return ret -= m;
}

inline const Matrix Matrix::operator*(const Matrix& m) const
{
	Matrix ret(*this);
	return ret *= m;
}

inline const Matrix Matrix::operator*(float s) const
{
	Matrix ret(*this);
	return ret *= s;
}

inline const Matrix Matrix::operator/(float s) const
{
	Matrix ret(*this);
	return ret /= s;
}

inline const Matrix Matrix::operator+() const
{
	return Matrix(m11, m12, m13, m14, 
				  m21, m22, m23, m24, 
				  m31, m32, m33, m34, 
				  m41, m42, m43, m44);
}

inline const Matrix Matrix::operator-() const
{
	return Matrix(-m11, -m12, -m13, -m14, 
				  -m21, -m22, -m23, -m24, 
				  -m31, -m32, -m33, -m34, 
				  -m41, -m42, -m43, -m44);
}

inline const Matrix Transpose(const Matrix& m)
{
	return Matrix(m.m11, m.m21, m.m31, m.m41, 
				  m.m12, m.m22, m.m32, m.m42, 
				  m.m13, m.m23, m.m33, m.m43, 
				  m.m14, m.m24, m.m34, m.m44);
}

inline Matrix IdentityMatrix()
{
	return Matrix(1, 0, 0, 0, 
				  0, 1, 0, 0, 
				  0, 0, 1, 0,
				  0, 0, 0, 1);
}

inline Matrix RotationMatrixX(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	return Matrix( 1,  0,  0,  0, 
				   0,  c,  s,  0, 
				   0, -s,  c,  0, 
				   0,  0,  0,  1);
}

inline Matrix RotationMatrixY(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	return Matrix( c,  0, -s,  0, 
				   0,  1,  0,  0, 
				   s,  0,  c,  0, 
				   0,  0,  0,  1);
}

inline Matrix RotationMatrixZ(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	return Matrix( c,  s,  0,  0, 
				  -s,  c,  0,  0, 
				   0,  0,  1,  0, 
				   0,  0,  0,  1);
}

inline Matrix NegationMatrixX()
{
	return Matrix(-1,  0,  0,  0, 
				   0,  1,  0,  0, 
				   0,  0,  1,  0, 
				   0,  0,  0,  1);
}

inline Matrix NegationMatrixY()
{
	return Matrix( 1,  0,  0,  0, 
				   0, -1,  0,  0, 
				   0,  0,  1,  0, 
				   0,  0,  0,  1);
}

inline Matrix NegationMatrixZ()
{
	return Matrix( 1,  0,  0,  0, 
				   0,  1,  0,  0, 
				   0,  0, -1,  0, 
				   0,  0,  0,  1);
}

inline Matrix ScalingMatrix(float fx, float fy, float fz)
{
	return Matrix(fx,  0,  0,  0, 
				   0, fy,  0,  0, 
				   0,  0, fz,  0, 
				   0,  0,  0,  1);
}

inline Matrix ScalingMatrix(float s)
{
	return Matrix( s,  0,  0,  0, 
				   0,  s,  0,  0, 
				   0,  0,  s,  0, 
				   0,  0,  0,  1);
}

inline Matrix TranslationMatrix(float tx = 0, float ty = 0, float tz = 0)
{
	return Matrix( 1,  0,  0,  0, 
				   0,  1,  0,  0, 
				   0,  0,  1,  0, 
				  tx, ty, tz,  1);
}

inline Matrix TranslationMatrix(const Vector& v)
{
	return Matrix(  1,   0,   0,   0, 
				    0,   1,   0,   0, 
				    0,   0,   1,   0, 
				  v.x, v.y, v.z,   1);
}

} // namespace Sparkler

#endif // ___Geometry_h


