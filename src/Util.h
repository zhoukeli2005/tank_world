#ifndef _util_h_
#define _util_h_

#include "Config.h"
#include "Vector.h"

namespace math
{
	class Util
	{
		public:
			static D3DXVECTOR3 vect3transform(const D3DXVECTOR3 & v, const D3DXMATRIX & matrix)
			{
				D3DXVECTOR4 v4;
				D3DXVec3Transform(&v4, &v, &matrix);

				return D3DXVECTOR3(v4.x, v4.y, v4.z);
			}

			static void Init()
			{
				int i;
				for(i = 0; i < 90; ++i) {
					float r = D3DXToRadian(i);
					m_sin[i] = sin(r);
					m_cos[i] = cos(r);
					m_tan[i] = tan(r);
				}
			}

			static float quick_sin(float degree)
			{
				while(degree < 0) {
					degree += 360;
				}
				while(degree >= 360) {
					degree -= 360;
				}

				int sgn = 1;
				if(degree > 90 && degree <= 180) {
					degree = 180 - degree;
				} else if (degree > 180 && degree <= 270) {
					degree = degree - 180;
					sgn = -1;
				} else if (degree > 270) {
					degree = 360 - degree;
					sgn = -1;
				}

				if(degree == 90) {
					return 1;
				}

				return m_sin[(int)degree] * sgn;
			}

			static float quick_cos(float degree)
			{
				while(degree < 0) {
					degree += 360;
				}
				while(degree >= 360) {
					degree -= 360;
				}

				int sgn = 1;
				if(degree > 90 && degree <= 180) {
					degree = 180 - degree;
					sgn = -1;
				} else if (degree > 180 && degree <= 270) {
					degree = degree - 180;
					sgn = -1;
				} else if (degree > 270) {
					degree = 360 - degree;
				}

				if(degree == 90) {
					return 0;
				}

				return m_cos[(int)degree] * sgn;
			}
			

		private:
			static float m_sin[90];
			static float m_cos[90];
			static float m_tan[90];
	};
}

#endif