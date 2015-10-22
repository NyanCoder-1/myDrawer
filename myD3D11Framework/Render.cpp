#include "pch.h"

#include "Shader.h"

#include "Render.h"
#include "Buffer.h"
#include "macros.h"

namespace myD3D11Framework
{
//------------------------------------------------------------------

	Render::Render()
	{
		m_driverType = D3D_DRIVER_TYPE_NULL;
		m_featureLevel = D3D_FEATURE_LEVEL_11_0;
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;
	}

	Render::~Render()
	{
	}

	bool Render::CreateDevice(HWND hwnd)
	{
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(hwnd, &rc);
		m_width = rc.right - rc.left;
		m_height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = m_width;
		sd.BufferDesc.Height = m_height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return false;

		ID3D11Texture2D* pBackBuffer = NULL;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return false;

		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		_RELEASE(pBackBuffer);
		if (FAILED(hr))
			return false;

		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)m_width;
		vp.Height = (FLOAT)m_height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pImmediateContext->RSSetViewports(1, &vp);

		return Init(hwnd);
	}
	void Render::Resize(int height, int width)
	{
		if (m_pSwapChain)
		{
			m_pImmediateContext->OMSetRenderTargets(0, 0, 0);

			// ����������� ������ ������
			m_pRenderTargetView->Release();

			HRESULT hr;
			// ������ ������ �������
			// ������ � ������ ���������� ������������� �� HWND
			hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			// ������ ������ ������
			ID3D11Texture2D* pBuffer;
			hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);

			hr = m_pd3dDevice->CreateRenderTargetView(pBuffer, NULL,
				&m_pRenderTargetView);
			pBuffer->Release();

			m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

			m_width = width;           // �������� ������
			m_height = height;   // � ������ ����
			
								 // ��������� ��������
			D3D11_VIEWPORT vp;
			vp.Width = m_width;
			vp.Height = m_height;
			vp.MinDepth = .0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			m_pImmediateContext->RSSetViewports(1, &vp);
		}
	}
	void Render::BeginFrame()
	{
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::White);
	}

	void Render::EndFrame()
	{
		m_pSwapChain->Present(0, 0);
	}

	void Render::Shutdown()
	{
		Close();

		if (m_pImmediateContext)
			m_pImmediateContext->ClearState();

		_RELEASE(m_pRenderTargetView);
		_RELEASE(m_pSwapChain);
		_RELEASE(m_pImmediateContext);
		_RELEASE(m_pd3dDevice);
	}

//------------------------------------------------------------------
}