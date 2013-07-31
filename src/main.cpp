#include "Config.h"
#include "TankWorld.h"
#include "Screen.h"

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	game::TankWorld * tw = new game::TankWorld(hInstance, 1280, 800);

	tw->Start();

	tw->Loop();

	return 0;
}