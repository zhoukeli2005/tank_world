#ifndef _object_h_
#define _object_h_

#include "Config.h"
#include "Vector.h"
#include "AABB.h"
#include "Util.h"

namespace engine {

	class Screen;

	class GameObject
	{
		// constructor
		public:
			GameObject(Screen * scn);

		// Destructor
		public:
			virtual ~GameObject();

		// Reference Count
		public:
			void Grab() { m_ref_count++; }
			void Drop() 
			{
				m_ref_count--;

				if(m_ref_count < 0) {
					Warning("Drop Error! Check Memory Release!");
				}

				if(m_ref_count == 0) {
					delete this;
				}
			}

		// interface
		public:

			// Draw
			virtual void Draw();

			// Children
			virtual void AddChild(GameObject * child);
			virtual void RemoveFromScreen();

			// Transform
			virtual math::Vector GetLocalPosition();
			virtual math::Vector GetGlobalPosition();
			virtual math::Vector ToGlobalPosition(const math::Vector & v);
			virtual math::Vector ToGlobalVector(const math::Vector & v)
			{
				math::Vector o = ToGlobalPosition(math::Vector(0, 0, 0));
				math::Vector l = ToGlobalPosition(v);

				return l - o;
			}

			virtual math::Vector ToLocalPosition(const math::Vector & v);
			virtual math::Vector ToLocalVector(const math::Vector & v)
			{
				math::Vector o = ToLocalPosition(math::Vector(0, 0, 0));
				math::Vector l = ToLocalPosition(v);

				return l - o;
			}

			virtual math::Vector GetLocalRotate();

			virtual math::Vector GetLocalScale();
			virtual math::Vector GetGlobalScale();

			virtual void Move(float dx, float dy, float dz);
			virtual void MoveTo(float x, float y, float z);

			virtual void MoveForward(float distance);
			virtual void MoveBackward(float distance);
			virtual void MoveToward(float distance, const math::Vector & direction);

			virtual void SetOriginFaceDirection(float x, float y, float z);
			virtual D3DXVECTOR3 GetFaceDirectionGlobal();
			virtual D3DXVECTOR3 GetFaceDirectionLocal();

			virtual void Rotate(float dx, float dy, float dz);
			virtual void RotateTo(float x, float y, float z);

			virtual void Scale(float dx, float dy, float dz);
			virtual void ScaleTo(float x, float y, float z);
			

		// internal
		protected:
			virtual D3DXMATRIX iGetWorldMatrix();
			virtual D3DXMATRIX iGetLocalMatrix();

		private:
			void RemoveChild(GameObject * child);

		// members -- cannot access from children
		private:
			GameObject *	m_first_child;
			GameObject *	m_sibling;

			math::Vector	m_origin_face;

			int m_last_draw;

			// reference count
			int m_ref_count;

		// members -- can access from children
		protected:
			int m_is_removing;

			Screen *		m_screen;
			LPDIRECT3DDEVICE9	m_device;

			GameObject *	m_parent;

			math::Vector	m_position;
			math::Vector	m_rotate;
			math::Vector	m_scale;

			IDirect3DVertexBuffer9 *	m_vertex;
			IDirect3DIndexBuffer9 *		m_index;
			int							m_vertex_count;
			int							m_index_count;

	};
}

#endif