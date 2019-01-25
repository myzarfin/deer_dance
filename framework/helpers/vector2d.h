#pragma once
#include "vector.h"

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

class Vector2
{
public:
	float x = 0, y = 0;
	Vector2() {}
	Vector2(float _x, float _y)
	{
		x = _x, y = _y;
	}

	float operator[](int i) const;
	float& operator[](int i);

	bool operator==(const Vector2& src) const;
	bool operator!=(const Vector2& src) const;

	__forceinline Vector2&	operator+=(float fl);
	__forceinline Vector2&	operator-=(float fl);

	__forceinline Vector2&	operator+=(const Vector2 &v);
	Vector2	operator+(const Vector2& v) const;

	Vector2& operator=(const Vector2 &vOther);
};

inline float Vector2::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
inline float& Vector2::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

inline bool Vector2::operator==(const Vector2& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y);
}
inline bool Vector2::operator!=(const Vector2& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y);
}

__forceinline Vector2&	Vector2::operator+=(float fl)
{
	x += fl;
	y += fl;
	CHECK_VALID(*this);
	return *this;
}

__forceinline  Vector2& Vector2::operator+=(const Vector2& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y;
	return *this;
}

inline Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 res;
	res.x = x + v.x;
	res.y = y + v.y;
	return res;
}
//===============================================
__forceinline Vector2&	Vector2::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	CHECK_VALID(*this);
	return *this;
}

inline Vector2& Vector2::operator=(const Vector2 &vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y;
	return *this;
}

#ifndef VECTOR2D_H
#define VECTOR2D_H

#ifdef _WIN32
#pragma once
#endif

typedef float vec_t;

#include <math.h>
#include <float.h>

// For rand(). We really need a library!
#include <stdlib.h>

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

#define VALVE_RAND_MAX 0x7fff

//=========================================================
// 2D Vector2D
//=========================================================

class vec2_t
{
public:
	// Members
	vec_t x, y;

	// Construction/destruction
	vec2_t(void);
	vec2_t(vec_t X, vec_t Y);
	vec2_t(const float *pFloat);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);

	// equality
	bool operator==(const vec2_t& v) const;
	bool operator!=(const vec2_t& v) const;

	// arithmetic operations
	vec2_t&	operator+=(const vec2_t &v);
	vec2_t&	operator-=(const vec2_t &v);
	vec2_t&	operator*=(const vec2_t &v);
	vec2_t&	operator*=(float s);
	vec2_t&	operator/=(const vec2_t &v);
	vec2_t&	operator/=(float s);

	// negate the Vector2D components
	void	Negate();

	// Get the Vector2D's magnitude.
	vec_t	Length() const;

	// Get the Vector2D's magnitude squared.
	vec_t	LengthSqr(void) const;

	// return true if this vector is (0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance);
	}

	// Normalize in place and return the old length.
	vec_t	NormalizeInPlace();

	// Compare length.
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// Get the distance from this Vector2D to the other one.
	vec_t	DistTo(const vec2_t &vOther) const;

	// Get the distance from this Vector2D to the other one squared.
	vec_t	DistToSqr(const vec2_t &vOther) const;

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector2D equation (because it's done per-component
	// rather than per-Vector2D).
	void	MulAdd(const vec2_t& a, const vec2_t& b, float scalar);

	// Dot product.
	vec_t	Dot(const vec2_t& vOther) const;

	// assignment
	vec2_t& operator=(const vec2_t &vOther);

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// copy constructors
	vec2_t(const vec2_t &vOther);

	// arithmetic operations
	vec2_t	operator-(void) const;

	vec2_t	operator+(const vec2_t& v) const;
	vec2_t	operator-(const vec2_t& v) const;
	vec2_t	operator*(const vec2_t& v) const;
	vec2_t	operator/(const vec2_t& v) const;
	vec2_t	operator*(float fl) const;
	vec2_t	operator/(float fl) const;

	// Cross product between two vectors.
	vec2_t	Cross(const vec2_t &vOther) const;

	// Returns a Vector2D with the min or max in X, Y, and Z.
	vec2_t	Min(const vec2_t &vOther) const;
	vec2_t	Max(const vec2_t &vOther) const;

#else

private:
	// No copy constructors allowed if we're in optimal mode
	Vector2D(const Vector2D& vOther);
#endif
};

//-----------------------------------------------------------------------------

const vec2_t vec2_origin(0, 0);
const vec2_t vec2_invalid(3.402823466e+38F, 3.402823466e+38F);

//-----------------------------------------------------------------------------
// Vector2D related operations
//-----------------------------------------------------------------------------

// Vector2D clear
void Vector2DClear(vec2_t& a);

// Copy
void Vector2DCopy(const vec2_t& src, vec2_t& dst);

// Vector2D arithmetic
void Vector2DAdd(const vec2_t& a, const vec2_t& b, vec2_t& result);
void Vector2DSubtract(const vec2_t& a, const vec2_t& b, vec2_t& result);
void Vector2DMultiply(const vec2_t& a, vec_t b, vec2_t& result);
void Vector2DMultiply(const vec2_t& a, const vec2_t& b, vec2_t& result);
void Vector2DDivide(const vec2_t& a, vec_t b, vec2_t& result);
void Vector2DDivide(const vec2_t& a, const vec2_t& b, vec2_t& result);
void Vector2DMA(const vec2_t& start, float s, const vec2_t& dir, vec2_t& result);

// Store the min or max of each of x, y, and z into the result.
void Vector2DMin(const vec2_t &a, const vec2_t &b, vec2_t &result);
void Vector2DMax(const vec2_t &a, const vec2_t &b, vec2_t &result);

#define Vector2DExpand( v ) (v).x, (v).y

// Normalization
vec_t Vector2DNormalize(vec2_t& v);

// Length
vec_t Vector2DLength(const vec2_t& v);

// Dot Product
vec_t DotProduct2D(const vec2_t& a, const vec2_t& b);

// Linearly interpolate between two vectors
void Vector2DLerp(const vec2_t& src1, const vec2_t& src2, vec_t t, vec2_t& dest);


//-----------------------------------------------------------------------------
//
// Inlined Vector2D methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline vec2_t::vec2_t(void)
{
	//#ifdef _DEBUG
	//	// Initialize to NAN to catch errors
	//	x = y = VEC_T_NAN;
	//#endif
}

inline vec2_t::vec2_t(vec_t X, vec_t Y)
{
	x = X; y = Y;
	Assert(IsValid());
}

inline vec2_t::vec2_t(const float *pFloat)
{
	Assert(pFloat);
	x = pFloat[0]; y = pFloat[1];
	Assert(IsValid());
}


//-----------------------------------------------------------------------------
// copy constructor
//-----------------------------------------------------------------------------

inline vec2_t::vec2_t(const vec2_t &vOther)
{
	Assert(vOther.IsValid());
	x = vOther.x; y = vOther.y;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void vec2_t::Init(vec_t ix, vec_t iy)
{
	x = ix; y = iy;
	Assert(IsValid());
}

inline void vec2_t::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline void Vector2DClear(vec2_t& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline vec2_t& vec2_t::operator=(const vec2_t &vOther)
{
	Assert(vOther.IsValid());
	x = vOther.x; y = vOther.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

inline vec_t& vec2_t::operator[](int i)
{
	Assert((i >= 0) && (i < 2));
	return ((vec_t*)this)[i];
}

inline vec_t vec2_t::operator[](int i) const
{
	Assert((i >= 0) && (i < 2));
	return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

inline vec_t* vec2_t::Base()
{
	return (vec_t*)this;
}

inline vec_t const* vec2_t::Base() const
{
	return (vec_t const*)this;
}

inline unsigned long& FloatBits(vec_t& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline bool IsFinite(vec_t f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool vec2_t::IsValid() const
{
	return IsFinite(x) && IsFinite(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool vec2_t::operator==(const vec2_t& src) const
{
	Assert(src.IsValid() && IsValid());
	return (src.x == x) && (src.y == y);
}

inline bool vec2_t::operator!=(const vec2_t& src) const
{
	Assert(src.IsValid() && IsValid());
	return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

inline void Vector2DCopy(const vec2_t& src, vec2_t& dst)
{
	Assert(src.IsValid());
	dst.x = src.x;
	dst.y = src.y;
}

inline void	vec2_t::CopyToArray(float* rgfl) const
{
	Assert(IsValid());
	Assert(rgfl);
	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------

inline void vec2_t::Negate()
{
	Assert(IsValid());
	x = -x; y = -y;
}

inline vec2_t& vec2_t::operator+=(const vec2_t& v)
{
	Assert(IsValid() && v.IsValid());
	x += v.x; y += v.y;
	return *this;
}

inline vec2_t& vec2_t::operator-=(const vec2_t& v)
{
	Assert(IsValid() && v.IsValid());
	x -= v.x; y -= v.y;
	return *this;
}

inline vec2_t& vec2_t::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	Assert(IsValid());
	return *this;
}

inline vec2_t& vec2_t::operator*=(const vec2_t& v)
{
	x *= v.x;
	y *= v.y;
	Assert(IsValid());
	return *this;
}

inline vec2_t& vec2_t::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	Assert(IsValid());
	return *this;
}

inline vec2_t& vec2_t::operator/=(const vec2_t& v)
{
	Assert(v.x != 0.0f && v.y != 0.0f);
	x /= v.x;
	y /= v.y;
	Assert(IsValid());
	return *this;
}

inline void Vector2DAdd(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

inline void Vector2DSubtract(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

inline void Vector2DMultiply(const vec2_t& a, vec_t b, vec2_t& c)
{
	Assert(a.IsValid() && IsFinite(b));
	c.x = a.x * b;
	c.y = a.y * b;
}

inline void Vector2DMultiply(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}


inline void Vector2DDivide(const vec2_t& a, vec_t b, vec2_t& c)
{
	Assert(a.IsValid());
	Assert(b != 0.0f);
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

inline void Vector2DDivide(const vec2_t& a, const vec2_t& b, vec2_t& c)
{
	Assert(a.IsValid());
	Assert((b.x != 0.0f) && (b.y != 0.0f));
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

inline void Vector2DMA(const vec2_t& start, float s, const vec2_t& dir, vec2_t& result)
{
	Assert(start.IsValid() && IsFinite(s) && dir.IsValid());
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
inline void	vec2_t::MulAdd(const vec2_t& a, const vec2_t& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

inline void Vector2DLerp(const vec2_t& src1, const vec2_t& src2, vec_t t, vec2_t& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
inline vec_t DotProduct2D(const vec2_t& a, const vec2_t& b)
{
	Assert(a.IsValid() && b.IsValid());
	return(a.x*b.x + a.y*b.y);
}

// for backwards compatability
inline vec_t vec2_t::Dot(const vec2_t& vOther) const
{
	return DotProduct2D(*this, vOther);
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline vec_t Vector2DLength(const vec2_t& v)
{
	Assert(v.IsValid());
	return (vec_t)sqrt(v.x*v.x + v.y*v.y);
}

inline vec_t vec2_t::LengthSqr(void) const
{
	Assert(IsValid());
	return (x*x + y * y);
}

inline vec_t vec2_t::NormalizeInPlace()
{
	return Vector2DNormalize(*this);
}

inline bool vec2_t::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val*val;
}

inline bool vec2_t::IsLengthLessThan(float val) const
{
	return LengthSqr() < val*val;
}

inline vec_t vec2_t::Length(void) const
{
	return Vector2DLength(*this);
}


inline void Vector2DMin(const vec2_t &a, const vec2_t &b, vec2_t &result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}


inline void Vector2DMax(const vec2_t &a, const vec2_t &b, vec2_t &result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}


//-----------------------------------------------------------------------------
// Normalization
//-----------------------------------------------------------------------------
inline vec_t Vector2DNormalize(vec2_t& v)
{
	Assert(v.IsValid());
	vec_t l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f;
	}
	return l;
}


//-----------------------------------------------------------------------------
// Get the distance from this Vector2D to the other one 
//-----------------------------------------------------------------------------
inline vec_t vec2_t::DistTo(const vec2_t &vOther) const
{
	vec2_t delta;
	Vector2DSubtract(*this, vOther, delta);
	return delta.Length();
}

inline vec_t vec2_t::DistToSqr(const vec2_t &vOther) const
{
	vec2_t delta;
	Vector2DSubtract(*this, vOther, delta);
	return delta.LengthSqr();
}


//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
inline void ComputeClosestPoint2D(const vec2_t& vecStart, float flMaxDist, const vec2_t& vecTarget, vec2_t *pResult)
{
	vec2_t vecDelta;
	Vector2DSubtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist)
	{
		*pResult = vecTarget;
	}
	else
	{
		vecDelta /= sqrt(flDistSqr);
		Vector2DMA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}



//-----------------------------------------------------------------------------
//
// Slow methods
//
//-----------------------------------------------------------------------------

#ifndef VECTOR_NO_SLOW_OPERATIONS

//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

inline vec2_t vec2_t::Min(const vec2_t &vOther) const
{
	return vec2_t(x < vOther.x ? x : vOther.x,
		y < vOther.y ? y : vOther.y);
}

inline vec2_t vec2_t::Max(const vec2_t &vOther) const
{
	return vec2_t(x > vOther.x ? x : vOther.x,
		y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

inline vec2_t vec2_t::operator-(void) const
{
	return vec2_t(-x, -y);
}

inline vec2_t vec2_t::operator+(const vec2_t& v) const
{
	vec2_t res;
	Vector2DAdd(*this, v, res);
	return res;
}

inline vec2_t vec2_t::operator-(const vec2_t& v) const
{
	vec2_t res;
	Vector2DSubtract(*this, v, res);
	return res;
}

inline vec2_t vec2_t::operator*(float fl) const
{
	vec2_t res;
	Vector2DMultiply(*this, fl, res);
	return res;
}

inline vec2_t vec2_t::operator*(const vec2_t& v) const
{
	vec2_t res;
	Vector2DMultiply(*this, v, res);
	return res;
}

inline vec2_t vec2_t::operator/(float fl) const
{
	vec2_t res;
	Vector2DDivide(*this, fl, res);
	return res;
}

inline vec2_t vec2_t::operator/(const vec2_t& v) const
{
	vec2_t res;
	Vector2DDivide(*this, v, res);
	return res;
}

inline vec2_t operator*(float fl, const vec2_t& v)
{
	return v * fl;
}

#endif //slow
#endif // VECTOR2D_H