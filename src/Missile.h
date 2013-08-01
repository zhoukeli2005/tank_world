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

			virtual void OctTreeConflict(OctTreeData * other, math::Vector direction)
			{
				// �Լ���ʧ
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
			 *	��д�����MoveTo������ʹ��ÿ���ƶ������ڰ˲����������ײ���
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

				// �Լ���ʧ
				Dispear();
			}

		// ���� Draw �����Խ����߼�����
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

				// �ƶ�
				math::Vector dis = m_curr_velocity * delta;
				
				Move(dis.x, dis.y, dis.z);

				// �ı��ٶ�
				m_curr_velocity += math::Vector(0, -5 * delta, 0);

				// ��ʧ
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

		protected:
			virtual D3DXMATRIX iGetLocalMatrix()
			{
				D3DXMATRIX mscale, mrotate, mtrans, out;
	
				D3DXMatrixScaling(&mscale, m_scale.x, m_scale.y, m_scale.z);

			//	D3DXMatrixRotationYawPitchRoll(&mrotate, D3DXToRadian(m_rotate.y), D3DXToRadian(m_rotate.x), D3DXToRadian(m_rotate.z));
				// �Դ�
				static int angle = 0;
				D3DXMatrixIdentity(&mrotate);
		//		D3DXMatrixRotationX(&mrotate, angle++);

				D3DXMATRIX tmp;
				float a = atan(-m_curr_velocity.y / m_curr_velocity.x);
				float x = cos(a) - sin(a);
				float z = sin(a) + cos(a);

				float b = math::Vector(x, 0, z) * math::Vector(1, 0, 0);
				b = acos(b);

				D3DXMatrixRotationY(&tmp, b);

				mrotate *= tmp;

				D3DXMatrixRotationZ(&tmp, -a);
				mrotate *= tmp;

				D3DXMatrixTranslation(&mtrans, m_position.x, m_position.y, m_position.z);

				out = mscale * mrotate * mtrans;

				return out;
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


			// ��������������Ϊ���˶��켣����
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