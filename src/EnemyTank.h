#ifndef _enemy_tank_
#define _enemy_tank_

#include "Tank.h"

namespace game
{
	class EnemyTank : public Tank
	{
	public:	
		EnemyTank(engine::Screen * scn) : Tank(scn, E_TT_TANK_ENEMY), m_last_time(0) { }

	public:
		EnemyTank * CopyTank()
		{
			EnemyTank * m = new EnemyTank(m_screen);
			Copy((MeshGameObject *)m);

			return m;
		}

	private:

		void tank_OnHit(const math::Vector & dir)
		{
			StartUnderHit(dir);

			if(m_on_hit_times == 1) {
				m_tank_world->SetRemark("Good!!!击中敌人!!敌人轻伤！");
			} else if (m_on_hit_times == 2) {
				m_tank_world->SetRemark("Perfect!!!击中敌人!!敌人重伤！");
			} else if (m_on_hit_times == 3) {
				m_tank_world->SetRemark("Awesome!!!敌人被歼灭!");
			}

			if(m_on_hit_times == 3) {
				Dispear();
			}
		}

		void tank_OnCollideTank(Tank * other)
		{
			StopMove();
			StopRotate();
		}

		void tank_Update()
		{
			int now = GetTickCount();
			if(!m_last_time) {
				m_last_time = now;
			}
			int delta = now - m_last_time;

			if(delta > 3000) {
				m_last_time = now;
				int r = rand() % 100;
				if(r < 20) {
					// keep on
				} else
				if(r < 50) {
					StartMove(30);
				} else
				if(r < 70) {
					StartRotate(30);
				} else
				if(r < 80) {
					StopMove();
				} else
				if(r < 100) {
					StopRotate();
				}
			}
		}

		void Dispear()
		{
			// 1. remove from screen
			RemoveFromScreen();

			// 2. remove from Octree
			m_tank_world->GetOctTree()->Remove(this);

			// 3. drop
			Drop();

			// 4. Create New One
			m_tank_world->CreateNewTank();
		}

		private:
			int m_last_time;
	};
}

#endif