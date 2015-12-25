#include "pch.h"
#include "MainRender.h"


Input::Input(MainRender *render)
{
	m_render = render;
}
bool Input::MousePressed(const MouseEventClick &arg)
{
	SetCapture(D3D11Framework::Window::Get()->GetHWND());

	for (int i = m_render->m_controls.size() - 1; i >= 0; i--)
	{
		auto c = m_render->m_controls[i];
		if (c->IsVisible())
		{
			if (c->MouseDown(arg))
				return true;
		}
	}
	m_render->currtool->MouseDown(MBtn(arg.btn), arg.x, arg.y);
	return true;
}
#undef in
bool Input::MouseReleased(const MouseEventClick &arg)
{
	ReleaseCapture();

	for (int i = m_render->m_controls.size() - 1; i >= 0; i--)
	{
		auto c = m_render->m_controls[i];
		if (c->IsVisible())
			if (c->MouseUp(arg))
				return true;
	}
	if (m_render->m_isMoving)
	{
		if (m_render->m_isMoving)
			m_render->m_isMoving = false;
		return true;
	}
	m_render->currtool->MouseUp();

	return true;
}
bool Input::MouseMove(const MouseEvent &arg)
{
	{
		auto h = m_render->m_pDrawingAreaHScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.x + (m_render->m_width / m_render->m_pnz.zoom) / 2, m_render->m_pnz.x);
		float Max = max(m_render->m_MinXYMaxXY.z - (m_render->m_width / m_render->m_pnz.zoom) / 2, m_render->m_pnz.x);
		h->SetMin(Min);
		h->SetMax(Max);
		h->SetValue(m_render->m_pnz.x);
		h->SetWidthInPercent(m_render->m_width / m_render->m_pnz.zoom / (Max - Min + m_render->m_width / m_render->m_pnz.zoom)); // Длина скроллера в процентах
	}
	{
		auto v = m_render->m_pDrawingAreaVScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.y + (m_render->m_height / m_render->m_pnz.zoom) / 2, m_render->m_pnz.y);
		float Max = max(m_render->m_MinXYMaxXY.w - (m_render->m_height / m_render->m_pnz.zoom) / 2, m_render->m_pnz.y);
		v->SetMin(Min);
		v->SetMax(Max);
		v->SetValue(m_render->m_pnz.y);
		v->SetWidthInPercent(m_render->m_height / m_render->m_pnz.zoom / (Max - Min + m_render->m_height / m_render->m_pnz.zoom));
	}
	for (int i = m_render->m_controls.size() - 1; i >= 0; i--)
	{
		auto c = m_render->m_controls[i];
		if (c->IsVisible())
			if (c->MouseMove(arg))
				return true;
	}
	if (m_render->m_isMoving)
	{
		m_render->m_pnz.x -= (arg.x - m_render->x0) / m_render->m_pnz.zoom;
		m_render->m_pnz.y -= (arg.y - m_render->y0) / m_render->m_pnz.zoom;
		m_render->x0 = arg.x;
		m_render->y0 = arg.y;
		return true;
	}
	bool res = m_render->currtool->MouseMove(arg.x, arg.y);
	return true;
}

bool Input::KeyPressed(const KeyEvent &arg)
{
	if (arg.code == KEY_ADD)
		m_render->m_pnz.zoom *= 2;
	else if (arg.code == KEY_SUBTRACT)
		m_render->m_pnz.zoom *= 0.5f;
	{
		auto h = m_render->m_pDrawingAreaHScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.x + (m_render->m_width / m_render->m_pnz.zoom) / 2, m_render->m_pnz.x);
		float Max = max(m_render->m_MinXYMaxXY.z - (m_render->m_width / m_render->m_pnz.zoom) / 2, m_render->m_pnz.x);
		h->SetMin(Min);
		h->SetMax(Max);
		h->SetValue(m_render->m_pnz.x);
		h->SetWidthInPercent(m_render->m_width / m_render->m_pnz.zoom / (Max - Min + m_render->m_width / m_render->m_pnz.zoom));
	}
	{
		auto v = m_render->m_pDrawingAreaVScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.y + (m_render->m_height / m_render->m_pnz.zoom) / 2, m_render->m_pnz.y);
		float Max = max(m_render->m_MinXYMaxXY.w - (m_render->m_height / m_render->m_pnz.zoom) / 2, m_render->m_pnz.y);
		v->SetMin(Min);
		v->SetMax(Max);
		v->SetValue(m_render->m_pnz.y);
		v->SetWidthInPercent(m_render->m_height / m_render->m_pnz.zoom / (Max - Min + m_render->m_height / m_render->m_pnz.zoom));
	}
	return true;
}