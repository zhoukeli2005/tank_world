#ifndef _sky_box_game_object_h_
#define _sky_box_game_object_h_

#include "GameObject.h"
#include "Vertex.h"

namespace engine
{
	static Vertex vertexes[] = {
		// front
		{-1, -1, -1,	0, 0, 1,	D3DCOLOR_XRGB(255, 255, 255), 1, 1},
		{1, -1, -1,	0, 0, 1,	D3DCOLOR_XRGB(255, 255, 255), 0, 1},
		{1, 1, -1,	0, 0, 1,	D3DCOLOR_XRGB(255, 255, 255), 0, 0},
		{-1, 1, -1,	0, 0, 1,	D3DCOLOR_XRGB(255, 255, 255), 1, 0},

		// right
		{1, -1, -1,	-1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 1},
		{1, -1, 1,	-1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 1},
		{1, 1, 1,	-1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 0},
		{1, 1, -1,	-1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 0},

		// back
		{1, -1, 1,	0, 0, -1,	D3DCOLOR_XRGB(255, 255, 255), 1, 1},
		{-1, -1, 1,	0, 0, -1,	D3DCOLOR_XRGB(255, 255, 255), 0, 1},
		{-1, 1, 1,	0, 0, -1,	D3DCOLOR_XRGB(255, 255, 255), 0, 0},
		{1, 1, 1,	0, 0, -1,	D3DCOLOR_XRGB(255, 255, 255), 1, 0},

		// left
		{-1, -1, 1,	1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 1},
		{-1, -1, -1,	1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 1},
		{-1, 1, -1,	1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 0},
		{-1, 1, 1,	1, 0, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 0},

		// top
		{1, 1, -1,	0, -1, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 1},
		{1, 1, 1,	0, -1, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 1},
		{-1, 1, 1,	0, -1, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 0},
		{-1, 1, -1, 0, -1, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 0},

		// bottom
		{1, -1, 1,	0, 1, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 0},
		{1, -1, -1,	0, 1, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 0},
		{-1, -1, -1,	0, 1, 0,	D3DCOLOR_XRGB(255, 255, 255), 1, 1},
		{-1, -1, 1,	0, 1, 0,	D3DCOLOR_XRGB(255, 255, 255), 0, 1}
	};

	static int indexes[] = {
		0, 1, 2, 3
	};

	class SkyBoxGameObject : public GameObject
	{
		public:
			SkyBoxGameObject(Screen * scn) : GameObject(scn) { }

		public:
			~SkyBoxGameObject() { }

		public:
			void SetSkyTexture(const char * front, const char * right, const char * back, const char * left,
								const char * up, const char * down )
			{
				int count = 6 * 4;

				if(FAILED(m_device->CreateVertexBuffer(count * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF_FLAGS, D3DPOOL_DEFAULT, &m_vertex, NULL))) {
					Warning("CreateVertexBuffer Failed!");
					return;
				}

				if(FAILED(m_device->CreateIndexBuffer(4 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index, NULL))) {
					Warning("CreateIndexBuffer Failed!");
					return;
				}

				void * pvertexes;
				m_vertex->Lock(0, m_vertex_count * sizeof(Vertex), &pvertexes, 0);

				memcpy(pvertexes, vertexes, sizeof(vertexes));

				m_vertex->Unlock();

				void * pindexes;
				m_index->Lock(0, m_index_count * sizeof(DWORD), &pindexes, 0);

				memcpy(pindexes, indexes, sizeof(indexes));

				m_index->Unlock();

				// Load Material
				const char * files[] = {front, left, back, right, up, down};
				int i;
				for(i = 0; i < 6; ++i) {
					if(FAILED(D3DXCreateTextureFromFile(m_device, files[i], &m_materials[i]))) {
						Warning("Failed to Create Texture From File");
					}
				}

				float range = (m_screen->GetNear() + m_screen->GetFar()) * 0.5;
				ScaleTo(range, range, range);
			}

		public:
			void Draw()
			{
				GameObject * c = m_screen->GetMainCamera();
				if(c) {
					math::Vector pos = c->GetGlobalPosition();
					MoveTo(pos.x, pos.y, pos.z);
				}

				GameObject::Draw();

				m_device->SetStreamSource(0, m_vertex, 0, sizeof(Vertex));
				m_device->SetIndices(m_index);
				m_device->SetFVF(Vertex::FVF_FLAGS);

				int i;
				for(i = 0; i < 6; ++i) {
					m_device->SetTexture(0, m_materials[i]);
					m_device->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, i * 4, 0, 4, 0, 2);
				}
			}

		private:
			LPDIRECT3DTEXTURE9	m_materials[6];
	};
}

#endif