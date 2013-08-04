#ifndef _tank_world_config_h_
#define _tank_world_config_h_

namespace game
{
	enum TANKWORLD_TYPES{
		E_TT_TANK_PLAYER,
		E_TT_TANK_ENEMY,
		E_TT_MISSILE_PLAYER,
		E_TT_MISSILE_ENEMY
	};

	enum TANK_WHEAE {
		E_WHERE_PLATFORM,
		E_WHERE_TERRAIN
	};

	const int MissileExistDuration = 1000;
	const int MissileVelocity = 400;

	const int MaxEnemyCount = 10;

	const int TerrainSize = 10;
	const int TerrainMinRange = 200;
	const int TerrainMaxRange = 400;

	const int LogoHeight = 2000;
}

#endif