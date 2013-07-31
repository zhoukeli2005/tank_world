#ifndef _config_h_
#define _config_h_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <math.h>

#include <iostream>
#include <string>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define Warning(str)	std::cout << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")" << str << std::endl

#endif