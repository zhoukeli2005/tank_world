#ifndef _camera_h_
#define _camera_h_

#include "Config.h"
#include "GameObject.h"
#include "Screen.h"
#include "Util.h"

namespace engine
{
	class Camera : public GameObject
	{
		public:
			Camera(Screen * scn):GameObject(scn), m_use_lookat(0) {}

		public:
			~Camera() {}

		public:
			virtual void Move(float dx, float dy, float dz)
			{
				GameObject::Move(dx, dy, dz);

				iRefresh();
			}

			virtual void MoveTo(float x, float y, float z)
			{
				GameObject::MoveTo(x, y, z);

				iRefresh();
			}
			
			virtual void Rotate(float dx, float dy, float dz)
			{
				m_use_lookat = 0;

				GameObject::Rotate(dx, dy, dz);

				iRefresh();
			}

			virtual void RotateTo(float x, float y, float z)
			{
				GameObject::RotateTo(x, y, z);

				iRefresh();
			}

			void LookAt(const math::Vector v)
			{
				m_use_lookat = 1;
				m_lookat = v;

				iRefresh();
			}
			
			void Draw()
			{
				iRefresh();
			}

		private:
			void iRefresh()
			{
				if(!m_screen || !m_screen->GetDevice()) {
					return;
				}

				if(m_use_lookat) {
					math::Vector pos = GetGlobalPosition();
					D3DXVECTOR3 vpos(pos.x, pos.y, pos.z);

					D3DXVECTOR3 up(0, 1, 0);
					D3DXVECTOR3 lookat(m_lookat.x, m_lookat.y, m_lookat.z);

					D3DXMATRIX matrix;
					D3DXMatrixLookAtLH(&matrix, &vpos, &lookat, &up);
					m_device->SetTransform(D3DTS_VIEW, &matrix);
					return;
				}

				D3DXMATRIX world_matrix = iGetWorldMatrix();
				if(world_matrix == m_last_world_matrix) {
					return;
				}

				m_last_world_matrix = world_matrix;

				D3DXMATRIX matrix;

				math::Vector pos = GetGlobalPosition();
				D3DXVECTOR3 vpos(pos.x, pos.y, pos.z);
				
				D3DXVECTOR3 up(0, 1, 0);

				up = math::Util::vect3transform(up, world_matrix);

				D3DXVECTOR3 orign(0, 0, 0);
				orign = math::Util::vect3transform(orign, world_matrix);

				up -= orign;

				D3DXVECTOR3 look(0, 0, 1);
				look = math::Util::vect3transform(look, world_matrix);

				look -= orign;

				look += vpos;
				
				D3DXMatrixLookAtLH(&matrix, &vpos, &look, &up);
				m_device->SetTransform(D3DTS_VIEW, &matrix);
			}

		private:
			D3DXMATRIX	m_last_world_matrix;

			int m_use_lookat;
			math::Vector m_lookat;
	};
}

#endif