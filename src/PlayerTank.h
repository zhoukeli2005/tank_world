#ifndef _player_tank_
#define _player_tank_

#include "Tank.h"

namespace game
{
	class PlayerTank : public Tank
	{
		public:	
			PlayerTank(engine::Screen * scn) : Tank(scn, E_TT_TANK_PLAYER) { }

		public:
			PlayerTank * CopyTank()
			{
				PlayerTank * m = new PlayerTank(m_screen);
				Copy((MeshGameObject *)m);

				return m;
			}

		private:
			void tank_OnHit(const math::Vector & dir)
			{
				StartUnderHit(dir);
			}

			void tank_OnCollideTank(Tank * other)
			{

			}

			void tank_Update()
			{

			}
	};
}

#endif