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

		// must override
		public:
			engine::Camera * GetMainCamera()
			{
				return m_camera;
			}

			engine::OctTree * GetOctTree()
			{
				return m_scene_octree;
			}

		protected:
			virtual void iEnterFrame();

		private:
			enum {
				E_GOD_VIEW,
				E_TANK_VIEW
			};
			void SetGodView();
			void SetTankView();


		private:
			engine::Camera			* m_camera;

			engine::SkyBoxGameObject * m_sky;

			engine::DirectionalLight * m_sun_light;

			// game 
			game::Tank * m_tank;
			engine::MeshGameObject * m_platform;
			engine::MeshGameObject * m_terrain;

			engine::OctTree * m_scene_octree;

			Missile * m_missile;

			// control
			long m_mouse_posX;
			long m_mouse_posY;
			int m_mouse_down;

			// view type
			int m_view_type;
	};
}

#endif