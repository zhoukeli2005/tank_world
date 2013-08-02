#include "TankWorld.h"
#include "CubeGameObject.h"
#include "Screen.h"
#include "GameObject.h"
#include "Camera.h"
#include "Config.h"
#include "PlayerTank.h"
#include "Missile.h"
#include "EnemyTank.h"
#include "Util.h"

using namespace game;

void TankWorld::Start()
{
	int now = GetTickCount();
	srand(time(NULL) * now);
	m_last_update_time = now;
	m_start_fire_time = 0;

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
	m_platform->MoveTo(0, 50, 0);

	// terrain
	m_terrain = new MeshGameObject(this);
	m_platform->Copy(m_terrain);
	m_terrain->ScaleTo(TerrainSize, TerrainSize, TerrainSize);

	GetRoot()->AddChild(m_terrain);
	m_terrain->MoveTo(0, -200, 0);

	// tank
	m_tank = new PlayerTank(this);
	m_tank->LoadFromFile(".\\resource\\tank\\tank1.x");

	m_tank->SetRange(m_platform->GetXWidth() / 2 - m_tank->GetZWidth());
	m_tank->MoveTo(0, m_platform->GetBoundBox().GetMax().y, 0);
	m_tank->SetOriginFaceDirection(0, 0, -1);

	m_platform->AddChild(m_tank);

	// init missile
	m_missile = new Missile(this, E_TT_MISSILE_PLAYER);
	m_missile->LoadFromFile(".\\resource\\missile\\m1.x");

	// enemy
	m_enemy = new EnemyTank(this);
	m_enemy->LoadFromFile(".\\resource\\tank\\tank1.x");

	int i;
	for(i = 0; i < MaxEnemyCount; ++i) {
		CreateNewTank();
	}


	SetTankView();
}

void TankWorld::iEnterFrame()
{
	int now = GetTickCount();

	int delta = now - m_last_update_time;
	m_last_update_time = now;

	int range = 500;
	m_platform->Rotate(0, delta * 0.001 * 1, 0);

	math::Vector rotate = m_platform->GetLocalRotate();

	float posX = range * cos(D3DXToRadian(rotate.y));
	float posZ = range * sin(D3DXToRadian(rotate.y));
	m_platform->MoveTo(posX, m_platform->GetLocalPosition().y, posZ);

	// 蓄力
	if(m_start_fire_time) {
		float v = GetFireEnergy();

		SetFireEnergy(v);
	}
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
		if(m_tank->StartFire()) {
			CreateMissile(MissileVelocity);
		}
	}

	if(vk == VK::K) {
		// 开始蓄力
		if(!m_start_fire_time) {
			m_start_fire_time = GetTickCount();
		}
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
	if(vk == VK::K) {
		float v = GetFireEnergy();
		m_start_fire_time = 0;

		if(m_tank->StartFire()) {
			CreateMissile(v * MissileVelocity);
		}
		SetFireEnergy(0);
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
	m_camera->MoveTo(0, 600, -600);
	m_camera->RotateTo(35, 0, 0);
}

void TankWorld::SetTankView()
{
	m_view_type = E_TANK_VIEW;
	m_tank->AddChild(m_camera);
	m_camera->MoveTo(0, 80, 120);
	m_camera->RotateTo(15, 180, 0);
}

void TankWorld::CreateNewTank()
{
	EnemyTank * enemy = m_enemy->CopyTank();
	enemy->SetOriginFaceDirection(0, 0, -1);

	// 有 10%的概率出生在平台上，90%概率出生在地面上
	int r = (rand() % 100);
	if(r < 10) {
		// 出生在平台上

		m_platform->AddChild(enemy);
		enemy->SetWhere(E_WHERE_PLATFORM);

		int range = m_platform->GetXWidth() / 2 - enemy->GetZWidth();
		float posX = range * 0.8 * (rand() % 100) * 0.01;
		float posZ = range * 0.8 * (rand() % 100) * 0.01;

		m_platform->AddChild(enemy);
		enemy->SetRange(range);
		enemy->MoveTo(posX, m_platform->GetBoundBox().GetMax().y, posZ);
		
		return;
	}

	enemy->SetWhere(E_WHERE_TERRAIN);

	int dis = TerrainMinRange + (TerrainMaxRange - TerrainMinRange) * (rand() % 100) * 0.01;
	int a = rand() % 360;
	float posX = math::Util::quick_cos(a) * dis;
	float posZ = math::Util::quick_sin(a) * dis;

	GetRoot()->AddChild(enemy);
	enemy->SetRange(TerrainMaxRange);

	math::Vector pos = math::Vector(0, m_terrain->GetBoundBox().GetMax().y, 0);
	pos = m_terrain->ToGlobalPosition(pos);

	enemy->MoveTo(posX, pos.y, posZ);
}

void TankWorld::CreateMissile(int velocity)
{
	Missile * m = m_missile->CopyMissile();
	GetRoot()->AddChild(m);
	math::Vector pos = m_tank->ToGlobalPosition(math::Vector(-3, 10, -55));
	D3DXVECTOR3 face3 = m_tank->GetFaceDirectionGlobal();
	math::Vector face(face3.x, face3.y, face3.z);
	m->Start(pos, face, velocity);
}

float TankWorld::GetFireEnergy()
{
	int now = GetTickCount();
	int delta = now - m_start_fire_time;
	delta = min(3000, max(300, delta));

	float v = delta / 1000.0;

	return v;
}