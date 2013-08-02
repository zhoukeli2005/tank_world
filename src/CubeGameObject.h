#ifndef _cube_game_object_
#define _cube_game_object_

#include "Config.h"
#include "Vertex.h"
#include "GameObject.h"

using namespace engine;

static Vertex vertexs[] = {
	// front
	{-1,-1, -1,	0, 0, -1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF), 0, 0},
	{1, -1, -1,	0, 0, -1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF), 1, 0},
	{1, 1, -1,	0, 0, -1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF), 1, 1},
	{-1, 1, -1,	0, 0, -1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF), 0, 1},

	// right
	{1, -1, -1,	1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 0},
	{1, -1, 1,	1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 0},
	{1, 1, 1,	1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 1},
	{1, 1, -1,	1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 1},

	// back
	{1, -1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 0},
	{-1, -1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 0},
	{-1, 1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 1},
	{1, 1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 1},

	// left
	{-1, -1, -1,-1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 0},
	{-1, 1, -1,	-1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 0},
	{-1, 1, 1,	-1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 1},
	{-1, -1, 1,	-1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 1},

	// top
	{-1, 1, -1,	0, 1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 0},
	{1, 1, -1,	0, 1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 0},
	{1, 1, 1,	0, 1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 1},
	{-1, 1, 1,	0, 1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 1},

	// down
	{-1, -1, -1,0, -1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 0},
	{-1, -1, 1,	0, -1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 0},
	{1, -1, 1,	0, -1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 1, 1},
	{1, -1, -1,	0, -1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00), 0, 1}
};

static DWORD indexes[] = {
	0, 3, 2, 1
};

namespace engine
{
	class CubeGameObject : public GameObject
	{
		public:
			CubeGameObject(Screen * scn) : GameObject(scn) 
			{
				m_vertex_count = 4 * 6;
				m_index_count = 4;

				if(FAILED(m_device->CreateVertexBuffer(m_vertex_count * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF_FLAGS, D3DPOOL_DEFAULT, &m_vertex, NULL))) {
					Warning("CreateVertexBuffer Failed!");
					return;
				}

				if(FAILED(m_device->CreateIndexBuffer(m_index_count * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_index, NULL))) {
					Warning("CreateIndexBuffer Failed!");
					return;
				}

				void * pvertexes;
				m_vertex->Lock(0, m_vertex_count * sizeof(Vertex), &pvertexes, 0);

				memcpy(pvertexes, vertexs, sizeof(vertexs));

				m_vertex->Unlock();

				void * pindexes;
				m_index->Lock(0, m_index_count * sizeof(DWORD), &pindexes, 0);

				memcpy(pindexes, indexes, sizeof(indexes));

				m_index->Unlock();

				m_vertex_count = 0;

				const char * dir = ".\\resource\\tank\\SU_85.jpg";
				if(FAILED(D3DXCreateTextureFromFile(m_device, dir, &m_texture))) {
					Warning("LoadingTexture Failed!");
				}
			}

			void Draw()
			{
				GameObject::Draw();

				// Draw Vetex List
				m_device->SetStreamSource(0, m_vertex, 0, sizeof(Vertex));
				m_device->SetIndices(m_index);
				m_device->SetFVF(Vertex::FVF_FLAGS);
				int i;
				for(i = 0; i < 6; ++i) {
					m_device->SetTexture(0, m_texture);
					if(FAILED(m_device->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 4 * i, 0, 4, 0, 2))) {
						Warning("DrawIndexedPrimitive Error");
					}
				}
			}
		private:
			LPDIRECT3DTEXTURE9 m_texture;
	};

}

#endif