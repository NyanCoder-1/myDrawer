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
	/*if (arg.btn == MOUSE_LEFT)
	{
		if (m_render->m_CurrentTool == 6)
		{
			m_render->m_isMoving = true;
			m_render->x0 = arg.x;
			m_render->y0 = arg.y;
		}
		else if (m_render->m_CurrentTool == 7)
		{
			if (arg.btn == MOUSE_LEFT)
			{
				m_render->m_pnz.x -= (m_render->m_width * 0.5f - arg.x) / m_render->m_pnz.z;
				m_render->m_pnz.y -= (m_render->m_height * 0.5f - arg.y) / m_render->m_pnz.z;
				m_render->m_pnz.z *= 2;
				m_render->m_pnz.x += (m_render->m_width * 0.5f - arg.x) / m_render->m_pnz.z;
				m_render->m_pnz.y += (m_render->m_height * 0.5f - arg.y) / m_render->m_pnz.z;
			}
		}
		else
			if (in(m_render->m_CurrentTool, 0, 5))
			{
				m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->MouseDown(float(arg.x - (float)m_render->m_width / 2) / m_render->m_pnz.z + m_render->m_pnz.x, float(arg.y - (float)m_render->m_height / 2) / m_render->m_pnz.z + m_render->m_pnz.y);

				if (m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->IsEnd())
				{
					m_render->m_pShapes.push_back(new ImprovedShape(m_render));
					m_render->SetCurrentTool(m_render->m_pShapes[m_render->m_pShapes.size() - 2]->m_pShape->GetID());
				}
			}
		return true;
	}
	else if (arg.btn == MOUSE_RIGHT)
	{
		if (m_render->m_CurrentTool == 7)
		{
			m_render->m_pnz.x += (m_render->m_width * 0.5f - arg.x) / m_render->m_pnz.z;
			m_render->m_pnz.y += (m_render->m_height * 0.5f - arg.y) / m_render->m_pnz.z;
			m_render->m_pnz.z *= 0.5f;
			m_render->m_pnz.x -= (m_render->m_width * 0.5f - arg.x) / m_render->m_pnz.z;
			m_render->m_pnz.y -= (m_render->m_height * 0.5f - arg.y) / m_render->m_pnz.z;
		}
		else if (m_render->m_CurrentTool < 6)
		{
			m_render->m_pShapes.push_back(new ImprovedShape(m_render));
			m_render->SetCurrentTool(m_render->m_pShapes[m_render->m_pShapes.size() - 2]->m_pShape->GetID());
		}
	}
	else if (arg.btn == MOUSE_MIDDLE)
	{
		m_render->m_isMoving = true;
		m_render->x0 = arg.x;
		m_render->y0 = arg.y;
	}*/
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
	/*if (m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->IsEnd())
	{
		m_render->m_pShapes.push_back(new ImprovedShape(m_render));
		m_render->SetCurrentTool(m_render->m_pShapes[m_render->m_pShapes.size() - 2]->m_pShape->GetID());
	}
	if (res)
		return true;*/

	return true;
}
bool Input::MouseMove(const MouseEvent &arg)
{
	{
		auto h = m_render->m_pDrawingAreaHScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.x + (m_render->m_width / m_render->m_pnz.z) / 2, m_render->m_pnz.x);
		float Max = max(m_render->m_MinXYMaxXY.z - (m_render->m_width / m_render->m_pnz.z) / 2, m_render->m_pnz.x);
		h->SetMin(Min);
		h->SetMax(Max);
		h->SetValue(m_render->m_pnz.x);
		h->SetWidthInPercent(m_render->m_width / m_render->m_pnz.z / (Max - Min + m_render->m_width / m_render->m_pnz.z));
	}
	{
		auto v = m_render->m_pDrawingAreaVScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.y + (m_render->m_height / m_render->m_pnz.z) / 2, m_render->m_pnz.y);
		float Max = max(m_render->m_MinXYMaxXY.w - (m_render->m_height / m_render->m_pnz.z) / 2, m_render->m_pnz.y);
		v->SetMin(Min);
		v->SetMax(Max);
		v->SetValue(m_render->m_pnz.y);
		v->SetWidthInPercent(m_render->m_height / m_render->m_pnz.z / (Max - Min + m_render->m_height / m_render->m_pnz.z));
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
		m_render->m_pnz.x -= (arg.x - m_render->x0) / m_render->m_pnz.z;
		m_render->m_pnz.y -= (arg.y - m_render->y0) / m_render->m_pnz.z;
		m_render->x0 = arg.x;
		m_render->y0 = arg.y;
		return true;
	}
	bool res = m_render->currtool->MouseMove(arg.x, arg.y);
	if (m_render->m_pShapes[m_render->m_pShapes.size() - 1]->m_pShape->MouseMove(
		float(arg.x - (int)m_render->m_width / 2) / m_render->m_pnz.z + m_render->m_pnz.x,
		float(arg.y - (int)m_render->m_height / 2) / m_render->m_pnz.z + m_render->m_pnz.y))
	{
		/*m_render->m_MinXYMaxXY.x = min(m_render->m_MinXYMaxXY.x, float(arg.x - (int)m_render->m_width / 2) / m_render->m_pnz.z + m_render->m_pnz.x);
		m_render->m_MinXYMaxXY.y = min(m_render->m_MinXYMaxXY.y, float(arg.y - (int)m_render->m_height / 2) / m_render->m_pnz.z + m_render->m_pnz.y);
		m_render->m_MinXYMaxXY.z = max(m_render->m_MinXYMaxXY.z, float(arg.x - (int)m_render->m_width / 2) / m_render->m_pnz.z + m_render->m_pnz.x);
		m_render->m_MinXYMaxXY.w = max(m_render->m_MinXYMaxXY.w, float(arg.y - (int)m_render->m_height / 2) / m_render->m_pnz.z + m_render->m_pnz.y);*/
		return true;
	}
	return true;
}

bool Input::KeyPressed(const KeyEvent &arg)
{
	if (arg.code == KEY_ADD)
		m_render->m_pnz.z *= 2;
	else if (arg.code == KEY_SUBTRACT)
		m_render->m_pnz.z *= 0.5f;
	{
		auto h = m_render->m_pDrawingAreaHScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.x + (m_render->m_width / m_render->m_pnz.z) / 2, m_render->m_pnz.x);
		float Max = max(m_render->m_MinXYMaxXY.z - (m_render->m_width / m_render->m_pnz.z) / 2, m_render->m_pnz.x);
		h->SetMin(Min);
		h->SetMax(Max);
		h->SetValue(m_render->m_pnz.x);
		h->SetWidthInPercent(m_render->m_width / m_render->m_pnz.z / (Max - Min + m_render->m_width / m_render->m_pnz.z));
	}
	{
		auto v = m_render->m_pDrawingAreaVScrollBar;
		float Min = min(m_render->m_MinXYMaxXY.y + (m_render->m_height / m_render->m_pnz.z) / 2, m_render->m_pnz.y);
		float Max = max(m_render->m_MinXYMaxXY.w - (m_render->m_height / m_render->m_pnz.z) / 2, m_render->m_pnz.y);
		v->SetMin(Min);
		v->SetMax(Max);
		v->SetValue(m_render->m_pnz.y);
		v->SetWidthInPercent(m_render->m_height / m_render->m_pnz.z / (Max - Min + m_render->m_height / m_render->m_pnz.z));
	}
	return true;
}