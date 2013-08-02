#include "Tank.h"
#include "TankWorld.h"

using namespace game;

void Tank::MoveTo(float x, float y, float z)
{
	if(x * x + y * y + z * z > m_range * m_range) {
		return;
	}

	math::Vector origin = GetLocalPosition();

	MeshGameObject::MoveTo(x, y, z);

	math::Vector direction = math::Vector(x - origin.x, y - origin.y, z - origin.z);

	engine::OctTreeData * other = m_tank_world->GetOctTree()->CheckConflict(this, direction);
	if(!other) {
		return;
	}

	int flags = other->GetFlags();
	int myflags = GetFlags();

	if((myflags == E_TT_TANK_PLAYER && flags == E_TT_MISSILE_ENEMY) ||
		(myflags == E_TT_TANK_ENEMY && flags == E_TT_MISSILE_PLAYER)) {
			m_on_hit_times++;
			tank_OnHit(direction);
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
	int myflags = GetFlags();

	if((myflags == E_TT_TANK_PLAYER && flags == E_TT_MISSILE_ENEMY) ||
		(myflags == E_TT_TANK_ENEMY && flags == E_TT_MISSILE_PLAYER)) {
			m_on_hit_times++;
			tank_OnHit(direction);
			return;
	}

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

	if(HaveState(E_STATE_MOVE_ROTATE) && !HaveState(E_STATE_FIRE)) {
		if(m_move_velocity) {
			MoveForward(delta * 0.001 * m_move_velocity);
		}
		if(m_rotate_velocity) {
			Rotate(0, delta * 0.001 * m_rotate_velocity, 0);
		}
	}
	if(HaveState(E_STATE_UNDER_HIT)) {
		float start_v = 1000;
		float start_a = -8000;
		
		float all_time = abs(start_v / start_a);

		float tmp = (now - m_under_hit_time) * 0.001;
		if(tmp >= all_time) {
			tmp = all_time;
		}
		
		GameObject::MoveTo(m_under_hit_pos.x, m_under_hit_pos.y, m_under_hit_pos.z);
		float s = start_v * tmp + 0.5 * start_a * tmp * tmp;
		MoveToward(s, m_under_hit_dir);
		if(now - m_under_hit_time > 2000) {
			RemoveState(E_STATE_UNDER_HIT);
		}
	}
	if(HaveState(E_STATE_FIRE)) {
		if(now - m_fire_time > 100) {
			RemoveState(E_STATE_FIRE);
			MoveForward(20);
		}
	}

	tank_Update();

	m_last_tick = now;
}

void Tank::ResetPosition()
{
	engine::OctTreeData * other = m_tank_world->GetOctTree()->CheckConflict(this, math::Vector(0, 0, 1));

	math::Vector origin = GetLocalPosition();

	int loop = 5;
	while(other && loop-- > 0) {
		float tmpX = origin.x + (rand() % 10 - 5);
		float tmpY = origin.y;
		float tmpZ = origin.z + (rand() % 10 - 5);

		MeshGameObject::MoveTo(tmpX, tmpY, tmpZ);
		other = m_tank_world->GetOctTree()->CheckConflict(this, math::Vector(tmpX - origin.x, tmpY - origin.y, tmpZ - origin.z));
	}
}