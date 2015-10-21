#pragma once

#include <Windows.h>

#include <string>
#include <vector>
#include <fstream>

#include <d3d11_2.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <wincodec.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment( lib, "dxguid.lib")

#define _DELETE(p)		{ if(p){delete (p); (p)=nullptr;} }
#define _DELETE_ARRAY(p){ if(p){delete[] (p); (p)=nullptr;} }
#define _RELEASE(p)		{ if(p){(p)->Release(); (p)=nullptr;} }
#define _CLOSE(p)		{ if(p){(p)->Close(); delete (p); (p)=nullptr;} }

#include "../myD3D11Framework/myD3D11Framework.h"
#ifdef _DEBUG
#pragma comment(lib, "../Debug/myD3D11Framework.lib")
#else
#pragma comment(lib, "../Release/myD3D11Framework.lib")
#endif

using namespace myD3D11Framework;