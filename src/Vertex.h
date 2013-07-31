#ifndef _vertex_h_
#define _vertex_h_

#include "Config.h"

namespace engine
{
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		DWORD clolor;
		float tu, tv;

		enum {
			FVF_FLAGS = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1
		};
	};
}
#endif