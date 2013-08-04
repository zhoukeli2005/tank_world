#ifndef _vector_h_
#define _vector_h_

#include "Config.h"

namespace math
{
	class Vector
	{
		public:
			Vector():x(0), y(0), z(0) { }
			
			Vector(float _x, float _y, float _z):x(_x), y(_y), z(_z) { }
			
			Vector(const Vector & v)
			{
				*this = v;
			}

			Vector & operator += (const Vector & v)
			{
				x += v.x;
				y += v.y;
				z += v.z;

				return *this;
			}

			Vector operator + (const Vector & v) const
			{
				Vector tmp = *this;
				tmp += v;
				return tmp;
			}

			Vector & operator = (const Vector & v)
			{
				x = v.x;
				y = v.y;
				z = v.z;

				return *this;
			}

			Vector operator - (const Vector & v) const
			{
				Vector tmp = *this;
				tmp -= v;
				return tmp;
			}

			Vector & operator -= (const Vector & v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;

				return *this;
			}

			float Length() const
			{
				return sqrt(x * x + y * y + z * z);
			}

			float operator * (const Vector & v) const
			{
				return x * v.x + y * v.y + z * v.z;
			}

			Vector & operator *= (const float d)
			{
				x *= d;
				y *= d;
				z *= d;

				return *this;
			}

			Vector operator * (const float d) const
			{
				Vector tmp = *this;
				tmp *= d;

				return tmp;
			}

			Vector & Normalize()
			{
				float len = Length();
				if(len > 0) {
					x /= len;
					y /= len;
					z /= len;
				}
				return *this;
			}


		public:
			float x, y, z;
	};
}

#endif