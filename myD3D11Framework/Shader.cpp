#include "pch.h"
#include "Shader.h"
#include "macros.h"

namespace myD3D11Framework
{
//------------------------------------------------------------------
#define MAXLAYOUT 8

	Shader::Shader(ID3D11Device *device, ID3D11DeviceContext *context)
	{
		m_device = device;
		m_context = context;
		m_vertexShader = nullptr;
		m_pixelShader = nullptr;
		m_layout = nullptr;
		m_layoutformat = nullptr;
		m_numlayout = 0;
	}

	void Shader::AddInputElementDesc(const char *SemanticName, DXGI_FORMAT format)
	{
		if (!m_numlayout)
		{
			m_layoutformat = new D3D11_INPUT_ELEMENT_DESC[MAXLAYOUT];
			if (!m_layoutformat)
				return;
		}
		else if (m_numlayout >= MAXLAYOUT)
			return;

		D3D11_INPUT_ELEMENT_DESC &Layout = m_layoutformat[m_numlayout];

		Layout.SemanticName = SemanticName;
		Layout.SemanticIndex = 0;
		Layout.Format = format;
		Layout.InputSlot = 0;
		if (!m_numlayout)
			Layout.AlignedByteOffset = 0;
		else
			Layout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		Layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Layout.InstanceDataStepRate = 0;

		m_numlayout++;
	}

	bool Shader::CreateShader(wchar_t *namevs, wchar_t *nameps)
	{
		HRESULT hr = S_OK;
		ID3DBlob *vertexShaderBuffer = nullptr;
		hr = m_compileshaderfromfile(namevs, "VS", "vs_5_0", &vertexShaderBuffer);
		if (FAILED(hr))
			return false;

		ID3DBlob *pixelShaderBuffer = nullptr;
		hr = m_compileshaderfromfile(nameps, "PS", "ps_5_0", &pixelShaderBuffer);
		if (FAILED(hr))
			return false;

		hr = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
		if (FAILED(hr))
			return false;

		hr = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
		if (FAILED(hr))
			return false;

		hr = m_device->CreateInputLayout(m_layoutformat, m_numlayout, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
		if (FAILED(hr))
			return false;
		_DELETE_ARRAY(m_layoutformat);

		_RELEASE(vertexShaderBuffer);
		_RELEASE(pixelShaderBuffer);

		return true;
	}

	HRESULT Shader::m_compileshaderfromfile(WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		ShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob *pErrorBlob = nullptr;
		hr = D3DCompileFromFile(FileName, NULL, NULL, EntryPoint, ShaderModel, ShaderFlags, 0, ppBlobOut, &pErrorBlob);

		_RELEASE(pErrorBlob);
		return hr;
	}


	void Shader::Draw()
	{
		m_context->IASetInputLayout(m_layout);
		m_context->VSSetShader(m_vertexShader, NULL, 0);
		m_context->PSSetShader(m_pixelShader, NULL, 0);
		if (!m_textures.empty())
			m_context->PSSetShaderResources(0, m_textures.size(), &m_textures[0]);
	}

	void Shader::Close()
	{
		_RELEASE(m_vertexShader);
		_RELEASE(m_pixelShader);
		_RELEASE(m_layout);
		for (int i = 0; i < m_textures.size(); i++)
			_RELEASE(m_textures[i]);
		m_textures.clear();
	}

//------------------------------------------------------------------
}