#ifndef __BASICSTRUCTS_H__
#define __BASICSTRUCTS_H__

struct float2
{
	float x;
	float y;

	float2() { x = 0.0f; y = 0.0f; }
	float2(float x, float y) { this->x = x; this->y = y; }
};

struct float3
{
	float x;
	float y;
	float z;

	float3() { x = y = z = 0.0f; }
	float3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
};
#endif // !__BASICSTRUCTS_H__

