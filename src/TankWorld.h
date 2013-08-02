#ifndef _tank_world_h_
#define _tank_world_h_

#include "config.h"
#include "Screen.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "CubeGameObject.h"
#include "MeshGameObject.h"
#include "SkyBoxGameObject.h"
#include "VK.h"
#include "Tank.h"
#include "OctTree.h"
#include "TankWorldConfig.h"

namespace game
{
	class Missile;
	class PlayerTank;
	class EnemyTank;

	class TankWorld : public engine::Screen
	{
		public:
			TankWorld(HINSTANCE hinstance, int width, int height):engine::Screen(hinstance, width, height) 
			{
				m_mouse_down = 0;
			}

		public:
			void Start();

		// events
		public:
			void OnLButtonDown(long x, long y);
			void OnMouseMove(long x, long y);
			void OnLButtonUp(long x, long y);
			void OnMouseWheel(long x, long y, long delta);
			void OnKeyDown(int vk);
			void OnKeyUp(int vk);

		// interface
		public:
			engine::Camera * GetMainCamera()
			{
				return m_camera;
			}

			engine::OctTree * GetOctTree()
			{
				return m_scene_octree;
			}

			void CreateNewTank();

		// update every frame
		protected:
			virtual void iEnterFrame();

		private:
			enum {
				E_GOD_VIEW,
				E_TANK_VIEW
			};
			enum {
				E_GAME_INIT,
					E_GAME_INIT_1,	// move camera
					E_GAME_INIT_2,	// rotate camera
					E_GAME_INIT_3,	// rotate camera
					E_GAME_INIT_4,	// move camera
					E_GAME_INIT_5,	// press any key to start
				E_GAME_NORMAL
			};
			void SetGodView();
			void SetTankView();
			void CreateMissile(int velocity);
			float GetFireEnergy();

			void StartLogoPresent();
			void DoLogoPresent(int delta);

		private:
			engine::Camera			* m_camera;

			engine::SkyBoxGameObject * m_sky;

			engine::DirectionalLight * m_sun_light;

			// game 
			game::PlayerTank * m_tank;
			engine::MeshGameObject * m_platform;
			engine::MeshGameObject * m_terrain;

			engine::OctTree * m_scene_octree;

			Missile * m_missile;
			EnemyTank * m_enemy;

			// control
			long m_mouse_posX;
			long m_mouse_posY;
			int m_mouse_down;

			// view type - god view or tank view
			int m_view_type;

			// game state -- init or normal
			int m_game_state;

			long m_last_update_time;
			int m_start_fire_time;
	};
}

#endif