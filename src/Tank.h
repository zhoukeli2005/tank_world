#ifndef _tank_
#define _tank_

#include "GameObject.h"
#include "MeshGameObject.h"
#include "OctTree.h"
#include "Screen.h"

namespace game
{
	class TankWorld;

	class Tank : public engine::MeshGameObject, public engine::OctTreeData
	{
		// construct
		public:
			Tank(engine::Screen * scn, int flags) 
				: engine::MeshGameObject(scn)
				, engine::OctTreeData(flags) 
				, m_state(E_STATE_STOP)
				, m_last_tick(0)
				, m_move_velocity(0)
				, m_rotate_velocity(0)
				, m_range(0)
				, m_on_hit_times(0)
			{
				m_tank_world = (TankWorld *)scn; 
			}

		// destructor
		public:
			virtual ~Tank() { }

		// interface
		public:
			// 设置坦克的活动范围
			void SetRange(float range)
			{
				m_range = range;
			}

			void SetWhere(int w)
			{
				m_where = w;
			}

			void StartMove(float delta)
			{
				AddState(E_STATE_MOVE_ROTATE);
				m_move_velocity = delta;
			}

			void StartRotate(float delta)
			{
				AddState(E_STATE_MOVE_ROTATE);
				m_rotate_velocity = delta;
			}

			void StopRotate()
			{
				m_rotate_velocity = 0;
				if(!m_move_velocity) {
					RemoveState(E_STATE_MOVE_ROTATE);
				}
			}

			void StopMove()
			{
				m_move_velocity = 0;
				if(!m_rotate_velocity) {
					RemoveState(E_STATE_MOVE_ROTATE);
				}
			}

			int StartFire()
			{
				if(HaveState(E_STATE_FIRE)) {
					// cannot fire while firing
					return FALSE;
				}

				m_fire_time = GetTickCount();

				AddState(E_STATE_FIRE);
				MoveBackward(20);
				return TRUE;
			}

			void StartUnderHit(math::Vector from_direction)
			{
				from_direction.y = 0;
				if(m_parent) {
					m_under_hit_dir = m_parent->ToLocalPosition(from_direction);
				} else {
					m_under_hit_dir = from_direction;
				}
				m_under_hit_time = GetTickCount();
				m_under_hit_pos = GetLocalPosition();
				AddState(E_STATE_UNDER_HIT);
			}

		// 进行碰撞检测的重载函数
		public:
			virtual math::AABB OctTreeGetBoundBox()
			{
				math::AABB local = GetBoundBox();

				math::Vector pos = GetGlobalPosition();

				math::AABB out;
				out.SetMin(local.GetMin() + pos);
				out.SetMax(local.GetMax() + pos);

				return out;
			}

			virtual void OctTreeConflict(OctTreeData * other, math::Vector direction);


			/*
			 *	重写父类的MoveTo函数，使得每次移动都会在八叉树里进行碰撞检测
			 *
			 */
			void MoveTo(float x, float y, float z);



		// 重载 Draw 函数以进行逻辑运算
		public:
			void Draw();

		// internal
		protected:
			// 找到一个不会发生碰撞的位置
			void ResetPosition();
			void AddState(int state)
			{
				m_state |= state;
			}

			void RemoveState(int state)
			{
				m_state &= ~state;
			}

			int HaveState(int state)
			{
				return m_state & state;
			}

		// 逻辑，必须被重载
		private:
			// 被击中
			virtual void tank_OnHit(const math::Vector & dir) = 0;
			virtual void tank_OnCollideTank(Tank * other) = 0;
			virtual void tank_Update() = 0;

		// member
		protected:
			TankWorld * m_tank_world;

			enum {
				E_STATE_STOP,
				E_STATE_MOVE_ROTATE = 0x1,
				E_STATE_FIRE = 0x2,
				E_STATE_UNDER_HIT = 0x4
			};
			
			int m_state;

			float m_move_velocity;
			float m_rotate_velocity;

			int m_last_tick;
			int m_under_hit_time;
			int m_fire_time;
			math::Vector m_under_hit_dir;
			math::Vector m_under_hit_pos;

			int m_range;

			int m_on_hit_times;

			int m_where;
	};
}

#endif