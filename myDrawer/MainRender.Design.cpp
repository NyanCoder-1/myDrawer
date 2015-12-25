#include "pch.h"
#include "MainRender.h"

void MainRender::InitializeControls(void)
{
	m_pfont = new BitmapFont(this);
	m_pfont->Init("SegoeUI16.fnt");
	m_ptxt = new Text(this, m_pfont);
	m_ptxt->Init(L"", false, 255);

	m_pShapes.push_back(new sPencil());

#define givepnz m_pnz.x, m_pnz.y
#define givescrsize m_width, m_height
	tools.push_back(new tHand(givepnz, m_pnz.zoom));
	tools.push_back(new tPencil(givepnz, givescrsize, m_pnz.zoom, m_pShapes));
	tools.push_back(new tLine(givepnz, givescrsize, m_pnz.zoom, m_pShapes));
	tools.push_back(new tPolyline(givepnz, givescrsize, m_pnz.zoom, m_pShapes));
	tools.push_back(new tRectangle(givepnz, givescrsize, m_pnz.zoom, m_pShapes));
	tools.push_back(new tRoundedRect(givepnz, givescrsize, m_pnz.zoom, m_pShapes));
	tools.push_back(new tEllips(givepnz, givescrsize, m_pnz.zoom, m_pShapes));
	tools.push_back(new tZoom(givepnz, givescrsize, m_pnz.zoom));
	currtool = tools[0];
#undef givepnz
#undef givescrsize

	m_pHidingTab = new HidingTab(this);
	m_pToolsChoose = new CustomImgRadioButtons(this);
	m_pDrawingAreaHScrollBar = new HScrollBar(this);
	m_pDrawingAreaVScrollBar = new VScrollBar(this);
	m_pColorChooser = new HoverBox(this);

	m_controls.push_back(m_pHidingTab);
	m_controls.push_back(m_pDrawingAreaHScrollBar);
	m_controls.push_back(m_pDrawingAreaVScrollBar);
	m_controls.push_back(m_pColorChooser);

	auto ht = m_pHidingTab;
	ht->SetSize(m_width, 115);
	ht->SetPos(0, m_height - 115);
	ht->Visible();
	ht->Controls += m_pToolsChoose;

	auto a = m_pToolsChoose;
	a->SetPos(0, 0);
	a->SetSize(400, 50);
	a->SetCaption(L"This is Button");
	a->Visible();
	a->AddButton(L"data\\images\\hand.png", [this]() { currtool = tools[0]; });
	a->AddButton(L"data\\images\\pencil.png", [this]() { currtool = tools[1]; currtool->CreateShape(); });
	a->AddButton(L"data\\images\\line.png", [this]() { currtool = tools[2]; currtool->CreateShape(); });
	a->AddButton(L"data\\images\\polyline.png", [this]() { currtool = tools[3]; currtool->CreateShape(); });
	a->AddButton(L"data\\images\\rectangle.png", [this]() { currtool = tools[4]; currtool->CreateShape(); });
	a->AddButton(L"data\\images\\roundrect.png", [this]() { currtool = tools[5]; currtool->CreateShape(); });
	a->AddButton(L"data\\images\\ellips.png", [this]() { currtool = tools[6]; currtool->CreateShape(); });
	a->AddButton(L"data\\images\\zoom.png", [this]() { currtool = tools[7]; });

	auto hs = m_pDrawingAreaHScrollBar;
	hs->SetPos(35, m_height - 10);
	hs->SetSize(m_width - 50, 10);
	hs->Visible();
	auto vs = m_pDrawingAreaVScrollBar;
	vs->SetPos(m_width - 10, 0);
	vs->SetSize(10, m_height - 15);
	vs->Visible();

	auto cc = m_pColorChooser;
	cc->SetSize(340, 300);
	{
		XMFLOAT4 colors[12] = {
			XMFLOAT4(1.0f,0.0f,0.0f,1.0f),XMFLOAT4(1.0f,1.0f,0.0f,1.0f),XMFLOAT4(0.0f,1.0f,0.0f,1.0f),XMFLOAT4(0.0f,1.0f,1.0f,1.0f),XMFLOAT4(0.0f,0.0f,1.0f,1.0f),XMFLOAT4(1.0f,0.0f,1.0f,1.0f),
			XMFLOAT4(0.8f,0.11372549019f,0.14117647058f,1.0f),XMFLOAT4(1.0f,0.94509803921f,0.0f,1.0f),XMFLOAT4(0.0f,0.65098039215f,0.31372549019f,1.0f),XMFLOAT4(0.0f,0.68235294117f,0.93725490196f,1.0f),XMFLOAT4(0.18431372549f,0.19215686274f,0.5725490196f,1.0f),XMFLOAT4(0.9294117647f,0.0f,1.0f,0.54901960784f),
		};
		for (int i = 0; i < 12; i++)
		{
			auto colorbtn = new ColoredRectButton(this);
			cc->m_controls += colorbtn;

			colorbtn->SetPos(i % 6 * 20 + 10, i / 6 * 20 + 10);
			colorbtn->SetSize(20, 20);
			cc->Visible();
			colorbtn->SetColor(colors[i]);
			colorbtn->OnClick += [this]() {MessageBox(Window::Get()->GetHWND(), L"", L"It's works!!!", 0); };
		}
	}
	{
		for (int i = 0; i < 20; i++)
		{
			auto colorbtn = new ColoredRectButton(this);
			cc->m_controls += colorbtn;

			colorbtn->SetPos(i % 10 * 20 + 130, i / 10 * 20 + 10);
			colorbtn->SetSize(20, 20);
			cc->Visible();
			auto color = XMFLOAT4(1.0f / 19 * (19 - i), 1.0f / 19 * (19 - i), 1.0f / 19 * (19 - i), 1.0f);
			colorbtn->SetColor(color);
			colorbtn->OnClick += [this]() { MessageBox(Window::Get()->GetHWND(), L"", L"It's works!!!", 0); };
		}
	}
	{
		for (int i = 0; i < 80; i++)
		{
			auto colorbtn = new ColoredRectButton(this);
			cc->m_controls += colorbtn;

			colorbtn->SetPos(i % 16 * 20 + 10, i / 16 * 20 + 50);
			colorbtn->SetSize(20, 20);
			cc->Visible();
			float summ = in(i / 16, 0, 1) ? (i / 16 == 0 ? 0.5f : 0.25f) : 0.0f, j = ((i / 16 == 4) || (i / 16 == 0)) ? 0.5f : (((i / 16 == 3) || (i / 16 == 1)) ? 0.75f : 1.0f), koef = i / 16.f - int(i / 16.f), r = j, g = j, b = j;
			if (in(koef, (1.0f / 6) * 0, (1.0f / 6) * 1))
			{
				r *= 1;
				g *= koef / (1.0f / 6);
				b *= 0;
			}
			else if (in(koef, (1.0f / 6) * 1, (1.0f / 6) * 2))
			{
				r *= 1.0f - (koef / (1.0f / 6) - 1);
				g *= 1;
				b *= 0;
			}
			else if (in(koef, (1.0f / 6) * 2, (1.0f / 6) * 3))
			{
				r *= 0;
				g *= 1;
				b *= koef / (1.0f / 6) - 2;
			}
			else if (in(koef, (1.0f / 6) * 3, (1.0f / 6) * 4))
			{
				r *= 0;
				g *= 1.0f - (koef / (1.0f / 6) - 3);
				b *= 1;
			}
			else if (in(koef, (1.0f / 6) * 4, (1.0f / 6) * 5))
			{
				r *= koef / (1.0f / 6) - 4;
				g *= 0;
				b *= 1;
			}
			else if (in(koef, (1.0f / 6) * 5, (1.0f / 6) * 6))
			{
				r *= 1;
				g *= 0;
				b *= 1.0f - (koef / (1.0f / 6) - 5);
			}
			auto color = XMFLOAT4(r + summ, g + summ, b + summ, 1.0f);
			colorbtn->SetColor(color);
			colorbtn->OnClick += [this]() { MessageBox(Window::Get()->GetHWND(), L"", L"It's works!!!", 0); };
		}
	}
	cc->Unvisible();
}

void MainRender::UpdatePos()
{
	m_pHidingTab->SetSize(m_width, 115);
	m_pHidingTab->SetPos(0, m_height - 115);
	m_pDrawingAreaHScrollBar->SetSize(m_width - 50, 10);
	m_pDrawingAreaVScrollBar->SetPos(m_width - 10, 0);
	if (m_pHidingTab->m_isHide)
	{
		m_pDrawingAreaHScrollBar->SetPos(35, m_height - 10);
		m_pDrawingAreaVScrollBar->SetSize(10, m_height - 15);
	}
	else
	{
		m_pDrawingAreaHScrollBar->SetPos(35, m_height - 110);
		m_pDrawingAreaVScrollBar->SetSize(10, m_height - 115);
	}
}