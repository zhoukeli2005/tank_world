#ifndef _mesh_game_object_h_
#define _mesh_game_object_h_

#include "GameObject.h"

namespace engine
{
	class MeshGameObject : public GameObject
	{
		public:
			MeshGameObject(Screen * scn) : GameObject(scn), m_mesh(NULL), m_materials(NULL), m_textures(NULL), m_local_aabb(NULL) { }

		public:
			virtual ~MeshGameObject();

		public:
			void LoadFromFile(LPCSTR filename);

			virtual void Draw();

			// 拷贝一个使用同一份资源的MeshGameObject，以节约内存和加载文件时间
			void Copy(MeshGameObject * to);

			math::AABB GetBoundBox()
			{
				if(!m_local_aabb) {
					IDirect3DVertexBuffer9 * vertex;
					m_mesh->GetVertexBuffer(&vertex);
					D3DXVECTOR3 * vecs;
					vertex->Lock(0, 0, (void**)&vecs, D3DLOCK_READONLY);

					D3DXVECTOR3 _min, _max;
					D3DXComputeBoundingBox(vecs, m_mesh->GetNumVertices(), D3DXGetFVFVertexSize(m_mesh->GetFVF()), &_min, &_max);

					m_local_aabb = new math::AABB();
					m_local_aabb->SetFromD3DVector(_min, _max);
				}


				return *m_local_aabb;
			}

		protected:
			LPD3DXMESH		m_mesh;

		private:
			D3DMATERIAL9 *			m_materials;
			LPDIRECT3DTEXTURE9 *	m_textures;

			DWORD	m_count;

			math::AABB * m_local_aabb;
	};
}

#endif