#ifndef _vector_h_
#define _vector_h_

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

		public:
			float x, y, z;
	};
}

#endif