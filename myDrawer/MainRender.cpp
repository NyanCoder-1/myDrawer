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

#define MY_PI (3.141592654f)
#define MY_PIDIV2 (1.570796327f)
float dee = 0;

/*void ShapeDrawer::Draw(ID3D11Device *device, ID3D11DeviceContext *context)
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
		m_pVBLines = Buffer::CreateVertexBuffer(device, sizeof(Vertex) * vlines.size(), false, vlines.data());
		_RELEASE(m_pVBTriangles);
		m_pVBTriangles = Buffer::CreateVertexBuffer(device, sizeof(Vertex) * vtriangles.size(), false, vtriangles.data());

		lsize = vlines.size();
		tsize = vtriangles.size();
	}

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	context->IASetVertexBuffers(0, 1, &m_pVBTriangles, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(tsize, 0);
	context->IASetVertexBuffers(0, 1, &m_pVBLines, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->Draw(lsize, 0);
}*/


MainRender::MainRender()
{
	m_pShader = nullptr;
	m_WVP = nullptr;
	
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

	m_pnz = { 0.0f, 0.0f, float(min(m_width, m_height)) };

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

void MainRender::DrawShapes(void)
{
	for (int i = 0; i < m_pShapes.size(); i++)
	{
		auto s = m_pShapes[i];
		if (!s->CanIDraw())
			continue;
		if (s->IsChanged())
		{
			while (i >= m_vertexBuffer.size())
				m_vertexBuffer.push_back(MyVertexBuffer());
			s->Render();
			vector<Triangle> triangles = s->GetTriangles();
			vector<Line> lines = s->GetLines();
			vector<Vertex> points;
			for (int i = 0; i < triangles.size(); i++)
			{
				auto color = XMFLOAT4(triangles[i].color.r, triangles[i].color.g, triangles[i].color.b, triangles[i].color.a);
				points.push_back({ XMFLOAT3(triangles[i].points[0].x, triangles[i].points[0].y, 0.0f), color });
				points.push_back({ XMFLOAT3(triangles[i].points[1].x, triangles[i].points[1].y, 0.0f), color });
				points.push_back({ XMFLOAT3(triangles[i].points[2].x, triangles[i].points[2].y, 0.0f), color });
			}
			_RELEASE(m_vertexBuffer[i].pVB);
			m_vertexBuffer[i].pVB = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex)*points.size(), false, points.data());
			m_vertexBuffer[i].vertices = points.size();
		}
		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer[i].pVB, &stride, &offset);
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pImmediateContext->Draw(m_vertexBuffer[i].vertices, 0);
	}
}
bool MainRender::Draw()
{
	m_pShader->Draw();

	Const cb;
	ZeroMemory(&cb, sizeof(Const));
	cb.WVP = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(
		m_pnz.x - (int)m_width / 2 / m_pnz.zoom, m_pnz.x + m_width / 2 / m_pnz.zoom,
		m_pnz.y + m_height / 2 / m_pnz.zoom, m_pnz.y - (int)m_height / 2 / m_pnz.zoom, .0f, 1.0f));
	m_pImmediateContext->UpdateSubresource(m_WVP, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_WVP);

	DrawShapes();

	for(Control *c: m_controls)
		if (c->IsVisible()) 
			c->Draw();

	wstringstream str;
	str << L"Pos: " << m_pnz.x << L"\/" << m_pnz.y << L" Zoom: " << m_pnz.zoom << L" Degree: " << dee;
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
	for (Shape *s: m_pShapes)
		_DELETE(s);
}