#include "pch.h"
#include "Controls.h"
#include "MainRender.h"
#include <limits>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};
struct Const
{
	XMMATRIX WVP;
};


void ImprovedShape::Draw()
{
	if (m_pShape->IsChanged())
	{
		vector<Vertex> vlines;
		vector<Vertex> vtriangles;

		m_pShape->Render();
		vector<Line> Lines = m_pShape->GetLines();
		for (int i = 0; i < Lines.size(); i++)
		{
			vlines.push_back({ XMFLOAT3(Lines[i].points[0].x, Lines[i].points[0].y, .0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
			vlines.push_back({ XMFLOAT3(Lines[i].points[1].x, Lines[i].points[1].y, .0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		}
		vector<Triangle> Triangles = m_pShape->GetTriangles();
		for (int i = 0; i < Triangles.size(); i++)
		{
			vtriangles.push_back({ XMFLOAT3(Triangles[i].points[0].x, Triangles[i].points[0].y, .0f), XMFLOAT4(0.0f, 0.0f, 0.5f, 0.5f) });
			vtriangles.push_back({ XMFLOAT3(Triangles[i].points[1].x, Triangles[i].points[1].y, .0f), XMFLOAT4(0.0f, 0.0f, 0.5f, 0.5f) });
			vtriangles.push_back({ XMFLOAT3(Triangles[i].points[2].x, Triangles[i].points[2].y, .0f), XMFLOAT4(0.0f, 0.0f, 0.5f, 0.5f) });
		}

		_RELEASE(m_pVBLines);
		m_pVBLines = Buffer::CreateVertexBuffer(m_pRender->m_pd3dDevice, sizeof(Vertex) * vlines.size(), false, vlines.data());
		_RELEASE(m_pVBTriangles);
		m_pVBTriangles = Buffer::CreateVertexBuffer(m_pRender->m_pd3dDevice, sizeof(Vertex) * vtriangles.size(), false, vtriangles.data());

		lsize = vlines.size();
		tsize = vtriangles.size();
	}

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	m_pRender->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVBTriangles, &stride, &offset);
	m_pRender->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pRender->m_pImmediateContext->Draw(tsize, 0);
	m_pRender->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVBLines, &stride, &offset);
	m_pRender->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_pRender->m_pImmediateContext->Draw(lsize, 0);
}
void ImprovedShape::Close()
{
	_DELETE(m_pShape);
	_RELEASE(m_pVBLines);
	_RELEASE(m_pVBTriangles);
}

void ImprovedShape::SetShape(Shape *shape)
{
	_DELETE(m_pShape);
	m_pShape = shape;
};


MainRender::MainRender()
{
	m_pShader = nullptr;
	m_WVP = nullptr;

	m_pShapes.push_back(new ImprovedShape(this, new sPencil()));
	
	m_GUIshow = false;
	m_isMoving = false;
}

bool MainRender::Init()
{
	m_pShader = new Shader(this);
	m_pShader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_pShader->AddInputElementDesc("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (!m_pShader->CreateShader(L"data\\shaders\\vsMainShader.hlsl", L"data\\shaders\\psMainShader.hlsl"))
		return false;

	m_WVP = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(Const), false);

	m_pnz = XMFLOAT3(0.0f, 0.0f, float(min(m_width, m_height)));

	InitializeControls();

	m_MinXYMaxXY = XMFLOAT4(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
	TurnOnAlphaBlending();

	UpdatePos();

	return true;
}
void MainRender::Resize()
{
	UpdatePos();
}

bool MainRender::Draw()
{
	m_pShader->Draw();

	Const cb;
	ZeroMemory(&cb, sizeof(Const));
	cb.WVP = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(
		m_pnz.x - (int)m_width / 2 / m_pnz.z, m_pnz.x + m_width / 2 / m_pnz.z,
		m_pnz.y + m_height / 2 / m_pnz.z, m_pnz.y - (int)m_height / 2 / m_pnz.z, .0f, 1.0f));
	m_pImmediateContext->UpdateSubresource(m_WVP, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_WVP);

	for (Shape *s : m_pShapes)
		s->Render();

	for(Control *c: m_controls)
		if (c->IsVisible()) 
			c->Draw();

	wstringstream str;
	str << L"Pos: " << m_pnz.x << L"\/" << m_pnz.y << L" Zoom: " << m_pnz.z;
	m_ptxt->SetText(str.str());
	m_ptxt->Draw(0.0f, 0.0f, 0.0f, 10, 10);

	return true;
}

void MainRender::Close()
{
	for (Control *c : m_controls)
		_CLOSE(c);
	m_controls.clear();
	_CLOSE(m_pShader);
	_RELEASE(m_WVP);
	for (Shape *s : m_pShapes)
		_DELETE(s);
}