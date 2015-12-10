#include "stdafx.h"
#include "Rectangle.h"
#include "Shader.h"
#include "macros.h"
#include "Log.h"
#include "Buffer.h"



using namespace D3D11Framework;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct ConstantBuffer
{
	XMMATRIX Ortho;
};

Rectangle::Rectangle(Render *render)
{
	m_render = render;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_constantBuffer = nullptr;
	m_shader = nullptr;
	m_color = XMFLOAT4();
}

bool Rectangle::Init(float Width, float Height)
{
	m_Width = Width;
	m_Height = Height;

	if (!m_InitBuffers())
		return false;

	m_shader = new Shader(m_render);
	if (!m_shader)
		return false;

	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (!m_shader->CreateShader(L"data\\shaders\\vsMainShader.hlsl", L"data\\shaders\\psMainShader.hlsl"))
		return false;

	return true;
}
bool Rectangle::SetSize(float Width, float Height)
{
	m_Width = Width;
	m_Height = Height;

	if (!m_InitBuffers())
		return false;
	return true;
}

bool Rectangle::m_InitBuffers()
{
	Vertex vertices[4];

	float left = 0.0f;
	float right = m_Width;
	float top = 0.0f;
	float bottom = m_Height;

	vertices[0].pos = XMFLOAT3(left, top, 0.0f);
	vertices[0].color = m_color;

	vertices[1].pos = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].color = m_color;

	vertices[2].pos = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].color = m_color;

	vertices[3].pos = XMFLOAT3(right, top, 0.0f);
	vertices[3].color = m_color;

	unsigned long indices[6] =
	{
		0,1,2,
		0,3,1
	};

	_RELEASE(m_vertexBuffer);
	_RELEASE(m_indexBuffer);
	_RELEASE(m_constantBuffer);

	m_vertexBuffer = Buffer::CreateVertexBuffer(m_render->m_pd3dDevice, sizeof(Vertex) * 4, false, &vertices);
	if (!m_vertexBuffer)
		return false;

	m_indexBuffer = Buffer::CreateIndexBuffer(m_render->m_pd3dDevice, sizeof(unsigned long) * 6, false, &indices);
	if (!m_indexBuffer)
		return false;

	m_constantBuffer = Buffer::CreateConstantBuffer(m_render->m_pd3dDevice, sizeof(ConstantBuffer), false);
	if (!m_constantBuffer)
		return false;

	return true;
}

void Rectangle::Draw(float positionX, float positionY)
{
	m_SetShaderParameters(positionX, positionY);
	m_shader->Draw();
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	m_render->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_render->m_pImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_render->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_render->m_pImmediateContext->DrawIndexed(6, 0, 0);
}

void Rectangle::m_SetShaderParameters(float x, float y)
{
	XMMATRIX objmatrix = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX wvp = objmatrix*m_render->m_Ortho;
	ConstantBuffer cb;
	cb.Ortho = XMMatrixTranspose(wvp);
	m_render->m_pImmediateContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);

	m_render->m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}

void Rectangle::Close()
{
	_RELEASE(m_vertexBuffer);
	_RELEASE(m_indexBuffer);
	_RELEASE(m_constantBuffer);
	_CLOSE(m_shader);
}