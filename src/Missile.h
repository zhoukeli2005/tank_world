#ifndef _missile_h_
#define _missile_h_

#include "GameObject.h"
#include "MeshGameObject.h"
#include "OctTree.h"
#include "TankWorld.h"

namespace game
{
	class Missile : public engine::MeshGameObject, public engine::OctTreeData
	{
		// constructor
		public:
			Missile(engine::Screen * scn, int flags) 
				: MeshGameObject(scn)
				, OctTreeData(flags)
				, m_curr_velocity()
				, m_last_time(0)
			{
				m_tank_world = (TankWorld *)scn;
				ScaleTo(0.03, 0.03, 0.03);
			}

			~Missile() 
			{
			}

		// interface
		public:
			void Start(const math::Vector & pos, const math::Vector & dir, float velocity)
			{
				MeshGameObject::MoveTo(pos.x, pos.y, pos.z);

				m_origin_pos = pos;
				m_dir = dir;
				m_dir.Normalize();
				m_velocity = velocity;
				m_start_time = GetTickCount();

				m_curr_velocity = m_dir * m_velocity;

				ResetFace(m_curr_velocity);
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

			virtual void OctTreeConflict(OctTreeData * other, math::Vector direction)
			{
				// 自己消失
				int flags = other->GetFlags();
				if(flags == E_TT_MISSILE_ENEMY || flags == E_TT_MISSILE_PLAYER) {
					return;
				}
				if(GetFlags() == E_TT_MISSILE_PLAYER && flags == E_TT_TANK_PLAYER) {
					return;
				}
				Dispear();
			}


			/*
			 *	重写父类的MoveTo函数，使得每次移动都会在八叉树里进行碰撞检测
			 *
			 */
			void MoveTo(float x, float y, float z)
			{
				math::Vector origin = GetLocalPosition();

				MeshGameObject::MoveTo(x, y, z);

				math::Vector direction = math::Vector(x - origin.x, y - origin.y, z - origin.z);

				engine::OctTreeData * other = m_tank_world->GetOctTree()->CheckConflict(this, direction);
				if(!other) {
					return;
				}

				int flags = other->GetFlags();
				if(flags == E_TT_MISSILE_PLAYER || flags == E_TT_MISSILE_ENEMY) {
					return;
				}
				if(GetFlags() == E_TT_MISSILE_PLAYER && flags == E_TT_TANK_PLAYER) {
					return;
				}

				// 自己消失
				Dispear();
			}

		// 重载 Draw 函数以进行逻辑运算
		public:
			void Draw()
			{
				MeshGameObject::Draw();

				int now = GetTickCount();
				if(!m_last_time) {
					m_last_time = now;
				}

				float delta = (now - m_last_time) * 0.001;

				m_last_time = now;

				// 自传
				Rotate(2, 0, 0);

				// 朝向
				ResetFace(m_curr_velocity);

				// 移动
				math::Vector dis = m_curr_velocity * delta;
				
				Move(dis.x, dis.y, dis.z);

				// 改变速度
				m_curr_velocity += math::Vector(0, -5 * delta, 0);

				// 消失
				if(now - m_start_time > 30000) {
					Dispear();
				}
			}

		public:
			Missile * CopyMissile()
			{
				Missile * m = new Missile(m_screen, GetFlags());
				Copy((MeshGameObject *)m);

				return m;
			}

		private:
			void Dispear()
			{
				// 1. remove from screen
				RemoveFromScreen();

				// 2. remove from octree
				m_tank_world->GetOctTree()->Remove(this);

				// 3. drop
				Drop();
			}


			// 将导弹方向设置为其运动轨迹切线
			void ResetFace(const math::Vector & dir)
			{
				float tmp = dir * math::Vector(1, 0, 0) / dir.Length();
				float a = D3DXToDegree(acos(tmp));

				if(dir.z > 0) {
					a = 360 - a;
				}

				math::Vector r = GetLocalRotate();

				RotateTo(0, a, 0);
			}

		private:
			TankWorld * m_tank_world;
			
			math::Vector m_dir;
			float m_velocity;
			int m_start_time;
			int m_last_time;

			math::Vector m_curr_velocity;

			math::Vector m_origin_pos;
	};
}

#endif