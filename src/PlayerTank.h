#ifndef _player_tank_
#define _player_tank_

#include "Tank.h"

namespace game
{
	class PlayerTank : public Tank
	{
		public:	
			PlayerTank(engine::Screen * scn, int flags) : Tank(scn, flags) { }

		private:
			void tank_OnHit()
			{

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