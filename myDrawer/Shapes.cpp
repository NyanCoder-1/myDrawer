#include "pch.h"
#include "Shapes.h"


#define sqr(a) ((a)*(a))
#define MY_PI (3.141592654f)
#define MY_PIDIV2 (1.570796327f)


Shape::Shape() :width(1), End(false), Changed(true)
{}

sPencil::sPencil() : is_Drawing(false)
{
	id = 0;
}
bool sPencil::MouseDown(float x, float y)
{
	is_Drawing = true;
	points.push_back(Point(x, y));
	Changed = true;
	return true;
}
bool sPencil::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points.push_back(Point(x, y));
		Changed = true;
		return true;
	}
	return false;
}
bool sPencil::MouseUp()
{
	is_Drawing = false;
	End = true;
	return true;
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
bool sPolyline::MouseDown(float x, float y)
{
	points.push_back(Point(x, y));
	Changed = true;
	return true;
}
bool sPolyline::MouseMove(float x, float y) {
	points[points.size() - 1] = Point(x, y);
	Changed = true;
	return true;
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
bool sRect::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	Changed = true;
	return true;
}
bool sRect::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
		return true;
	}
	return false;
}
bool sRect::MouseUp()
{
	if (is_Drawing)
	{
		End = true;
		return true;
	}
	return false;
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
bool sLine::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	Changed = true;
	return true;
}
bool sLine::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
		return true;
	}
	return false;
}
bool sLine::MouseUp()
{
	if (is_Drawing)
	{
		End = true;
		return true;
	}
	return false;
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
bool sRoundRect::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	Changed = true;
	return true;
}
bool sRoundRect::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
		return true;
	}
	return false;
}
bool sRoundRect::MouseUp()
{
	if (is_Drawing)
	{
		End = true;
		return true;
	}
	return false;
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
	for (int i = 1; i < 101; i++)
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
bool sEllips::MouseDown(float x, float y)
{
	is_Drawing = true;
	points[0] = Point(x, y);
	points[1] = Point(x, y);
	Changed = true;
	return true;
}
bool sEllips::MouseMove(float x, float y)
{
	if (is_Drawing)
	{
		points[1] = Point(x, y);
		Changed = true;
		return true;
	}
	return false;
}
bool sEllips::MouseUp()
{
	if (is_Drawing)
	{
		End = true;
		return true;
	}
	return false;
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