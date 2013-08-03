#ifndef _missile_h_
#define _missile_h_

#include "GameObject.h"
#include "MeshGameObject.h"
#include "OctTree.h"
#include "TankWorld.h"
#include "Util.h"

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
				, m_last_face_time(0)
				, m_is_dispearing(0)
			{
				m_tank_world = (TankWorld *)scn;
				ScaleTo(0.01, 0.01, 0.01);
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
				math::AABB local;
				local.SetMin(math::Vector(-1, -1, -1));
				local.SetMax(math::Vector(1, 1, 1));

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

				// 移动
				math::Vector dis = m_curr_velocity * delta;
								
				Move(dis.x, dis.y, dis.z);

				ResetFace(m_curr_velocity);

				// 改变速度
				m_curr_velocity += math::Vector(0, -100 * delta, 0);

				// 消失
				if(now - m_start_time > MissileExistDuration) {
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

		protected:

		private:
			void Dispear()
			{
				if(m_is_dispearing){
					return;
				}
				m_is_dispearing = 1;

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
				math::Vector v = dir;
				float rotateX = -90;
				float len = v.Length();
				float tmp;
				if(len > 0) {
					tmp = v * math::Vector(0, -1, 0) / len;
					rotateX = D3DXToDegree(acos(tmp));
					if(dir.z > 0) {
						rotateX = 360 - rotateX;
					}
				}
				

				v = dir;
				v.y = 0;
				float rotateY = 0;
				len = v.Length();
				if(len > 0) {
					math::Vector vv(0, 0, -1);
					if(dir.z > 0) {
						vv.z = 1;
					}
					tmp = v * vv / len;
					rotateY = D3DXToDegree(acos(tmp));

					if(dir.z > 0 && dir.x < 0) {
						rotateY = 360 - rotateY;
					}
					if(dir.z < 0 && dir.x > 0) {
						rotateY = 360 - rotateY;
					}
				}

				RotateTo(rotateX, rotateY, 0);
			}

		private:
			TankWorld * m_tank_world;
			
			math::Vector m_dir;
			float m_velocity;
			int m_start_time;
			int m_last_time;

			math::Vector m_curr_velocity;

			math::Vector m_origin_pos;

			int m_last_face_time;

			int m_is_dispearing;
	};
}

#endif