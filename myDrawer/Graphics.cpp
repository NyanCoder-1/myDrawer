#include "pch.h"

#include "Shader.h"
#include "Buffer.h"

#include "Tools.h"


using namespace std;
using namespace DirectX;

extern HWND g_hWnd;

D3D_DRIVER_TYPE			g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL		g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device1*			g_pd3dDevice = NULL;
ID3D11DeviceContext1*	g_pImmediateContext = NULL;
IDXGISwapChain*			g_pSwapChain = NULL;
ID3D11RenderTargetView*	g_pRenderTargetView = NULL;
ID3D11BlendState*		g_pBlendingState = NULL;
ID3D11SamplerState*		g_pSampleState = NULL;

UINT g_width = 0;           // получаем ширину
UINT g_height = 0;   // и высоту окна


HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	g_width = rc.right - rc.left;           // получаем ширину
	g_height = rc.bottom - rc.top;   // и высоту окна
	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// Тут мы создаем список поддерживаемых версий DirectX
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
	// которая описывает свойства переднего буфера и привязывает его к нашему окну.
	DXGI_SWAP_CHAIN_DESC sd;            // Структура, описывающая цепь связи (Swap Chain)
	ZeroMemory(&sd, sizeof(sd));    // очищаем ее
	sd.BufferCount = 1;                               // у нас один задний буфер
	sd.BufferDesc.Width = g_width;                     // ширина буфера
	sd.BufferDesc.Height = g_height;                          // высота буфера
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // формат пикселя в буфере
	sd.BufferDesc.RefreshRate.Numerator = 75;         // частота обновления экрана
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера - задний буфер
	sd.OutputWindow = g_hWnd;                               // привязываем к нашему окну
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;                               // не полноэкранный режим
	
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &g_pSwapChain, (ID3D11Device**)&g_pd3dDevice, &g_featureLevel, (ID3D11DeviceContext**)&g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) return hr;
	
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)g_width;
	vp.Height = (FLOAT)g_height;
	vp.MinDepth = .0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}


Shader *g_MainShader;
ID3D11Buffer *g_GUIvb;
ID3D11Buffer *g_GUIib;
ID3D11Buffer *g_WVP;
XMMATRIX g_matOrtho;
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};
struct Const
{
	XMMATRIX WVP;
};

vector<Shape*> shapes = { new sLine() };
vector<Shape*> toolbar = { new sPencil(), new sLine(), new sPolyline(), new sRect(), new sRoundRect(), new sEllips() };
tools curtool = tPencil;

//--------------------------------------------------------------------------------------
// Удалить все созданные объекты
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	// Сначала отключим контекст устройства, потом отпустим объекты.
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	// Порядок удаления имеет значение. Обратите внимание, мы удалеям
	// эти объекты порядке, обратном тому, в котором создавали.

	_RELEASE(g_pSampleState);
	_CLOSE(g_MainShader);
	_RELEASE(g_GUIvb);
	_RELEASE(g_GUIib);
	_RELEASE(g_WVP);
	_RELEASE(g_pRenderTargetView);
	_RELEASE(g_pSwapChain);
	_RELEASE(g_pImmediateContext);
	_RELEASE(g_pd3dDevice);
	for (Shape *s : shapes)
		_DELETE(s);
}

void SetCurrentTool(byte t)
{
	curtool = (tools)t;
	_DELETE(shapes[shapes.size() - 1]);
	switch (curtool)
	{
	case tPencil:
		shapes[shapes.size() - 1] = new sPencil();
		break;
	case tLine:
		shapes[shapes.size() - 1] = new sLine();
		break;
	case tPolyline:
		shapes[shapes.size() - 1] = new sPolyline();
		break;
	case tRectangle:
		shapes[shapes.size() - 1] = new sRect();
		break;
	case tRoundRect:
		shapes[shapes.size() - 1] = new sRoundRect();
		break;
	case tEllips:
		shapes[shapes.size() - 1] = new sEllips();
		break;
	}
}

bool GUIshow = false;
int cindices = 0;
void InitGUIBuffers()
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	if (GUIshow)
	{
		vertices.push_back({ XMFLOAT3(0, g_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width + 20, g_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width + 20, g_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(0, g_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(g_width*0.5f - 20, g_height - 120, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 20, g_height - 120, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f - 20, g_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 20, g_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(g_width*0.5f - 17, g_height - 115, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f - 17, g_height - 110, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f, g_height - 105, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 17, g_height - 110, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 17, g_height - 115, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f, g_height - 110, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(g_width / 6.0f*curtool, g_height - 100, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width / 6.0f*(curtool + 1), g_height - 100, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width / 6.0f*curtool, g_height, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width / 6.0f*(curtool + 1), g_height, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		indices.push_back(6);
		indices.push_back(5);
		indices.push_back(7);

		indices.push_back(8);
		indices.push_back(13);
		indices.push_back(10);
		indices.push_back(8);
		indices.push_back(10);
		indices.push_back(9);
		indices.push_back(13);
		indices.push_back(12);
		indices.push_back(11);
		indices.push_back(13);
		indices.push_back(11);
		indices.push_back(10);

		indices.push_back(14);
		indices.push_back(15);
		indices.push_back(16);
		indices.push_back(16);
		indices.push_back(15);
		indices.push_back(17);

		_DELETE(toolbar[0]);
		toolbar[0] = new sPencil();
		_DELETE(toolbar[1]);
		toolbar[1] = new sLine();
		_DELETE(toolbar[2]);
		toolbar[2] = new sPolyline();
		_DELETE(toolbar[3]);
		toolbar[3] = new sRect();
		_DELETE(toolbar[4]);
		toolbar[4] = new sRoundRect();
		_DELETE(toolbar[5]);
		toolbar[5] = new sEllips();

		int x0 = (0 + g_width / 6.0f)*0.5f, y0 = g_height - 50;
		toolbar[0]->MouseDown(x0, y0 - 25);
		toolbar[0]->MouseMove(x0 - 25, y0);
		toolbar[0]->MouseMove(x0 - 25, y0 + 25);
		toolbar[0]->MouseMove(x0 - 20, y0 + 25);
		toolbar[0]->MouseMove(x0 - 25, y0 + 20);
		toolbar[0]->MouseMove(x0 - 25, y0 + 25);
		toolbar[0]->MouseMove(x0, y0 + 25);
		toolbar[0]->MouseMove(x0 + 25, y0);
		toolbar[0]->MouseMove(x0 + 24, y0 - 12);
		toolbar[0]->MouseMove(x0 + 12, y0 - 24);
		toolbar[0]->MouseMove(x0, y0 - 25);
		toolbar[0]->MouseMove(x0 - 25, y0);
		toolbar[0]->MouseMove(x0 - 13, y0 + 1);
		toolbar[0]->MouseMove(x0 + 12, y0 - 24);
		toolbar[0]->MouseMove(x0 - 13, y0 + 1);
		toolbar[0]->MouseMove(x0 - 1, y0 + 13);
		toolbar[0]->MouseMove(x0 + 24, y0 - 12);
		toolbar[0]->MouseMove(x0 - 1, y0 + 13);
		toolbar[0]->MouseMove(x0, y0 + 25);

		x0 = (g_width / 6.0f + g_width / 6.0f * 2)*0.5f; y0 = g_height - 50;
		toolbar[1]->MouseDown(x0 - 25, y0 + 12);
		toolbar[1]->MouseMove(x0 + 25, y0 - 12);

		x0 = (g_width / 6.0f * 2 + g_width / 6.0f * 3)*0.5f; y0 = g_height - 50;
		toolbar[2]->MouseDown(x0 - 25, y0 - 12);
		toolbar[2]->MouseDown(x0 + 12, y0 - 25);
		toolbar[2]->MouseDown(x0 - 13, y0 + 15);
		toolbar[2]->MouseDown(x0 + 25, y0 + 25);

		x0 = (g_width / 6.0f * 3 + g_width / 6.0f * 4)*0.5f; y0 = g_height - 50;
		toolbar[3]->MouseDown(x0 - 40, y0 - 25);
		toolbar[3]->MouseMove(x0 + 40, y0 + 25);

		x0 = (g_width / 6.0f * 4 + g_width / 6.0f * 5)*0.5f; y0 = g_height - 50;
		toolbar[4]->MouseDown(x0 - 40, y0 - 25);
		toolbar[4]->MouseMove(x0 + 40, y0 + 25);
		toolbar[4]->SetRound(10);

		x0 = (g_width / 6.0f * 5 + g_width / 6.0f * 6)*0.5f; y0 = g_height - 50;
		toolbar[5]->MouseDown(x0 - 40, y0 - 25);
		toolbar[5]->MouseMove(x0 + 40, y0 + 25);
	}
	else
	{
		vertices.push_back({ XMFLOAT3(g_width*0.5f - 20, g_height - 20, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 20, g_height - 20, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f - 20, g_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 20, g_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(g_width*0.5f - 17, g_height - 5, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f - 17, g_height - 10, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f, g_height - 15, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 17, g_height - 10, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f + 17, g_height - 5, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(g_width*0.5f, g_height - 10, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });


		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);

		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		indices.push_back(6);
		indices.push_back(9);
		indices.push_back(4);
		indices.push_back(6);
		indices.push_back(7);
		indices.push_back(8);
		indices.push_back(8);
		indices.push_back(9);
		indices.push_back(6);
	}
	_RELEASE(g_GUIvb);
	_RELEASE(g_GUIib);
	g_GUIvb = Buffer::CreateVertexBuffer(sizeof(Vertex)*vertices.size(), false, vertices.data());
	g_GUIib = Buffer::CreateIndexBuffer(sizeof(DWORD)*indices.size(), false, indices.data());
	cindices = indices.size();
}

void Resize(int height, int width)
{
	if (g_pSwapChain)
	{
		g_pImmediateContext->OMSetRenderTargets(0, 0, 0);

		// Освобождаем задний буффер
		g_pRenderTargetView->Release();

		HRESULT hr;
		// Меняем размер буффера
		// Ширина и высота выбираются автоматически из HWND
		hr = g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// Создаём задний буффер
		ID3D11Texture2D* pBuffer;
		hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(void**)&pBuffer);

		hr = g_pd3dDevice->CreateRenderTargetView(pBuffer, NULL,
			&g_pRenderTargetView);
		pBuffer->Release();

		g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

		g_width = width;           // получаем ширину
		g_height = height;   // и высоту окна

		// Настройка вьюпорта
		D3D11_VIEWPORT vp;
		vp.Width = g_width;
		vp.Height = g_height;
		vp.MinDepth = .0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_pImmediateContext->RSSetViewports(1, &vp);

		InitGUIBuffers();
	}
}

bool InitGeometry()
{
	g_MainShader = new Shader();
	g_MainShader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	g_MainShader->AddInputElementDesc("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (!g_MainShader->CreateShader(L"data\\shaders\\vsMainShader.hlsl", L"data\\shaders\\psMainShader.hlsl"))
		return false;

	g_WVP = Buffer::CreateConstantBuffer(sizeof(Const), false);

	InitGUIBuffers();

	SetCurrentTool(curtool);

	return true;
}


vector<Vertex> lines;
//--------------------------------------------------------------------------------------
// Рисование примитивов
//--------------------------------------------------------------------------------------

void DrawLine(int x1, int y1, int x2, int y2)
{
	lines.push_back({ XMFLOAT3(x1, y1, .0f), XMFLOAT4(.0f, .0f, .0f, 1.0f) });
	lines.push_back({ XMFLOAT3(x2, y2, .0f), XMFLOAT4(.0f, .0f, .0f, 1.0f) });
}

//--------------------------------------------------------------------------------------
// Рисование кадра
//--------------------------------------------------------------------------------------
void Render()
{
	// Просто очищаем задний буфер
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::White);

	g_MainShader->Draw();

	Const cb;
	ZeroMemory(&cb, sizeof(Const));
	cb.WVP = XMMatrixTranspose(XMMatrixOrthographicOffCenterRH(.0f, g_width, g_height, .0f, .0f, 1.0f));
	g_pImmediateContext->UpdateSubresource(g_WVP, 0, NULL, &cb, 0, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_WVP);

	// ToDo
	for (Shape *s : shapes)
	{
		lines.clear();

		s->Render();

		ID3D11Buffer *g_vbLine = Buffer::CreateVertexBuffer(sizeof(Vertex) * lines.size(), false, lines.data());
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_vbLine, &stride, &offset);
		g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		g_pImmediateContext->Draw(lines.size(), 0);
		_RELEASE(g_vbLine);
	}

	ZeroMemory(&cb, sizeof(Const));
	cb.WVP = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(0.0f, g_width, g_height, .0f, .0f, 1.0f));
	g_pImmediateContext->UpdateSubresource(g_WVP, 0, NULL, &cb, 0, 0);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_GUIvb, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_GUIib, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_WVP);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(cindices, 0, 0);

	if (GUIshow)
		for (Shape *s : toolbar)
		{
			lines.clear();

			s->Render();

			ID3D11Buffer *g_vbLine = Buffer::CreateVertexBuffer(sizeof(Vertex) * lines.size(), false, lines.data());
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;
			g_pImmediateContext->IASetVertexBuffers(0, 1, &g_vbLine, &stride, &offset);
			g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			g_pImmediateContext->Draw(lines.size(), 0);
			_RELEASE(g_vbLine);
		}

	// Выбросить задний буфер на экран
	g_pSwapChain->Present(0, 0);
}

#define in(n, n1, n2) (n>=min(n1,n2)&&n<=max(n1,n2))
void MouseDown(int x, int y)
{
	if (!GUIshow)
	{
		if (in(x, g_width*0.5f - 20, g_width*0.5f + 20) && in(y, g_height - 20, g_height))
		{
			GUIshow = true;
			InitGUIBuffers();
			return;
		}
		shapes[shapes.size() - 1]->MouseDown(x, y);
	}
	else
	{
		if (in(x, g_width*0.5f - 20, g_width*0.5f + 20) && in(y, g_height - 120, g_height - 100))
		{
			GUIshow = false;
			InitGUIBuffers();
			return;
		}
		if (y<g_height - 100)
			shapes[shapes.size() - 1]->MouseDown(x, y);
		else
		{
			shapes.push_back(nullptr);
			SetCurrentTool(x / int(g_width/6.0f));
			InitGUIBuffers();
		}
	}
}
void MouseMove(int x, int y)
{
	shapes[shapes.size() - 1]->MouseMove(x, y);
}
void MouseUp(int x, int y)
{
	shapes[shapes.size() - 1]->MouseUp();
}
void EndDraw()
{
	shapes.push_back(nullptr);
	SetCurrentTool(curtool);
}