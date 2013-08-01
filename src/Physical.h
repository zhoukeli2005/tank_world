#ifndef _physical_h_
#define _physical_h_

#include "GameObject.h"
#include "Vector.h"

namespace engine
{
	class Physical
	{
		// constructor
		public:
			Physical();

		// destructor
		public:
			~Physical();

		// interface
		public:
			void SetTarget(GameObject *);
			void EnableGravity(bool b);

			void SetVelocity(float x, float y, float z);
			void SetAcceleration(float x, float y, float z);

		// internal -- for Physical Manager
		private:
			void Update(int msec);

		// members
		private:
			math::Vector	m_last_pos;
			int				m_last_msec;

			math::Vector	m_origin_velocity;
			math::Vector	m_acceleration;

			Physical * m_prev;
			Physical * m_next;

		// Physical Manager
		private:
			class PhysicalManager {
				public:
					PhysicalManager();

				public:
					void AddPhysical(Physical * phy);

					void RemovePhysical(Physical * phy);

				private:
					Physical * m_first_physical;
			};

			friend class PhysicalManager;

			static PhysicalManager g_physical_manager;
	};
}

#endif