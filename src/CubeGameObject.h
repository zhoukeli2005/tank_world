#ifndef _cube_game_object_
#define _cube_game_object_

#include "Config.h"
#include "Vertex.h"
#include "GameObject.h"

using namespace engine;

static Vertex vertexs[] = {
	{-1,-1, -1,	-1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0x00, 0x00)},
	{1, -1, -1,	1, 0, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00)},
	{1, 1, -1,	0, 1, 0,		D3DCOLOR_XRGB(0xFF, 0x00, 0xFF)},
	{-1, 1, -1,	0, 1, 0,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00)},
	{-1, -1, 1,	0, 0, -1,		D3DCOLOR_XRGB(0xFF, 0x00, 0xFF)},
	{1, -1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0x00, 0xFF, 0xFF)},
	{1, 1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0xFF)},
	{-1, 1, 1,	0, 0, 1,		D3DCOLOR_XRGB(0xFF, 0xFF, 0x00)}
};

static DWORD indexes[] = {
	0, 1, 2,
	0, 2, 3, 

	1, 5, 6,
	1, 6, 2,

	5, 4, 6,
	4, 7, 6,

	0, 3, 7,
	0, 7, 4,

	3, 2, 6,
	3, 6, 7,

	0, 4, 5,
	0, 5, 1
};

namespace engine
{
	class CubeGameObject : public GameObject
	{
		public:
			CubeGameObject(Screen * scn) : GameObject(scn) 
			{
				m_vertex_count = 8;
				m_index_count = 12 * 3;

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
			}
	};

}

#endif