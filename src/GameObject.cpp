#include "GameObject.h"
#include "Screen.h"
#include "Vertex.h"

using namespace engine;


GameObject::GameObject(Screen * scn)
:m_screen(scn)
,m_ref_count(1)
,m_parent(NULL)
,m_first_child(NULL)
,m_sibling(NULL)
,m_position()
,m_rotate()
,m_scale(1, 1, 1)
,m_vertex(NULL)
,m_index(NULL)
,m_vertex_count(0)
,m_origin_face(0, 0, 1)
,m_is_removing(0)
,m_last_draw(0)
{
	m_device = m_screen->GetDevice();
}

GameObject::~GameObject()
{
	if(m_vertex) {
		m_vertex->Release();
		m_vertex = NULL;
	}
	if(m_index) {
		m_index->Release();
		m_index = NULL;
	}

	m_vertex_count = 0;
}

void GameObject::Draw()
{
	if(m_is_removing) {
		return;
	}

	GameObject * child = m_first_child;

	while(child) {
		GameObject * next = child->m_sibling;
		if(child->m_is_removing) {
			RemoveChild(child);
		}
		child = next;
	}
	
	child = m_first_child;
	while(child) {
		GameObject * next = child->m_sibling;
		if(!child->m_is_removing) {
			child->Draw();
		}
		child = next;
	}


	D3DXMATRIX matrix = iGetWorldMatrix();
	LPDIRECT3DDEVICE9 device = m_screen->GetDevice();
	device->SetTransform(D3DTS_WORLD, &matrix);

	if(m_vertex_count <= 0) {
		return;
	}

	// Draw Vetex List
	{
		device->SetStreamSource(0, m_vertex, 0, sizeof(Vertex));
		device->SetIndices(m_index);
		device->SetFVF(Vertex::FVF_FLAGS);
		if(FAILED(device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertex_count, 0, m_index_count / 3))) {
			Warning("DrawIndexedPrimitive Error");
		}

	}
}

void GameObject::AddChild(GameObject * child)
{
	if(child->m_parent == this) {
		return;
	}

	child->Grab();

	if(child->m_parent) {
		child->m_parent->RemoveChild(child);
	}

	child->m_parent = this;
	child->m_sibling = NULL;

	child->m_sibling = m_first_child;
	m_first_child = child;
}

void GameObject::RemoveChild(GameObject * child)
{
	if(child->m_parent != this) {
		Warning("child->parent != this");
		return;
	}

	GameObject * sibling = child->m_sibling;
	
	child->m_sibling = NULL;
	child->m_parent = NULL;

	if(m_first_child == child) {
		m_first_child = sibling;
		child->Drop();
		return;
	}

	GameObject * prev = m_first_child;
	while(prev && prev->m_sibling != child) {
		prev = prev->m_sibling;
	}

	if(!prev) {
		Warning("No such child!");
		return;
	}

	prev->m_sibling = sibling;
	child->Drop();
}

void GameObject::RemoveFromScreen()
{
	m_is_removing = 1;
}

math::Vector GameObject::GetLocalPosition()
{
	return m_position;
}

math::Vector GameObject::GetGlobalPosition()
{
	math::Vector v(0, 0, 0);
	return ToGlobalPosition(v);
}

math::Vector GameObject::ToGlobalPosition(const math::Vector & v)
{
	D3DXMATRIX matrix = iGetWorldMatrix();
	D3DXVECTOR3 v3(v.x, v.y, v.z);
	D3DXVECTOR3 out3 = math::Util::vect3transform(v3, matrix);

	return math::Vector(out3.x, out3.y, out3.z);
}

math::Vector GameObject::ToLocalPosition(const math::Vector & v)
{
	D3DXMATRIX matrix = iGetWorldMatrix();
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &matrix);

	if(!m) {
		return math::Vector(0, 0, 1);
	}
	D3DXVECTOR3 v3(v.x, v.y, v.z);
	D3DXVECTOR3 out3 = math::Util::vect3transform(v3, m);

	return math::Vector(out3.x, out3.y, out3.z);
}

math::Vector GameObject::GetLocalRotate()
{
	return m_rotate;
}

math::Vector GameObject::GetLocalScale()
{
	return m_scale;
}

math::Vector GameObject::GetGlobalScale()
{
	math::Vector g;
	if(m_parent) {
		g = m_parent->GetGlobalScale();
	}
	return m_scale + g;
}

void GameObject::Move(float dx, float dy, float dz)
{
	MoveTo(m_position.x + dx, m_position.y + dy, m_position.z + dz);
}

void GameObject::MoveTo(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void GameObject::MoveForward(float distance)
{
	D3DXVECTOR3 face = GetFaceDirectionLocal();

	Move(face.x * distance, face.y * distance, face.z * distance);
}

void GameObject::MoveBackward(float distance)
{

	D3DXVECTOR3 face = GetFaceDirectionLocal();

	Move(-face.x * distance, -face.y * distance, -face.z * distance);
}

void GameObject::MoveToward(float distance, const math::Vector & direction)
{
	math::Vector tmp = direction;
	tmp.Normalize();

	Move(distance * tmp.x, distance * tmp.y, distance * tmp.z);
}

void GameObject::SetOriginFaceDirection(float x, float y, float z)
{
	m_origin_face.x = x;
	m_origin_face.y = y;
	m_origin_face.z = z;
}

D3DXVECTOR3 GameObject::GetFaceDirectionGlobal()
{
	D3DXMATRIX m = iGetWorldMatrix();

	D3DXVECTOR3 v(m_origin_face.x, m_origin_face.y, m_origin_face.z);
	v = math::Util::vect3transform(v, m);

	D3DXVECTOR3 o(0, 0, 0);
	o = math::Util::vect3transform(o, m);

	D3DXVECTOR3 out = v - o;
	D3DXVec3Normalize(&out, &out);

	return out;
}

D3DXVECTOR3 GameObject::GetFaceDirectionLocal()
{
	D3DXMATRIX m = iGetLocalMatrix();

	D3DXVECTOR3 v(m_origin_face.x, m_origin_face.y, m_origin_face.z);
	v = math::Util::vect3transform(v, m);

	D3DXVECTOR3 o(0, 0, 0);
	o = math::Util::vect3transform(o, m);

	D3DXVECTOR3 out = v - o;
	D3DXVec3Normalize(&out, &out);

	return out;
}

void GameObject::Rotate(float dx, float dy, float dz)
{
	RotateTo(m_rotate.x + dx, m_rotate.y + dy, m_rotate.z + dz);
}

void GameObject::RotateTo(float x, float y, float z)
{
	while(x < 0) {
		x += 360;
	}
	while(x > 360) {
		x -= 360;
	}
	while(y < 0) {
		y += 360;
	}
	while(y > 360) {
		y -= 360;
	}
	while(z < 0) {
		z += 360;
	}
	while(z > 360) {
		z -= 360;
	}
	m_rotate.x = x;
	m_rotate.y = y;
	m_rotate.z = z;
}

void GameObject::Scale(float dx, float dy, float dz)
{
	m_scale.x += dx;
	m_scale.y += dy;
	m_scale.z += dz;
}

void GameObject::ScaleTo(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

D3DXMATRIX GameObject::iGetWorldMatrix()
{
	D3DXMATRIX pmatrix, out;

	D3DXMatrixIdentity(&pmatrix);
	D3DXMatrixIdentity(&out);

	if(m_parent) {
		pmatrix = m_parent->iGetWorldMatrix();
	}

	D3DXMATRIX lm = iGetLocalMatrix();

	out = lm * pmatrix;

	return out;
}

D3DXMATRIX GameObject::iGetLocalMatrix()
{
	D3DXMATRIX mscale, mrotate, mtrans, out;
	
	D3DXMatrixScaling(&mscale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&mrotate, D3DXToRadian(m_rotate.y), D3DXToRadian(m_rotate.x), D3DXToRadian(m_rotate.z));
	D3DXMatrixTranslation(&mtrans, m_position.x, m_position.y, m_position.z);

	out = mscale * mrotate * mtrans;

	return out;
}
