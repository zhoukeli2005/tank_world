#ifndef _config_h_
#define _config_h_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <math.h>

#include <iostream>
#include <string>

#include <time.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define Warning(str)
//#define Warning(str)	std::cout << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << ")" << str << std::endl
//#define  Warning(str)	throw str

#endif