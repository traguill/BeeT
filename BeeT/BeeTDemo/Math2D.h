#ifndef __MATH_2D_H__
#define __MATH_2D_H__

#include <math.h>

#define PI 3.14159265f

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
	~Point(){}

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

	bool operator== (const Point<T>& b) const
	{
		return (this->x == b.x && this->y == b.y);
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

	float AngleBetween(const Point<T>& p) const
	{
		float dot = x * p.x + y * p.y;
		float det = x * p.y - y * p.x;
		return atan2(det, dot) * 180.0f / PI;
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

// Trace a line and get all intersected tiles
inline void TraceLine(float x1, float y1, float x2, float y2, std::vector<Point<int>>& tiles)
{
	tiles.clear();
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x<maxX; x++)
	{
		if (steep)
		{
			tiles.push_back(iPoint(y, x));
		}
		else
		{
			tiles.push_back(iPoint(x, y));
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}
#endif // !__MATH_2D_H__

