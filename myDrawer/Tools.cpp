#include "pch.h"
#include "Tools.h"

bool tHand::MouseDown(MBtn button, float x, float y)
{
	isMoving = true;
	prevX = x / (*Zoom);
	prevY = y / (*Zoom);
	return true;
}
bool tHand::MouseMove(float x, float y)
{
	if (isMoving)
	{
		*X += prevX - x / (*Zoom);
		*Y += prevY - y / (*Zoom);
		prevX = x / (*Zoom);
		prevY = y / (*Zoom);
	}
	return true;
}
bool tHand::MouseUp()
{
	isMoving = false;
	return true;
}

bool tShape::MouseDown(MBtn button, float x, float y)
{
	if ((*Shapes)[Shapes->size() - 1]->IsEnd())
		CreateShape();
	return true;
}
bool tShape::MouseMove(float x, float y)
{
	return true;
}
bool tShape::MouseUp()
{
	return true;
}
void tPencil::CreateShape()
{
	Shapes->push_back(new sPencil);
}
void tLine::CreateShape()
{
	Shapes->push_back(new sLine);
}
void tPolyline::CreateShape()
{
	Shapes->push_back(new sPolyline);
}
void tRectangle::CreateShape()
{
	Shapes->push_back(new sRect);
}
void tRoundedRect::CreateShape()
{
	Shapes->push_back(new sRoundRect);
}
void tEllips::CreateShape()
{
	Shapes->push_back(new sEllips);
}

bool tZoom::MouseDown(MBtn button, float x, float y)
{
	if (button == BtnLeft)
	{
		*X -= (*Width * 0.5f - x) / *Zoom;
		*Y -= (*Height * 0.5f - y) / *Zoom;
		*Zoom *= 2;
		*X += (*Width * 0.5f - x) / *Zoom;
		*Y += (*Height * 0.5f - y) / *Zoom;
	}
	else if (button == BtnRight)
	{
		*X -= (*Width * 0.5f - x) / *Zoom;
		*Y -= (*Height * 0.5f - y) / *Zoom;
		*Zoom *= 0.5f;
		*X += (*Width * 0.5f - x) / *Zoom;
		*Y += (*Height * 0.5f - y) / *Zoom;
	}
	return true;
}
