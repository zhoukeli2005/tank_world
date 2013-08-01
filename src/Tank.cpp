#include "Tank.h"
#include "TankWorld.h"

using namespace game;

void Tank::MoveTo(float x, float y, float z)
{
	math::Vector origin = GetLocalPosition();

	MeshGameObject::MoveTo(x, y, z);

	math::Vector direction = math::Vector(x - origin.x, y - origin.y, z - origin.z);

	engine::OctTreeData * other = m_tank_world->GetOctTree()->CheckConflict(this, direction);
	if(!other) {
		return;
	}

	int flags = other->GetFlags();

	if(flags == E_TT_MISSILE_ENEMY || flags == E_TT_MISSILE_PLAYER) {
		tank_OnHit();
		return;
	}

	ResetPosition();

	tank_OnCollideTank((Tank*)other);

	return;
}

void Tank::OctTreeConflict(OctTreeData * other, math::Vector direction)
{
	Warning("Conflict !");

	int flags = other->GetFlags();
	if(flags == E_TT_MISSILE_ENEMY || flags == E_TT_MISSILE_PLAYER) {
		tank_OnHit();
		return;
	}

	ResetPosition();

	tank_OnCollideTank((Tank *)other);
}

void Tank::Draw()
{
	MeshGameObject::Draw();

	if(!m_last_tick) {
		m_last_tick = GetTickCount();
		return;
	}

	int now = GetTickCount();
	int delta = now - m_last_tick;

	if(m_state == E_STATE_MOVE_ROTATE) {
		if(m_move_velocity) {
			MoveForward(delta * 0.001 * m_move_velocity);
		}
		if(m_rotate_velocity) {
			Rotate(0, delta * 0.001 * m_rotate_velocity, 0);
		}
	}
	if(m_state == E_STATE_UNDER_HIT) {
		float start_v = 50;
		float start_a = -50;
		int all_time = 1000;

		float tmp = (now - m_under_hit_time) * 0.001;

		GameObject::MoveTo(m_under_hit_pos.x, m_under_hit_pos.y, m_under_hit_pos.z);
		float s = start_v * tmp + 0.5 * start_a * tmp * tmp;
		MoveToward(s, m_under_hit_dir);
		if(now - m_under_hit_time > 2000) {
			m_state = E_STATE_STOP;
		}
	}

	tank_Update();

	m_last_tick = now;
}

void Tank::ResetPosition()
{
	engine::OctTreeData * other = m_tank_world->GetOctTree()->CheckConflict(this, math::Vector(0, 0, 1));

	math::Vector origin = GetLocalPosition();

	int loop = 100;
	while(other && loop-- > 0) {
		float tmpX = origin.x + (rand() % 10 - 5);
		float tmpY = origin.y;
		float tmpZ = origin.z + (rand() % 10 - 5);

		MeshGameObject::MoveTo(tmpX, tmpY, tmpZ);
		other = m_tank_world->GetOctTree()->CheckConflict(this, math::Vector(tmpX - origin.x, tmpY - origin.y, tmpZ - origin.z));
	}
}