#ifndef _aabb_h_
#define _aabb_h_

#include "Vector.h"

namespace math
{
	class AABB
	{
		public:
			AABB() : m_min(), m_max() { }
		public:
			~AABB() {}

		public:
			void SetFromD3DVector(const D3DXVECTOR3 & min, const D3DXVECTOR3 & max)
			{
				m_min.x = min.x;
				m_min.y = min.y;
				m_min.z = min.z;

				m_max.x = max.x;
				m_max.y = max.y;
				m_max.z = max.z;
			}

			void AddPoin(float x, float y, float z)
			{
				m_min.x = min(m_min.x, x);
				m_min.y = min(m_min.y, y);
				m_min.z = min(m_min.z, z);

				m_max.x = max(m_max.x, x);
				m_max.y = max(m_max.y, y);
				m_max.z = max(m_max.z, z);
			}

			int CheckConflict(const AABB & other)
			{
				if(	m_min.x <= other.m_max.x && m_min.y <= other.m_max.y && m_min.z <= other.m_max.z &&
					m_max.x >= other.m_min.x && m_max.y >= other.m_min.y && m_max.z >= other.m_min.z) {
						return TRUE;
				}
				return FALSE;
			}

			const Vector & GetMin() const { return m_min; }
			const Vector & GetMax() const { return m_max; }

			void SetMin(const Vector & _min) { m_min = _min; }
			void SetMax(const Vector & _max) { m_max = _max; }

		private:
			Vector m_min;
			Vector m_max;
	};
}

#endif