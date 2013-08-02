#ifndef _game_logo_h_
#define _game_logo_h_

#include "GameObject.h"
#include "CubeGameObject.h"
#include "Screen.h"

namespace game
{
	class GameLogo : public engine::GameObject
	{
		public:
			GameLogo(engine::Screen * scn) : engine::GameObject(scn)
			{
				int i;

				// "Ì¹"
				for(i = 0; i < 7; ++i) {
					Add(i, 1);
				}
				Add(2, 0); Add(2, 2);
				Add(6, 0); Add(6, 2);

				for(i = 0; i < 5; ++i) {
					Add(i, 5);
				}
				for(i = 0; i < 5; ++i) {
					Add(i, 7);
				}
				Add(0, 6); Add(2, 6); Add(4, 6);
				for(i = 0; i < 5; ++i) {
					Add(6, 4 + i);
				}

				// "¿Ë"
				for(i = 0; i < 3; ++i) {
					Add(i, 13);
				}
				Add(1, 12); Add(1, 14);

				for(i = 0; i < 3; ++i) {
					Add(3, 12 + i);
				}
				for(i = 0; i < 3; ++i) {
					Add(5, 12 + i);
				}
				Add(4, 12); Add(4, 14);

				Add(6, 12.5); Add(6, 13.5);

				Add(7, 12.5); Add(7, 13.5);
				Add(8, 12.5); Add(8, 13.5);

				Add(8, 11.5); Add(8, 14.5);
			}

		private:
			void Add(float row, float line)
			{
				engine::CubeGameObject * cube = new engine::CubeGameObject(m_screen);
				AddChild(cube);

				cube->MoveTo(line * 2.5, -row * 2.5, 0);
			}
	};
}
#endif