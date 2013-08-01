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
	};
}

#endif