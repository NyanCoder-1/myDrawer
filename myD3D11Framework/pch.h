#pragma once

#include <clocale>
#include <ctime>

#include <string>
#include <list>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"d3dcompiler.lib")

#ifndef DLL
#define DLL __declspec(dllexport)
#endif

using namespace std;
using namespace DirectX;