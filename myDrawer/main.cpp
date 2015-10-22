#include "pch.h"


#include "FW1FontWrapper.h"


HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT InitDevice();
void CleanupDevice();
bool InitGeometry();
void Render();
void Resize(int height, int width);

void MouseDown(int x, int y);
void MouseMove(int x, int y);
void MouseUp(int x, int y);
void EndDraw();

#include "MainRender.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Framework framework;

	MainRender *render = new MainRender();

	framework.SetRender(render);
	framework.Init();
	framework.AddInputListener(new Input(render));

	framework.Run();

	framework.Close();

	return 0;

	/*
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return -1;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return -1;
	}

	if (!InitGeometry())
	{
		CleanupDevice();
		return -1;
	}

	// Цикл обработки сообщений
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CleanupDevice();

	return (int)msg.wParam;
	*/
}

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0; // LoadIcon( hInstance, ( LPCTSTR )IDI_ICON );
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"myDrawer";
	wcex.hIconSm = wcex.hIcon;
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 860, 640 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"myDrawer", L"myDrawer v1.0", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}


#define MCHECK(p)(p>=0x8000?-(0xffff-p):p)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	/*switch (message)
	{
	case WM_PAINT:
		Render();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_RBUTTONDOWN:
		EndDraw();
		break;

	case WM_LBUTTONDOWN:
		MouseDown(MCHECK(LOWORD(lParam)), MCHECK(HIWORD(lParam)));
		break;
	case WM_MOUSEMOVE:
		MouseMove(MCHECK(LOWORD(lParam)), MCHECK(HIWORD(lParam)));
		InvalidateRect(g_hWnd, nullptr, NULL);
		break;
	case WM_LBUTTONUP:
		MouseUp(MCHECK(LOWORD(lParam)), MCHECK(HIWORD(lParam)));
		break;

	case WM_SIZE:
		Resize(MCHECK(HIWORD(lParam)), MCHECK(LOWORD(lParam)));
		break;
	
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 640;
			lpMMI->ptMinTrackSize.y = 480;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}*/

	return 0;
}