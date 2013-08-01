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
			{
				m_tank_world = (TankWorld *)scn; 
			}

		// destructor
		public:
			virtual ~Tank() { }

		// interface
		public:
			void StartMove(float delta)
			{
				m_state = E_STATE_MOVE_ROTATE;
				m_move_velocity = delta;
			}

			void StartRotate(float delta)
			{
				m_state = E_STATE_MOVE_ROTATE;
				m_rotate_velocity = delta;
			}

			void StopRotate()
			{
				m_rotate_velocity = 0;
				if(!m_move_velocity) {
					m_state = E_STATE_STOP;
				}
			}

			void StopMove()
			{
				m_move_velocity = 0;
				if(!m_rotate_velocity) {
					m_state = E_STATE_STOP;
				}
			}

			void StartFire()
			{
				if(m_state == E_STATE_FIRE) {
					// cannot fire while firing
					return;
				}

				m_state = E_STATE_FIRE;
				m_move_velocity = m_rotate_velocity = 0;
			}

			void StartUnderHit(const math::Vector & from_direction)
			{
				m_under_hit_dir = from_direction;
				m_under_hit_time = GetTickCount();
				m_under_hit_pos = GetLocalPosition();
				m_state = E_STATE_UNDER_HIT;
			}

		// ������ײ�������غ���
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
			 *	��д�����MoveTo������ʹ��ÿ���ƶ������ڰ˲����������ײ���
			 *
			 */
			void MoveTo(float x, float y, float z);



		// ���� Draw �����Խ����߼�����
		public:
			void Draw();

		// internal
		protected:
			// �ҵ�һ�����ᷢ����ײ��λ��
			void ResetPosition();

		// �߼������뱻����
		private:
			// ������
			virtual void tank_OnHit() = 0;
			virtual void tank_OnCollideTank(Tank * other) = 0;
			virtual void tank_Update() = 0;

		// member
		protected:
			TankWorld * m_tank_world;

			enum {
				E_STATE_STOP,
				E_STATE_MOVE_ROTATE,
				E_STATE_FIRE,
				E_STATE_UNDER_HIT
			};
			int m_state;

			float m_move_velocity;
			float m_rotate_velocity;

			int m_last_tick;
			int m_under_hit_time;
			math::Vector m_under_hit_dir;
			math::Vector m_under_hit_pos;
	};
}

#endif