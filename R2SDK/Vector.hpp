#pragma once

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)
#define M_PI		3.14159265358979323846f
#define M_TAU		6.2831853071795864769252867666f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )

class Vector
{
public:

	float	x, y, z;
	Vector(void);
	Vector(float x, float y, float z);

	float& operator[](int) const;
	Vector& operator=(const Vector&);

	Vector& operator+=(const Vector&);
	Vector& operator-=(const Vector&);
	Vector& operator*=(const Vector&);
	Vector& operator*=(const float&);
	Vector& operator/=(const Vector&);
	Vector& operator/=(const float&);

	Vector	operator+(const Vector&) const;
	Vector	operator-(const Vector&) const;
	Vector	operator*(const Vector&) const;
	Vector	operator*(const float&) const;
	Vector	operator/(const Vector&) const;
	Vector	operator/(const float&) const;

	bool	operator==(const Vector&) const;
	bool	operator!=(const Vector&) const;

	float	DistTo(const Vector&) const;
	inline float DistTo2D(const Vector& v) const;
	float	Length() const;
	float	LengthSqr() const;
	float	Length2D() const;
	float	Length2DSqr() const;
	Vector	Cross(const Vector&) const;
	float	Dot(const Vector&) const;
	float	Normalize();
	void Init(float, float, float);
	void Zero();
	bool IsZero(float);
	void VectorClear(Vector&);
	void VectorCopy(const Vector&, Vector&);
	float DotProduct(const Vector& a, const Vector& b);
};

inline Vector::Vector()
{
	x = y = z = 0.f;
}

inline void Vector::Zero()
{
	x = y = z = 0.0f;
}

inline Vector::Vector(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

inline float& Vector::operator[](int i) const
{
	return ((float*)this)[i];
}

inline Vector& Vector::operator=(const Vector& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
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

inline Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

inline Vector& Vector::operator*=(const float& f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

inline Vector& Vector::operator/=(const Vector& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

inline Vector& Vector::operator/=(const float& f)
{
	x /= f;
	y /= f;
	z /= f;

	return *this;
}

inline Vector Vector::operator+(const Vector& v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

inline Vector Vector::operator-(const Vector& v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

inline Vector Vector::operator*(const Vector& v) const
{
	return Vector(x * v.x, y * v.y, z * v.z);
}

inline Vector Vector::operator*(const float& f) const
{
	return Vector(x * f, y * f, z * f);
}

inline Vector Vector::operator/(const Vector& v) const
{
	return Vector(x / (v.x), y / (v.y), z / (v.z));
}

inline Vector Vector::operator/(const float& f) const
{
	return Vector(x / (f), y / (f), z / (f));
}

inline bool Vector::operator==(const Vector& v) const
{
	return v.x == x && v.y == y && v.z == z;
}

inline bool Vector::operator!=(const Vector& e) const
{
	return e.x != x || e.y != y || e.z != z;
}

inline float Vector::DistTo(const Vector& v) const
{
	return (*this - v).Length();
}

inline float Vector::DistTo2D(const Vector& v) const
{
	return (*this - v).Length2D();
}

inline float Vector::Length() const
{
	return ::sqrtf((x * x) + (y * y) + (z * z));
}

inline float Vector::LengthSqr() const
{
	return (x * x) + (y * y) + (z * z);
}

inline float Vector::Length2D() const
{
	return ::sqrtf((x * x) + (y * y));
}

inline float Vector::Length2DSqr() const
{
	return (x * x) + (y * y);
}

inline Vector Vector::Cross(const Vector& e) const
{
	return Vector((y * e.z) - (z * e.y), (z * e.x) - (x * e.z), (x * e.y) - (y * e.x));
}

inline float Vector::Dot(const Vector& e) const
{
	return (x * e.x) + (y * e.y) + (z * e.z);
}

inline float Vector::Normalize()
{
	float l = Length();
	float m = 1.f / (l);

	*this *= m;

	return l;
}

inline void Vector::Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
{
	x = ix; y = iy; z = iz;
}

inline void Vector::VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.0f;
}

inline bool Vector::IsZero(float tolerance = 0.01f)
{
	return (x > -tolerance && x < tolerance&&
		y > -tolerance && y < tolerance&&
		z > -tolerance && z < tolerance);
}

__forceinline void Vector::VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline float Vector::DotProduct(const Vector& a, const Vector& b)
{
	return(a.x * b.x + a.y * b.y + a.z * b.z);
}

inline void AngleVectors(const Vector& angles, Vector* forward)
{
	float sp, sy, cp, cy;
	sp = ::sin(DEG2RAD(angles.x));
	sy = ::sin(DEG2RAD(angles.y));
	cp = ::cos(DEG2RAD(angles.x));
	cy = ::cos(DEG2RAD(angles.y));
	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}