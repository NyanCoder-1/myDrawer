#include "pch.h"
#include "MainRender.h"

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

		m_pShape->Render();
		vector<Line> Lines = m_pShape->GetLines();
		for (int i = 0; i < Lines.size(); i++)
		{
			vlines.push_back({ XMFLOAT3(Lines[i].points[0].x, Lines[i].points[0].y, .0f), XMFLOAT4(0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f) });
			vlines.push_back({ XMFLOAT3(Lines[i].points[1].x, Lines[i].points[1].y, .0f), XMFLOAT4(0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f) });
		}

		_RELEASE(m_pVB);
		m_pVB = Buffer::CreateVertexBuffer(m_pRender->m_pd3dDevice, sizeof(Vertex) * vlines.size(), false, vlines.data());

		size = vlines.size();
	}

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	m_pRender->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	m_pRender->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_pRender->m_pImmediateContext->Draw(size, 0);
}
void ImprovedShape::Close()
{
	_DELETE(m_pShape);
	_RELEASE(m_pVB);
}

void ImprovedShape::SetShape(Shape *shape)
{
	_DELETE(m_pShape);
	m_pShape = shape;
};


MainRender::MainRender()
{
	m_pShader = nullptr;
	m_GUIvb = nullptr;
	m_GUIib = nullptr;
	m_WVP = nullptr;

	m_pShapes.push_back(new ImprovedShape(this, new sPencil()));
	
	m_GUIshow = false;
}

bool MainRender::Init(HWND hwnd)
{
	m_pShader = new Shader(m_pd3dDevice, m_pImmediateContext);
	m_pShader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_pShader->AddInputElementDesc("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	if (!m_pShader->CreateShader(L"data\\shaders\\vsMainShader.hlsl", L"data\\shaders\\psMainShader.hlsl"))
		return false;

	m_WVP = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(Const), false);

	InitBuffers();

	return true;
}
void MainRender::Resize(int height, int width)
{
	Render::Resize(height, width);
	InitBuffers();
}
void MainRender::InitBuffers()
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	if (m_GUIshow)
	{
		int CurrToolID = m_pShapes[m_pShapes.size() - 1]->m_pShape->GetID();

		vertices.push_back({ XMFLOAT3(0, m_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width + 20, m_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width + 20, m_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(0, m_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(m_width*0.5f - 20, m_height - 120, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 20, m_height - 120, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f - 20, m_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 20, m_height - 100, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(m_width*0.5f - 17, m_height - 115, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f - 17, m_height - 110, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f, m_height - 105, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 17, m_height - 110, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 17, m_height - 115, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f, m_height - 110, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(m_width / 6.0f*CurrToolID, m_height - 100, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width / 6.0f*(CurrToolID + 1), m_height - 100, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width / 6.0f*CurrToolID, m_height, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width / 6.0f*(CurrToolID + 1), m_height, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });

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

		m_pToolBar.clear();
		m_pToolBar.push_back(new ImprovedShape(this, new sPencil()));
		m_pToolBar.push_back(new ImprovedShape(this, new sLine()));
		m_pToolBar.push_back(new ImprovedShape(this, new sPolyline()));
		m_pToolBar.push_back(new ImprovedShape(this, new sRect()));
		m_pToolBar.push_back(new ImprovedShape(this, new sRoundRect()));
		m_pToolBar.push_back(new ImprovedShape(this, new sEllips()));
		int x0 = (0 + m_width / 6.0f)*0.5f, y0 = m_height - 50;
		m_pToolBar[0]->m_pShape->MouseDown(x0, y0 - 25);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 25, y0);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 25, y0 + 25);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 20, y0 + 25);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 25, y0 + 20);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 25, y0 + 25);
		m_pToolBar[0]->m_pShape->MouseMove(x0, y0 + 25);
		m_pToolBar[0]->m_pShape->MouseMove(x0 + 25, y0);
		m_pToolBar[0]->m_pShape->MouseMove(x0 + 24, y0 - 12);
		m_pToolBar[0]->m_pShape->MouseMove(x0 + 12, y0 - 24);
		m_pToolBar[0]->m_pShape->MouseMove(x0, y0 - 25);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 25, y0);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 13, y0 + 1);
		m_pToolBar[0]->m_pShape->MouseMove(x0 + 12, y0 - 24);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 13, y0 + 1);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 1, y0 + 13);
		m_pToolBar[0]->m_pShape->MouseMove(x0 + 24, y0 - 12);
		m_pToolBar[0]->m_pShape->MouseMove(x0 - 1, y0 + 13);
		m_pToolBar[0]->m_pShape->MouseMove(x0, y0 + 25);

		x0 = (m_width / 6.0f + m_width / 6.0f * 2)*0.5f; y0 = m_height - 50;
		m_pToolBar[1]->m_pShape->MouseDown(x0 - 25, y0 + 12);
		m_pToolBar[1]->m_pShape->MouseMove(x0 + 25, y0 - 12);

		x0 = (m_width / 6.0f * 2 + m_width / 6.0f * 3)*0.5f; y0 = m_height - 50;
		m_pToolBar[2]->m_pShape->MouseDown(x0 - 25, y0 - 12);
		m_pToolBar[2]->m_pShape->MouseDown(x0 + 12, y0 - 25);
		m_pToolBar[2]->m_pShape->MouseDown(x0 - 13, y0 + 15);
		m_pToolBar[2]->m_pShape->MouseDown(x0 + 25, y0 + 25);

		x0 = (m_width / 6.0f * 3 + m_width / 6.0f * 4)*0.5f; y0 = m_height - 50;
		m_pToolBar[3]->m_pShape->MouseDown(x0 - 40, y0 - 25);
		m_pToolBar[3]->m_pShape->MouseMove(x0 + 40, y0 + 25);

		x0 = (m_width / 6.0f * 4 + m_width / 6.0f * 5)*0.5f; y0 = m_height - 50;
		m_pToolBar[4]->m_pShape->MouseDown(x0 - 40, y0 - 25);
		m_pToolBar[4]->m_pShape->MouseMove(x0 + 40, y0 + 25);
		m_pToolBar[4]->m_pShape->SetRound(10);

		x0 = (m_width / 6.0f * 5 + m_width / 6.0f * 6)*0.5f; y0 = m_height - 50;
		m_pToolBar[5]->m_pShape->MouseDown(x0 - 40, y0 - 25);
		m_pToolBar[5]->m_pShape->MouseMove(x0 + 40, y0 + 25);
	}
	else
	{
		vertices.push_back({ XMFLOAT3(m_width*0.5f - 20, m_height - 20, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 20, m_height - 20, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f - 20, m_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 20, m_height, .0f), XMFLOAT4(0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f) });

		vertices.push_back({ XMFLOAT3(m_width*0.5f - 17, m_height - 5, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f - 17, m_height - 10, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f, m_height - 15, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 17, m_height - 10, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f + 17, m_height - 5, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });
		vertices.push_back({ XMFLOAT3(m_width*0.5f, m_height - 10, .0f), XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f) });


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
	_RELEASE(m_GUIvb);
	_RELEASE(m_GUIib);
	m_GUIvb = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex)*vertices.size(), false, vertices.data());
	m_GUIib = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(DWORD)*indices.size(), false, indices.data());
	cindices = indices.size();
}
void MainRender::SetCurrentTool(byte t)
{
	switch (t)
	{
	case 0:
		m_pShapes[m_pShapes.size() - 1]->SetShape(new sPencil());
		break;
	case 1:
		m_pShapes[m_pShapes.size() - 1]->SetShape(new sLine());
		break;
	case 2:
		m_pShapes[m_pShapes.size() - 1]->SetShape(new sPolyline());
		break;
	case 3:
		m_pShapes[m_pShapes.size() - 1]->SetShape(new sRect());
		break;
	case 4:
		m_pShapes[m_pShapes.size() - 1]->SetShape(new sRoundRect());
		break;
	case 5:
		m_pShapes[m_pShapes.size() - 1]->SetShape(new sEllips());
		break;
	}
}

bool MainRender::Draw()
{
	m_pShader->Draw();

	Const cb;
	ZeroMemory(&cb, sizeof(Const));
	cb.WVP = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(0.0f, m_width, m_height, .0f, .0f, 1.0f));
	m_pImmediateContext->UpdateSubresource(m_WVP, 0, NULL, &cb, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_WVP);

	for (ImprovedShape *s : m_pShapes)
		s->Draw();

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_GUIvb, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_GUIib, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pImmediateContext->DrawIndexed(cindices, 0, 0);

	if (m_GUIshow)
		for (ImprovedShape *s : m_pToolBar)
			s->Draw();

	return true;
}

void MainRender::Close()
{
	_CLOSE(m_pShader);
	_RELEASE(m_GUIvb);
	_RELEASE(m_GUIib);
	_RELEASE(m_WVP);
	for (ImprovedShape *s : m_pShapes)
		_CLOSE(s);
	for (ImprovedShape *s : m_pToolBar)
		_CLOSE(s);
}

Input::Input(MainRender *render)
{
	m_render = render;
}
#define in(n, n1, n2) (n>=min(n1,n2)&&n<=max(n1,n2))
bool Input::MousePressed(const MouseEventClick &arg)
{
	if (arg.btn == MOUSE_LEFT)
	{
		if (!m_render->m_GUIshow)
		{
			if (in(arg.x, m_render->m_width*0.5f - 20, m_render->m_width*0.5f + 20) && in(arg.y, m_render->m_height - 20, m_render->m_height))
			{
				m_render->m_GUIshow = true;
				m_render->InitBuffers();
				return true;
			}
		}
		else
		{
			if (in(arg.x, m_render->m_width*0.5f - 20, m_render->m_width*0.5f + 20) && in(arg.y, m_render->m_height - 120, m_render->m_height - 100))
			{
				m_render->m_GUIshow = false;
				m_render->InitBuffers();
				return true;
			}
			if (arg.y > m_render->m_height - 100)
			{
				m_render->m_pShapes.push_back(new ImprovedShape(m_render));
				m_render->SetCurrentTool(arg.x / int(m_render->m_width / 6.0f));
				m_render->InitBuffers();
				return true;
			}
		}

		m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->MouseDown(arg.x, arg.y);

		if (m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->IsEnd())
		{
			m_render->m_pShapes.push_back(new ImprovedShape(m_render));
			m_render->SetCurrentTool(m_render->m_pShapes[m_render->m_pShapes.size() - 2]->m_pShape->GetID());
		}
	}
	else if (arg.btn == MOUSE_RIGHT)
	{
		m_render->m_pShapes.push_back(new ImprovedShape(m_render));
		m_render->SetCurrentTool(m_render->m_pShapes[m_render->m_pShapes.size() - 2]->m_pShape->GetID());
	}
	return true;
}
#undef in
bool Input::MouseReleased(const MouseEventClick &arg)
{
	m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->MouseUp();
	if (m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->IsEnd())
	{
		m_render->m_pShapes.push_back(new ImprovedShape(m_render));
		m_render->SetCurrentTool(m_render->m_pShapes[m_render->m_pShapes.size() - 2]->m_pShape->GetID());
	}
	return true;
}
bool Input::MouseMove(const MouseEvent &arg)
{
	m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->MouseMove(arg.x, arg.y);
	return true;
}