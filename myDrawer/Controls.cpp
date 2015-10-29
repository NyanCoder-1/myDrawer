#include "pch.h"
#include "MainRender.h"
#include "Controls.h"



Event& operator+=(Event& left, const function<void()> right)
{
	left.push_back(right);
	return left;
}
Controls& operator+=(Controls& left, Control* right)
{
	left.push_back(right);
	return left;
}

Control::Control(MainRender *render) : m_render(render)
{
}

void Container::Draw()
{
	for (Control* c : Controls)
		c->Draw();
}
void Container::MouseDown(const MouseEventClick &arg)
{
	for (Control* c : Controls)
		if (arg.x >= c->x, arg.x < c->x + c->width, arg.y >= c->y, arg.y < c->y + c->height)
		{
			c->MouseDown(arg);
			return;
		}
}
void Container::MouseUp(const MouseEventClick &arg)
{
	for (Control* c : Controls)
		if (arg.x >= c->x, arg.x < c->x + c->width, arg.y >= c->y, arg.y < c->y + c->height)
		{
			c->MouseUp(arg);
			return;
		}
}
void Container::MouseMove(const MouseEvent &arg)
{
	for (Control* c : Controls)
		if (arg.x >= c->x, arg.x < c->x + c->width, arg.y >= c->y, arg.y < c->y + c->height)
			c->MouseMove(arg);
}


void BMButton::SetImg(wchar_t *imgstr, float width, float height)
{
	m_img = new Image(m_render);
	m_img->Init(imgstr, width, height);
}
void BMButton::Draw()
{
	m_bg->Draw(x, y);
	m_img->Draw(x, y);
}
void BMButton::MouseDown(const MouseEventClick &arg)
{
}

void BMButton::Init(wchar_t *bgstr, float width, float height)
{
	m_bg = new Image(m_render);
	m_bg->Init(bgstr, width, height);
}