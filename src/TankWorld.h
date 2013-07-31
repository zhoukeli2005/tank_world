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

namespace game
{
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

		// must override
		public:
			engine::Camera * GetMainCamera()
			{
				return m_camera;
			}

		protected:
			virtual void iEnterFrame();


		private:
			engine::Camera			* m_camera;

			engine::SkyBoxGameObject * m_sky;

			engine::DirectionalLight * m_sun_light;

			// game 
			engine::MeshGameObject * m_tank;
			engine::MeshGameObject * m_platform;
			engine::MeshGameObject * m_terrain;

			// control
			long m_mouse_posX;
			long m_mouse_posY;
			int m_mouse_down;
	};
}

#endif