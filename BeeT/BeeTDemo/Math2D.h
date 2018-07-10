#ifndef __MATH_2D_H__
#define __MATH_2D_H__

#include <math.h>

template<class T>
class Point
{
public:
	T x;
	T y;

	Point() { x = 0; y = 0; }
	Point(const Point& p) { 
		this->x = p.x; this->y = p.y; 
	}
	Point(T x, T y) {
		this->x = x;
		this->y = y;
	}

	Point<T> operator+ (const Point<T>& p)const
	{
		Point<T> result;
		result.x = this->x + p.x;
		result.y = this->y + p.y;
		return result;
	}

	Point<T> operator- (const Point<T>& p)const
	{
		Point<T> result;
		result.x = this->x - p.x;
		result.y = this->y - p.y;
		return result;
	}

	Point<T> operator* (const Point<T>& p)const
	{
		Point<T> result;
		result.x = this->x * p.x;
		result.y = this->y * p.y;
		return result;
	}

	Point<T> operator/ (const Point<T>& p)const
	{
		Point<T> result;
		result.x = this->x / p.x;
		result.y = this->y / p.y;
		return result;
	}

	Point<T> operator* (int val)const
	{
		Point<T> result;
		result.x = this->x * val;
		result.y = this->y * val;
		return result;
	}

	Point<T> operator* (float val)const
	{
		Point<T> result;
		result.x = this->x * val;
		result.y = this->y * val;
		return result;
	}

	Point<T> operator/ (int val)const
	{
		Point<T> result;
		result.x = this->x / val;
		result.y = this->y / val;
		return result;
	}

	Point<T> operator/ (float val)const
	{
		Point<T> result;
		result.x = this->x / val;
		result.y = this->y / val;
		return result;
	}

	Point<T> operator+= (const Point<T>& p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	bool operator== (const Point<T>& p)
	{
		return (this->x == p.x && this->y == p->y);
	}

	Point<T> normalize()
	{
		float module = sqrt(x * x + y * y);
		this->x = x / module;
		this->y = y / module;
		return *this;
	}

	Point<T> normalized() const
	{
		Point<T> result;
		float module = sqrt(x * x + y * y);
		result.x = x / module;
		result.y = y / module;
		return result;
	}

	float DistanceTo(const Point<T>& p) const
	{
		float a = p.x - this->x;
		float b = p.y - this->y;
		return sqrt(a * a + b * b);
	}

};

typedef Point<int> iPoint;
typedef Point<int>	iVec;
typedef Point<float> fPoint;
typedef Point<float> fVec;

// Line
template <class T>
int PointInLine(const Point<T>& p, const Point<T>& a, const Point<T>& b)
{
	return (b.y - a.y) * p.x + (a.x - b.x) * p.y + (b.x * a.y - a.x * b.y);
}

// Sign
template <class T>
int GetSign(const T& number)
{
	return number >= 0.0f ? 1.0f : -1.0f;
}
#endif // !__MATH_2D_H__

