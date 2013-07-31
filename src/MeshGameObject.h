#ifndef _mesh_game_object_h_
#define _mesh_game_object_h_

#include "GameObject.h"

namespace engine
{
	class MeshGameObject : public GameObject
	{
		public:
			MeshGameObject(Screen * scn) : GameObject(scn), m_mesh(NULL), m_materials(NULL), m_textures(NULL) { }

		public:
			virtual ~MeshGameObject();

		public:
			void LoadFromFile(LPCSTR filename);

			virtual void Draw();

			// 拷贝一个使用同一份资源的MeshGameObject，以节约内存和加载文件时间
			MeshGameObject * Copy();

		private:
			LPD3DXMESH		m_mesh;

			D3DMATERIAL9 *			m_materials;
			LPDIRECT3DTEXTURE9 *	m_textures;

			DWORD	m_count;
	};
}

#endif