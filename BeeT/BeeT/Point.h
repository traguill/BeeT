#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>

template<class TYPE>
class Point
{
public:

	Point<TYPE>() : x(0), y(0)
	{}

	Point<TYPE>(TYPE x, TYPE y) : x(x), y(y)
	{}

	Point<TYPE> operator + (const Point<TYPE>& p2)const
	{
		Point<TYPE> p;
		p.x = this->x + p2.x;
		p.y = this->y + p2.y;

		return p;
	}
	
	Point<TYPE> operator - (const Point<TYPE>& p2)const
	{
		Point<TYPE> p;
		p.x = this->x - p2.x;
		p.y = this->y - p2.y;

		return p;
	}

	const Point<TYPE>& operator += (const Point<TYPE>& p2)
	{
		x += p2.x;
		y += p2.y;

		return (*this);
	}

	const Point<TYPE>& operator -= (const Point<TYPE>& p2)
	{
		x -= p2.x;
		y -= p2.y;

		return (*this);
	}

	Point<TYPE> operator * (const TYPE& u)const
	{
		Point<TYPE> p;
		p.x = this->x * u;
		p.y = this->y * u;

		return p;
	}

	int Lengthi()const
	{
		return sqrt((x*x) + (y*y));
	}

	float Lengthf()const
	{
		float x2 = (float)x*(float)x;
		float y2 = (float)y*(float)y;
		float sum = x2 + y2;
		return sqrtf(sum);
	}

	float DistanceTo(const Point<TYPE>& p2)const
	{
		TYPE w = p2.x - x;
		TYPE h = p2.y - y;

		return sqrtf(powf((float)w, 2) + powf((float)h, 2));
	}

public:
	TYPE x;
	TYPE y;
};

#endif // !__POINT_H__



