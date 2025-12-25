#if !defined(IncSDKTypes_Included)
#define IncSDKTypes_Included

#include "Misc/BaseTypes.h"

#define PI 3.14159265358979323846
#define M_RADPI 57.295779513082
#define DEG2RAD(x) ((float)(x) * (float)((float)(PI) / 180.f))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / (float)(PI)))

typedef struct Vec2
{
	float x, y;
} Vec2;

typedef Vec2 Vector2D;

typedef struct Vec3
{
	float x, y, z;
} Vec3;

typedef Vec3 Vector;
typedef Vec3 QAngle;
typedef Vec3 RadianEuler;
typedef Vec3 AngularImpulse;

typedef struct Vector4D
{
	float x, y, z, w;
} Vector4D;

typedef Vector4D Quaternion;

typedef struct VectorAligned
{
	float x, y, z, w;
} VectorAligned __attribute__((aligned(16)));

typedef struct VMatrix
{
	float m[4][4];
} VMatrix;

typedef struct matrix3x4_t
{
	float m[3][4];
} matrix3x4_t;

typedef matrix3x4_t matrix3x4;

typedef color32 Color_t;

typedef struct FColor_t
{
	float r, g, b, a;
} FColor_t;

#endif
