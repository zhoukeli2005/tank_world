#ifndef _screen_h_
#define _screen_h_

#include "config.h"

namespace engine {

	class GameObject;
	class Camera;

	class Screen
	{
		// constructor
		public:
			Screen(HINSTANCE hinstance, int width, int hight);

		// destructor
		public:
			~Screen();

		// interface
		public:
			GameObject * GetRoot() { return m_root; }

			LPDIRECT3DDEVICE9 GetDevice() { return m_pdevice9; }

			int GetWidth() { return m_width; }
			int GetHeight() { return m_height; }
			float GetNear() { return m_near; }
			float GetFar() { return m_far; }

			void Loop();

		// interface ( GUI )
		public:
			void SetFPS(float fps)
			{
				m_fps = fps;
			}

			void SetFireEnergy(float energy)
			{
				m_fire_energy = energy;
			}

			void SetRemark(const char * p)
			{
				memcpy(m_remark, p, strlen(p) + 1);
				m_remark_time = GetTickCount();
			}

		// must be overwrite
		public:
			virtual Camera * GetMainCamera() = 0;

		// callback -- events
		public:
			virtual void OnLButtonDown(long x, long y) {}
			virtual void OnLButtonUp(long x, long y) {}
			virtual void OnRButtonDown(long x, long y) {}
			virtual void OnRButtonUp(long x, long y) {}
			virtual void OnMouseMove(long x, long y) {}
			virtual void OnMouseWheel(long x, long y, long delta) {}
			virtual void OnKeyDown(int vk) {}
			virtual void OnKeyUp(int vk) {}

		// internal
		private:
			void iCreateWindow();
			void iInitD3D9();
			void iDestroy();

			static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

			void iUpdate();

			void calculateFPS(int now);

		// for override
		protected:
			virtual void iEnterFrame() { }

		// members
		private:
			HINSTANCE			m_hinstance;
			HWND				m_hwnd;
			LPDIRECT3D9			m_pd3d9;
			LPDIRECT3DDEVICE9	m_pdevice9;
			LPD3DXFONT			m_font;

			int m_width;
			int m_height;

			float m_near;
			float m_far;

			GameObject *		m_root;

			int m_last_draw;

			int m_fps_time;
			int m_fps_count;

		// gui
		private:
			float m_fps;
			float m_fire_energy;
			char m_remark[256];
			int m_remark_time;
	};

} // namespace engine

#endif