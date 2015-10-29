#include "pch.h"
#include "Tools.h"


#define sqr(a) ((a)*(a))
#define MY_PI (3.141592654f)
#define MY_PIDIV2 (1.570796327f)

void DrawLine(int x1, int y1, int x2, int y2) {};
void EndDraw() {};


Shape::Shape() :width(1), End(false), Changed(true)
{}

sPencil::sPencil() : is_Drawing(false)
{
	id = 0;
}
void sPencil::MouseDown(float x, float y)
{
	is_Drawing = true;
	points.push_back(Point(x, y));
	SetCapture(D3D11Framework::Window::Get()->GetHWND());
	Changed = true;
}
void sPencil::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points.push_back(Point(x, y));
		Changed = true;
	}
}
void sPencil::MouseUp()
{
	ReleaseCapture();
	is_Drawing = false;
	End = true;
}

void sPencil::Render()
{
	Lines.clear();
	for (int i = 1; i < points.size(); i++)
		Lines.push_back(Line(points[i - 1], points[i]));
}

sPolyline::sPolyline()
{
	id = 2;
	points.push_back(Point());
}
void sPolyline::MouseDown(float x, float y)
{
	points.push_back(Point(x, y));
	Changed = true;
}
void sPolyline::MouseMove(float x, float y) {
	points[points.size() - 1] = Point(x, y);
	Changed = true;
}

void sPolyline::Render()
{
	Lines.clear();
	for (int i = 1; i < points.size(); i++)
		Lines.push_back(Line(points[i - 1], points[i]));
}

sRect::sRect()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());

	id = 3;
}
void sRect::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(D3D11Framework::Window::Get()->GetHWND());
	Changed = true;
}
void sRect::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
	}
}
void sRect::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		End = true;
}

void sRect::Render()
{
	Lines.clear();
	Lines.push_back(Line(Point(points[0].x, points[0].y), Point(points[0].x, points[1].y)));
	Lines.push_back(Line(Point(points[0].x, points[1].y), Point(points[1].x, points[1].y)));
	Lines.push_back(Line(Point(points[1].x, points[1].y), Point(points[1].x, points[0].y)));
	Lines.push_back(Line(Point(points[1].x, points[0].y), Point(points[0].x, points[0].y)));
	Triangles.clear();
	vector<Point> newpoints = {
		Point(min(points[0].x, points[1].x), min(points[0].y, points[1].y)),
		Point(max(points[0].x, points[1].x), max(points[0].y, points[1].y))
	};
	Triangles.push_back(Triangle(Point(newpoints[0].x, newpoints[0].y), Point(newpoints[1].x, newpoints[0].y), Point(newpoints[0].x, newpoints[1].y)));
	Triangles.push_back(Triangle(Point(newpoints[0].x, newpoints[1].y), Point(newpoints[1].x, newpoints[0].y), Point(newpoints[1].x, newpoints[1].y)));
}

sLine::sLine()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());
	id = 1;
}
void sLine::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(D3D11Framework::Window::Get()->GetHWND());
	Changed = true;
}
void sLine::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
	}
}
void sLine::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		End = true;
}

void sLine::Render()
{
	Lines.clear();
	Lines.push_back(Line(points[0], points[1]));
}

sRoundRect::sRoundRect()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());
	radius = 0.2f;

	id = 4;
}
void sRoundRect::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(D3D11Framework::Window::Get()->GetHWND());
	Changed = true;
}
void sRoundRect::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
	}
}
void sRoundRect::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		End = true;
}

void sRoundRect::SetRound(float r)
{
	radius = r;
}

void sRoundRect::Render()
{
	Lines.clear();

	float x1 = min(points[0].x, points[1].x);
	float x2 = max(points[0].x, points[1].x);
	float y1 = min(points[0].y, points[1].y);
	float y2 = max(points[0].y, points[1].y);
	float r = radius > (x2 - x1 > y2 - y1 ? y2 - y1 : x2 - x1) / 2 ? (x2 - x1 > y2 - y1 ? y2 - y1 : x2 - x1) / 2 : radius;
	Lines.push_back(Line(Point(x1 + r, y1), Point(x2 - r, y1)));
	Lines.push_back(Line(Point(x1 + r, y2), Point(x2 - r, y2)));
	Lines.push_back(Line(Point(x1, y1 + r), Point(x1, y2 - r)));
	Lines.push_back(Line(Point(x2, y1 + r), Point(x2, y2 - r)));

	x1 += r;
	x2 -= r;
	y1 += r;
	y2 -= r;

	Triangles.clear();

	Triangles.push_back(Triangle(Point(x1, y1 - r), Point(x2, y1 - r), Point(x1, y2 + r)));
	Triangles.push_back(Triangle(Point(x1, y2 + r), Point(x2, y1 - r), Point(x2, y2 + r)));

	Triangles.push_back(Triangle(Point(x1 - r, y1), Point(x1, y1), Point(x1 - r, y2)));
	Triangles.push_back(Triangle(Point(x1 - r, y2), Point(x1, y1), Point(x1, y2)));

	Triangles.push_back(Triangle(Point(x2, y1), Point(x2 + r, y1), Point(x2, y2)));
	Triangles.push_back(Triangle(Point(x2, y2), Point(x2 + r, y1), Point(x2 + r, y2)));
	for (int i = 0; i < 101; i++)
	{
		float e = (MY_PIDIV2*0.01f);
		float fx1 = cos(e*(i - 1))*r;
		float fx2 = cos(e*i)*r;
		float fy1 = sin(e*(i - 1))*r;
		float fy2 = sin(e*i)*r;
		Lines.push_back(Line(Point(x1 - fx1, y1 - fy1), Point(x1 - fx2, y1 - fy2)));
		Lines.push_back(Line(Point(x1 - fx1, y2 + fy1), Point(x1 - fx2, y2 + fy2)));
		Lines.push_back(Line(Point(x2 + fx1, y1 - fy1), Point(x2 + fx2, y1 - fy2)));
		Lines.push_back(Line(Point(x2 + fx1, y2 + fy1), Point(x2 + fx2, y2 + fy2)));

		Triangles.push_back(Triangle(Point(x1, y1), Point(x1 - fx1, y1 - fy1), Point(x1 - fx2, y1 - fy2)));
		Triangles.push_back(Triangle(Point(x2, y1), Point(x2 + fx2, y1 - fy2), Point(x2 + fx1, y1 - fy1)));
		Triangles.push_back(Triangle(Point(x1, y2), Point(x1 - fx2, y2 + fy2), Point(x1 - fx1, y2 + fy1)));
		Triangles.push_back(Triangle(Point(x2, y2), Point(x2 + fx1, y2 + fy1), Point(x2 + fx2, y2 + fy2)));
	}
}

sEllips::sEllips()
{
	is_Drawing = false;
	points.push_back(Point());
	points.push_back(Point());

	id = 5;
}
void sEllips::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	SetCapture(D3D11Framework::Window::Get()->GetHWND());
	Changed = true;
}
void sEllips::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
	}
}
void sEllips::MouseUp()
{
	ReleaseCapture();
	if (is_Drawing)
		End = true;
}

void sEllips::Render()
{
	Lines.clear();
	Triangles.clear();
	for (int i = 1; i < 401; i++)
	{
		float minx = min(points[0].x, points[1].x);
		float miny = min(points[0].y, points[1].y);
		float maxx = max(points[0].x, points[1].x);
		float maxy = max(points[0].y, points[1].y);
		float x0 = (points[0].x + points[1].x)*0.5f;
		float y0 = (points[0].y + points[1].y)*0.5f;
		float e = MY_PI * 0.005f;
		float x1 = x0 + cos(e*i)*(maxx - minx)*0.5f;
		float y1 = y0 + sin(e*i)*(maxy - miny)*0.5f;
		float x2 = x0 + cos(e*(i - 1))*(maxx - minx)*0.5f;
		float y2 = y0 + sin(e*(i - 1))*(maxy - miny)*0.5f;
		Lines.push_back(Line(Point(x1, y1), Point(x2, y2)));
		Triangles.push_back(Triangle(Point(x0, y0), Point(x2, y2), Point(x1, y1)));
	}
}