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
	(*Shapes)[Shapes->size() - 1]->MouseDown((x - *Width / 2) / (*Zoom) + *X, (y - *Height / 2) / (*Zoom) + *Y);
	return true;
}
bool tShape::MouseMove(float x, float y)
{
	(*Shapes)[Shapes->size() - 1]->MouseMove((x - *Width / 2) / (*Zoom) + *X, (y - *Height / 2) / (*Zoom) + *Y);
	return true;
}
bool tShape::MouseUp()
{
	(*Shapes)[Shapes->size() - 1]->MouseUp();
	if ((*Shapes)[Shapes->size() - 1]->IsEnd())
		CreateShape();
	return true;
}
void tPencil::CreateShape()
{
	if ((*Shapes)[Shapes->size() - 1]->GetLines().size()>0)
		Shapes->push_back(new sPencil);
	else
	{
		delete (*Shapes)[Shapes->size() - 1];
		(*Shapes)[Shapes->size() - 1] = new sPencil();
	}
}
void tLine::CreateShape()
{
	if ((*Shapes)[Shapes->size() - 1]->GetLines().size()>0)
		Shapes->push_back(new sLine);
	else
	{
		delete (*Shapes)[Shapes->size() - 1];
		(*Shapes)[Shapes->size() - 1] = new sLine();
	}
}
void tPolyline::CreateShape()
{
	if ((*Shapes)[Shapes->size() - 1]->GetLines().size()>0)
		Shapes->push_back(new sPolyline);
	else
	{
		delete (*Shapes)[Shapes->size() - 1];
		(*Shapes)[Shapes->size() - 1] = new sPolyline();
	}
}
void tRectangle::CreateShape()
{
	if ((*Shapes)[Shapes->size() - 1]->GetLines().size()>0)
		Shapes->push_back(new sRect);
	else
	{
		delete (*Shapes)[Shapes->size() - 1];
		(*Shapes)[Shapes->size() - 1] = new sRect();
	}
}
void tRoundedRect::CreateShape()
{
	if ((*Shapes)[Shapes->size() - 1]->GetLines().size()>0)
		Shapes->push_back(new sRoundRect);
	else
	{
		delete (*Shapes)[Shapes->size() - 1];
		(*Shapes)[Shapes->size() - 1] = new sRoundRect();
	}
}
void tEllips::CreateShape()
{
	if ((*Shapes)[Shapes->size() - 1]->GetLines().size()>0)
		Shapes->push_back(new sEllips);
	else
	{
		delete (*Shapes)[Shapes->size() - 1];
		(*Shapes)[Shapes->size() - 1] = new sPencil();
	}
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
