#include "MeshGameObject.h"
#include "Config.h"

using namespace engine;

MeshGameObject::~MeshGameObject()
{
	if(m_mesh) {
		m_mesh->Release();
		m_mesh = NULL;
	}

	int i;
	if(m_materials) {
		delete[] m_materials;
		m_materials = NULL;
	}

	if(m_textures) {
		for(i = 0; i < m_count; ++i) {
			m_textures[i]->Release();
		}
		delete[] m_textures;
		m_textures = NULL;
	}
}

void MeshGameObject::LoadFromFile(LPCSTR filename)
{
	LPD3DXBUFFER buffer;
	HRESULT hr;
	hr = D3DXLoadMeshFromX(filename, 0, m_device, NULL, &buffer, NULL, &m_count, &m_mesh);
	if( FAILED( hr ) ) {
		if(hr == E_OUTOFMEMORY) {
			Warning("OutofMemory");
		}
		if(hr == D3DERR_OUTOFVIDEOMEMORY) {
			Warning("OutOfVideoMemory");
		}
		if(hr == D3DERR_INVALIDCALL) {
			Warning("invalid call");
		}
		Warning("LoadMeshFromFileX Error!");
		return;
	}

	// find dir
	const char * p = strrchr(filename, '\\');
	char dir[1024];
	int dir_len = p - filename + 1;
	memcpy(dir, filename, dir_len);
	
	D3DXMATERIAL * M = (D3DXMATERIAL *)buffer->GetBufferPointer();

	m_materials = new D3DMATERIAL9[m_count];
	m_textures = new LPDIRECT3DTEXTURE9[m_count];

	int i;
	for(i = 0; i < m_count; ++i) {
		m_materials[i] = M[i].MatD3D;
		m_materials[i].Ambient = m_materials[i].Diffuse;

		m_textures[i] = NULL;

		if(!M[i].pTextureFilename) {
			continue;;
		}
		int len = strlen(M[i].pTextureFilename);
		memcpy(dir + dir_len, M[i].pTextureFilename, len);

		dir[dir_len + len] = 0;

		if(FAILED(hr = D3DXCreateTextureFromFile(m_device, dir, &m_textures[i]))) {
			Warning("LoadingTexture Failed!");
		}
	}

	buffer->Release();
}

void MeshGameObject::Draw()
{
	GameObject::Draw();

	int i;
	for(i = 0; i < m_count; ++i) {
		m_device->SetMaterial(&m_materials[i]);
		if(m_textures[i]) {
			m_device->SetTexture(0, m_textures[i]);
		}
		m_mesh->DrawSubset(i);
	}
}

MeshGameObject * MeshGameObject::Copy()
{
	MeshGameObject * m = new MeshGameObject(m_screen);

	m->m_count = m_count;
	
	m_mesh->AddRef();
	m->m_mesh = m_mesh;

	m->m_materials = new D3DMATERIAL9[m_count];
	int i;
	for(i = 0; i < m_count; ++i) {
		m->m_materials[i] = m_materials[i];
	}

	m->m_textures = new LPDIRECT3DTEXTURE9[m_count];
	for(i = 0; i < m_count; ++i) {
		m->m_textures[i] = m_textures[i];
		if(m_textures[i]) {
			m_textures[i]->AddRef();
		}
	}

	return m;
}