#pragma once

#include <clocale>
#include <ctime>

#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifndef DLL
#define DLL __declspec(dllexport)
#endif

using namespace DirectX;
using namespace std;