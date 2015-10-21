#pragma once

#include <clocale>
#include <ctime>

#include <string>
#include <list>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#pragma comment (lib,"d3d11.lib")

#ifndef DLL
#define DLL __declspec(dllexport)
#endif