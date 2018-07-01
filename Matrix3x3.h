#include "Vector3.h"

class Matrix3x3
{
public:
	float m11, m12, m13,
	      m21, m22, m23,
	      m31, m32, m33;
public:
	Matrix3x3();
	Matrix3x3(const Vector3&, const Vector3&, const Vector3&);
	Matrix3x3(float, float, float,
	          float, float, float,
	          float, float, float);
	inline void setColumn1(const Vector3&);
	inline void setColumn2(const Vector3&);
	inline void setColumn3(const Vector3&);
	inline void set(float, float, float,
	                float, float, float,
	                float, float, float);
	inline float det();
	
};

inline Matrix3x3::Matrix3x3() {
	m11 = m12 = m13 = m21 = m22 = m23 = m31 = m32 = m33 = 0.0f;
}

inline Matrix3x3::Matrix3x3(const Vector3& a, const Vector3& b, const Vector3& c) {
	m11 = a.x; m21 = a.y; m31 = a.z;
	m12 = b.x; m22 = b.y; m32 = b.z;
	m13 = c.x; m23 = c.y; m33 = c.z;
}
inline Matrix3x3::Matrix3x3(float a11, float a12, float a13,
                            float a21, float a22, float a23,
                            float a31, float a32, float a33)
{
	set(a11, a12, a13, a21, a22, a23, a31, a32, a33);
}

inline void Matrix3x3::setColumn1(const Vector3& v) {
	m11 = v.x; m21 = v.y, m31 = v.z;
}

inline void Matrix3x3::setColumn2(const Vector3& v) {
	m12 = v.x; m22 = v.y; m32 = v.z;
}

inline void Matrix3x3::setColumn3(const Vector3& v) {
	m13 = v.x; m23 = v.y; m33 = v.z;	
}

inline void Matrix3x3::set(float a11, float a12, float a13,
                           float a21, float a22, float a23,
                           float a31, float a32, float a33)
{
	m11 = a11;
	m12 = a12;
	m13 = a13;
	m21 = a21;
	m22 = a22;
	m23 = a23;
	m31 = a31;
	m32 = a32;
	m33 = a33;
}

inline float Matrix3x3::det()
{
	return m11 * m22 * m33 + m21 * m32 * m13 + m31 * m12 * m23 - m13 * m22 * m31 - m23 * m32 * m11 - m33 * m12 * m21;
}
