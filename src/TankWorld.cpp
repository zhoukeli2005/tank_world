#include "TankWorld.h"
#include "CubeGameObject.h"
#include "Screen.h"
#include "GameObject.h"
#include "Camera.h"
#include "Config.h"
#include "PlayerTank.h"
#include "Missile.h"

using namespace game;

void TankWorld::Start()
{
	// oct tree
	m_scene_octree = new engine::OctTree();

	// camera
	m_camera = new engine::Camera(this);

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
	m_platform->MoveTo(0, 500, 0);

	// terrain
	m_terrain = new MeshGameObject(this);
	m_platform->Copy(m_terrain);
	m_terrain->ScaleTo(10, 10, 10);

	GetRoot()->AddChild(m_terrain);
	m_terrain->MoveTo(0, -200, 0);

	// tank
	m_tank = new PlayerTank(this, E_TT_TANK_PLAYER);
	m_tank->LoadFromFile(".\\resource\\tank\\tank1.x");
	m_tank->MoveTo(0, 50, 0);
	m_tank->SetOriginFaceDirection(0, 0, -1);

	m_platform->AddChild(m_tank);
	
//	Missile * missile = new Missile(this, E_TT_MISSILE_PLAYER);
//	missile->LoadFromFile(".\\resource\\missile\\m1.x");
//	GetRoot()->AddChild(missile);
//	missile->ScaleTo(0.03, 0.03, 0.03);
//	missile->Start(math::Vector(50, 600, 0), math::Vector(0, 0, 1), 10);

	// init missile
	m_missile = new Missile(this, E_TT_MISSILE_PLAYER);
	m_missile->LoadFromFile(".\\resource\\missile\\m1.x");

	SetGodView();
}

void TankWorld::iEnterFrame()
{
	int range = 500;
//	m_platform->Rotate(0, 0.03, 0);

	math::Vector rotate = m_platform->GetLocalRotate();
//	m_platform->MoveTo(range * sin(D3DXToRadian(rotate.y)), m_platform->GetLocalPosition().y, range * cos(D3DXToRadian(rotate.y)));
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
	//	m_tank->Rotate(0, -2, 0);
		m_tank->StartRotate(-40);
	}
	if(vk == VK::D) {
	//	m_tank->Rotate(0, 2, 0);
		m_tank->StartRotate(40);
	}
	if(vk == VK::W) {
	//	m_tank->MoveForward(2);
		m_tank->StartMove(40);
	}
	if(vk == VK::S) {
	//	m_tank->MoveBackward(2);
		m_tank->StartMove(-40);
	}

	math::Vector v;
	if(vk == VK::SPACE) {
		if(m_view_type == E_GOD_VIEW) {
			SetTankView();
		} else {
			SetGodView();
		}
	}

	if(vk == VK::J) {
		m_tank->StartFire();

		Missile * m = m_missile->CopyMissile();
		GetRoot()->AddChild(m);
		math::Vector pos = m_tank->ToGlobalPosition(math::Vector(-3, 10, -52));
		D3DXVECTOR3 face3 = m_tank->GetFaceDirectionGlobal();
		math::Vector face(face3.x, face3.y, face3.z);
		m->Start(pos, face, 20);
	}

	if(vk == VK::G) {
		m_tank->StartUnderHit(math::Vector(1, 0, 0));
	}
}

void TankWorld::OnKeyUp(int vk)
{
	if(vk == VK::A || vk == VK::D) {
		m_tank->StopRotate();
	}
	if(vk == VK::W || vk == VK::S) {
		m_tank->StopMove();
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

void TankWorld::SetGodView()
{
	m_view_type = E_GOD_VIEW;
	GetRoot()->AddChild(m_camera);
	m_camera->MoveTo(0, 100, -100);
	m_camera->RotateTo(45, 0, 0);
}

void TankWorld::SetTankView()
{
	m_view_type = E_TANK_VIEW;
	m_tank->AddChild(m_camera);
	m_camera->MoveTo(0, 200, 200);
	m_camera->RotateTo(45, 180, 0);
}