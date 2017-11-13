#include <cassert>
using namespace std;

#include "Log.h"
#include "Geometry.h"

namespace Sparkler {

#define FLOAT_FORMAT "%13.6f"

void Vector::DebugPrint() const
{
    Log::Printf("("FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT")\n", x, y, z);
}

void Matrix::DebugPrint() const
{
    Log::Printf("["FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT"]\n", m11, m12, m13, m14);
    Log::Printf("["FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT"]\n", m21, m22, m23, m24);
    Log::Printf("["FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT"]\n", m31, m32, m33, m34);
    Log::Printf("["FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT", "FLOAT_FORMAT"]\n", m41, m42, m43, m44);
}

float Determinant(const Matrix& m)
{
    return (m.m11 * m.m22 - m.m12 * m.m21) * (m.m33 * m.m44 - m.m34 * m.m43)
         - (m.m11 * m.m23 - m.m13 * m.m21) * (m.m32 * m.m44 - m.m34 * m.m42)
         + (m.m11 * m.m24 - m.m14 * m.m21) * (m.m32 * m.m43 - m.m33 * m.m42)
         + (m.m12 * m.m23 - m.m13 * m.m22) * (m.m31 * m.m44 - m.m34 * m.m41)
         - (m.m12 * m.m24 - m.m14 * m.m22) * (m.m31 * m.m43 - m.m33 * m.m41)
         + (m.m13 * m.m24 - m.m14 * m.m23) * (m.m31 * m.m42 - m.m32 * m.m41);
}

const Matrix Inverse(const Matrix& m)
{
    float determinant = Determinant(m);

    assert(determinant != 0);
    
    Matrix ret(
        m.m22*(m.m33*m.m44 - m.m34*m.m43) + m.m23*(m.m34*m.m42 - m.m32*m.m44) + m.m24*(m.m32*m.m43 - m.m33*m.m42),
        m.m32*(m.m13*m.m44 - m.m14*m.m43) + m.m33*(m.m14*m.m42 - m.m12*m.m44) + m.m34*(m.m12*m.m43 - m.m13*m.m42),
        m.m42*(m.m13*m.m24 - m.m14*m.m23) + m.m43*(m.m14*m.m22 - m.m12*m.m24) + m.m44*(m.m12*m.m23 - m.m13*m.m22),
        m.m12*(m.m24*m.m33 - m.m23*m.m34) + m.m13*(m.m22*m.m34 - m.m24*m.m32) + m.m14*(m.m23*m.m32 - m.m22*m.m33),
        
        m.m23*(m.m31*m.m44 - m.m34*m.m41) + m.m24*(m.m33*m.m41 - m.m31*m.m43) + m.m21*(m.m34*m.m43 - m.m33*m.m44),
        m.m33*(m.m11*m.m44 - m.m14*m.m41) + m.m34*(m.m13*m.m41 - m.m11*m.m43) + m.m31*(m.m14*m.m43 - m.m13*m.m44),
        m.m43*(m.m11*m.m24 - m.m14*m.m21) + m.m44*(m.m13*m.m21 - m.m11*m.m23) + m.m41*(m.m14*m.m23 - m.m13*m.m24),
        m.m13*(m.m24*m.m31 - m.m21*m.m34) + m.m14*(m.m21*m.m33 - m.m23*m.m31) + m.m11*(m.m23*m.m34 - m.m24*m.m33),
        
        m.m24*(m.m31*m.m42 - m.m32*m.m41) + m.m21*(m.m32*m.m44 - m.m34*m.m42) + m.m22*(m.m34*m.m41 - m.m31*m.m44),
        m.m34*(m.m11*m.m42 - m.m12*m.m41) + m.m31*(m.m12*m.m44 - m.m14*m.m42) + m.m32*(m.m14*m.m41 - m.m11*m.m44),
        m.m44*(m.m11*m.m22 - m.m12*m.m21) + m.m41*(m.m12*m.m24 - m.m14*m.m22) + m.m42*(m.m14*m.m21 - m.m11*m.m24),
        m.m14*(m.m22*m.m31 - m.m21*m.m32) + m.m11*(m.m24*m.m32 - m.m22*m.m34) + m.m12*(m.m21*m.m34 - m.m24*m.m31),
        
        m.m21*(m.m33*m.m42 - m.m32*m.m43) + m.m22*(m.m31*m.m43 - m.m33*m.m41) + m.m23*(m.m32*m.m41 - m.m31*m.m42),
        m.m31*(m.m13*m.m42 - m.m12*m.m43) + m.m32*(m.m11*m.m43 - m.m13*m.m41) + m.m33*(m.m12*m.m41 - m.m11*m.m42),
        m.m41*(m.m13*m.m22 - m.m12*m.m23) + m.m42*(m.m11*m.m23 - m.m13*m.m21) + m.m43*(m.m12*m.m21 - m.m11*m.m22),
        m.m11*(m.m22*m.m33 - m.m23*m.m32) + m.m12*(m.m23*m.m31 - m.m21*m.m33) + m.m13*(m.m21*m.m32 - m.m22*m.m31)
    );
    ret /= determinant;
    return ret;
}

Matrix LookAtMatrix(const Vector& camera, const Vector& target, const Vector& up)
{
    Vector axisZ = Unit(target - camera);
    Vector axisX = Unit(CrossProduct(up, axisZ));
    Vector axisY = CrossProduct(axisZ, axisX);

    return Inverse(Matrix(axisX, axisY, axisZ, camera));
}

Matrix PerspectiveMatrix(float fovH, float aspectRatio, float nearZ, float farZ)
{
    float h = 1 / tan(fovH / 2);
    float w = h / aspectRatio;
    float q = farZ / (farZ - nearZ);
    
    return Matrix( w,  0,  0,  0, 
                   0,  h,  0,  0, 
                   0,  0,  q,  1, 
                   0,  0, -q * nearZ, 0);
}

} // namespace Sparkler


