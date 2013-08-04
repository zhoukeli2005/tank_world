#include "Config.h"
#include "TankWorld.h"
#include "Screen.h"

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	math::Util::Init();

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	game::TankWorld * tw = new game::TankWorld(hInstance, width * 0.8, height * 0.8);

	tw->Start();

	tw->Loop();

	return 0;
}