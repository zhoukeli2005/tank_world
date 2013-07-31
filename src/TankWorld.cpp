#include "TankWorld.h"
#include "CubeGameObject.h"
#include "Screen.h"
#include "GameObject.h"
#include "Camera.h"
#include "Config.h"

using namespace game;

void TankWorld::Start()
{
	// camera
	m_camera = new engine::Camera(this);
	GetRoot()->AddChild(m_camera);
	m_camera->MoveTo(0, 1500, 0);
	m_camera->RotateTo(90, 0, 0);

	// sky
	m_sky = new engine::SkyBoxGameObject(this);
	m_sky->SetSkyTexture(
		".\\resource\\sky\\sky_ft.jpg", 
		".\\resource\\sky\\sky_rt.jpg", 
		".\\resource\\sky\\sky_bk.jpg", 
		".\\resource\\sky\\sky_lf.jpg",
		".\\resource\\sky\\sky_up.jpg", 
		".\\resource\\sky\\sky_dn.jpg");

	GetRoot()->AddChild(m_sky);

	// sun light
	m_sun_light = new engine::DirectionalLight(this);
	m_sun_light->SetDirection(0, -10, 0);

	// platform
	m_platform = new engine::MeshGameObject(this);
	m_platform->LoadFromFile(".\\resource\\platform\\platform.x");
	GetRoot()->AddChild(m_platform);
	m_platform->MoveTo(0, 0, 0);

	// terrain
	m_terrain = m_platform->Copy();
	m_terrain->ScaleTo(3, 3, 3);

	GetRoot()->AddChild(m_terrain);
	m_terrain->MoveTo(0, -100, 0);
	// tank
	m_tank = new engine::MeshGameObject(this);
	m_tank->LoadFromFile(".\\resource\\tank\\tank1.x");
	m_tank->MoveTo(0, 30, 0);
	m_tank->SetOriginFaceDirection(0, 0, -1);

	m_platform->AddChild(m_tank);

//	m_tank->AddChild(m_camera);
//	m_camera->MoveTo(0, 1050, 80);
//	m_camera->RotateTo(0, 180, 0);

}

void TankWorld::iEnterFrame()
{
	int range = 100;
	m_platform->Rotate(0, 0.01, 0);

	math::Vector rotate = m_platform->GetLocalRotate();
	m_platform->MoveTo(range * sin(D3DXToRadian(rotate.y)), m_platform->GetLocalPosition().y, range * cos(D3DXToRadian(rotate.y)));
}

void TankWorld::OnLButtonDown(long x, long y)
{
	if(!m_mouse_down) {
		m_mouse_down = 1;
		m_mouse_posX = x;
		m_mouse_posY = y;
	}
}

void TankWorld::OnLButtonUp(long x, long y)
{
	m_mouse_down = 0;
}

void TankWorld::OnMouseMove(long x, long y)
{
	if(!m_mouse_down) {
		return;
	}

	m_camera->Rotate((y - m_mouse_posY) * 0.5, (x - m_mouse_posX) * 0.5, 0);

	m_mouse_posX = x;
	m_mouse_posY = y;
}

void TankWorld::OnKeyDown(int vk)
{
	if(vk == VK::A) {
		m_tank->Rotate(0, -2, 0);
	}
	if(vk == VK::D) {
		m_tank->Rotate(0, 2, 0);
	}
	if(vk == VK::W) {
		m_tank->MoveForward(2);
	}
	if(vk == VK::S) {
		m_tank->MoveBackward(2);
	}

	math::Vector v;
	if(vk == VK::SPACE) {
		v = m_camera->GetGlobalPosition();
	}
}

void TankWorld::OnMouseWheel(long x, long y, long delta)
{
	if(delta > 0) {
		m_camera->MoveForward(10);
	} else {
		m_camera->MoveBackward(10);
	}
}