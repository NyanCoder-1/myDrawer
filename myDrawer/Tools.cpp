#include "pch.h"
#include "Tools.h"


#define sqr(a) ((a)*(a))
#define MY_PI (3.141592654f)
#define MY_PIDIV2 (1.570796327f)

void DrawDot(int x1, int y1, int x2, int y2);
void DrawLine(int x1, int y1, int x2, int y2);
void EndDraw();


Shape::Shape() :width(1)
{}

sPencil::sPencil() : is_Drawing(false)
{}
void sPencil::MouseDown(int x, int y)
{
	is_Drawing = true;
	points.push_back(Point(x, y));
	SetCapture(g_hWnd);
}
void sPencil::MouseMove(int x, int y)
{
	if (is_Drawing)
		points.push_back(Point(x, y));
}
void sPencil::MouseUp()
{
	ReleaseCapture();
	is_Drawing = false;
	EndDraw();
}

void sPencil::SetWidth(unsigned char w)
{
	width = w;
}

void sPencil::Render()
{
	for (int i = 1; i < points.size(); i++)
		DrawLine(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
}

sPolyline::sPolyline()
{
}
void sPolyline::MouseDown(int x, int y)
{
	if (points.size() > 1 && sqr(x - points[0].x) + sqr(y - points[0].y)<25)
	{
		points.push_back(points[0]);
		EndDraw();
	}
	else
		points.push_back(Point(x, y));
}
void sPolyline::MouseMove(int x, int y)
{
}
void sPolyline::MouseUp()
{
}

void sPolyline::SetWidth(unsigned char w)
{
	width = w;
}

void sPolyline::Render()
{
	for (int i = 1; i < points.size(); i++)
		DrawLine(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
}

sRect::sRect()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());
}
void sRect::MouseDown(int x, int y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(g_hWnd);
}
void sRect::MouseMove(int x, int y)
{
	if (is_Drawing)
		points[1] = Point(x, y);
}
void sRect::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		EndDraw();
}

void sRect::SetWidth(unsigned char w)
{
	width = w;
}

void sRect::Render()
{
	DrawLine(points[0].x, points[0].y, points[0].x, points[1].y);
	DrawLine(points[0].x, points[1].y, points[1].x, points[1].y);
	DrawLine(points[1].x, points[1].y, points[1].x, points[0].y);
	DrawLine(points[1].x, points[0].y, points[0].x, points[0].y);
}

sLine::sLine()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());
}
void sLine::MouseDown(int x, int y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(g_hWnd);
}
void sLine::MouseMove(int x, int y)
{
	if (is_Drawing)
		points[1] = Point(x, y);
}
void sLine::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		EndDraw();
}

void sLine::SetWidth(unsigned char w)
{
	width = w;
}

void sLine::Render()
{
	DrawLine(points[0].x, points[0].y, points[1].x, points[1].y);
}

sRoundRect::sRoundRect()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());
	radius = 20;
}
void sRoundRect::MouseDown(int x, int y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(g_hWnd);
}
void sRoundRect::MouseMove(int x, int y)
{
	if (is_Drawing)
		points[1] = Point(x, y);
}
void sRoundRect::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		EndDraw();
}

void sRoundRect::SetWidth(unsigned char w)
{
	width = w;
}
void sRoundRect::SetRound(int r)
{
	radius = r;
}

void sRoundRect::Render()
{
	int x1 = min(points[0].x, points[1].x);
	int x2 = max(points[0].x, points[1].x);
	int y1 = min(points[0].y, points[1].y);
	int y2 = max(points[0].y, points[1].y);
	int r = radius > (x2 - x1 > y2 - y1 ? y2 - y1 : x2 - x1) / 2 ? (x2 - x1 > y2 - y1 ? y2 - y1 : x2 - x1) / 2 : radius;
	DrawLine(x1 + r, y1, x2 - r, y1);
	DrawLine(x1 + r, y2, x2 - r, y2);
	DrawLine(x1, y1 + r, x1, y2 - r);
	DrawLine(x2, y1 + r, x2, y2 - r);
	x1 += r;
	x2 -= r;
	y1 += r;
	y2 -= r;
	for (int i = 0; i < 100; i++)
	{
		float e = (MY_PIDIV2*0.01f);
		int fx1 = cos(e*(i - 1))*r;
		int fx2 = cos(e*i)*r;
		int fy1 = sin(e*(i - 1))*r;
		int fy2 = sin(e*i)*r;
		DrawLine(x1 - fx1, y1 - fy1, x1 - fx2, y1 - fy2);
		DrawLine(x1 - fx1, y2 + fy1, x1 - fx2, y2 + fy2);
		DrawLine(x2 + fx1, y1 - fy1, x2 + fx2, y1 - fy2);
		DrawLine(x2 + fx1, y2 + fy1, x2 + fx2, y2 + fy2);
	}
}

sEllips::sEllips()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());
}
void sEllips::MouseDown(int x, int y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(g_hWnd);
}
void sEllips::MouseMove(int x, int y)
{
	if (is_Drawing)
		points[1] = Point(x, y);
}
void sEllips::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		EndDraw();
}

void sEllips::SetWidth(unsigned char w)
{
	width = w;
}

void sEllips::Render()
{
	for (int i = 1; i < 401; i++)
	{
		int x0 = (points[0].x + points[1].x)*0.5f;
		int y0 = (points[0].y + points[1].y)*0.5f;
		float e = MY_PI * 0.005f;
		DrawLine(x0 + cos(e*i)*(points[0].x - points[1].x)*0.5f, y0 + sin(e*i)*(points[0].y - points[1].y)*0.5f, x0 + cos(e*(i - 1))*(points[0].x - points[1].x)*0.5f, y0 + sin(e*(i - 1))*(points[0].y - points[1].y)*0.5f);
	}
}