#pragma once

#include "Shapes.h"

enum MBtn
{
	BtnLeft = 0,
	BtnMiddle,
	BtnRight
};

class Tools
{
public:
	virtual bool MouseDown(MBtn button, float x, float y) { return 0; };
	virtual bool MouseMove(float x, float y) { return 0; };
	virtual bool MouseUp() { return 0; };

	virtual void CreateShape() {};

	void SetZoomVar(float &zoom) { Zoom = &zoom; };
	void SetPosVars(float &x, float &y) { X = &x; Y = &y; };
	void SetScreenSize(unsigned int &width, unsigned int &height) { Width = &width; Height = &height; };

protected:
	float *Zoom, *X, *Y;
	unsigned int *Width, *Height;
	vector<Shape*> *Shapes;
};

class tHand : public Tools
{
public:
	tHand(float &x, float &y, float &zoom) { SetPosVars(x, y); SetZoomVar(zoom); };

	virtual bool MouseDown(MBtn button, float x, float y) override;
	virtual bool MouseMove(float x, float y) override;
	virtual bool MouseUp() override;
private:
	bool isMoving;
	float prevX, prevY;
};

class tShape : public Tools
{
public:
	tShape(float &x, float &y, float &zoom, vector<Shape*> &shapes) { SetPosVars(x, y); SetZoomVar(zoom); Shapes = &shapes; };

	virtual bool MouseDown(MBtn button, float x, float y) override;
	virtual bool MouseMove(float x, float y) override;
	virtual bool MouseUp() override;

	virtual void CreateShape() {};
};

class tZoom : public Tools
{
public:
	tZoom(float &x, float &y, unsigned int &width, unsigned int &height, float &zoom) { SetZoomVar(zoom); SetPosVars(x, y); SetScreenSize(width, height); };

	virtual bool MouseDown(MBtn button, float x, float y) override;
};


class tPencil : public tShape
{
public:
	tPencil(float &x, float &y, float &zoom, vector<Shape*> &shapes) : tShape(x, y, zoom, shapes) {};
	virtual void CreateShape() override;
};
class tLine : public tShape
{
public:
	tLine(float &x, float &y, float &zoom, vector<Shape*> &shapes) : tShape(x, y, zoom, shapes) {};
	virtual void CreateShape() override;
};
class tPolyline : public tShape
{
public:
	tPolyline(float &x, float &y, float &zoom, vector<Shape*> &shapes) : tShape(x, y, zoom, shapes) {};
	virtual void CreateShape() override;
};
class tRectangle : public tShape
{
public:
	tRectangle(float &x, float &y, float &zoom, vector<Shape*> &shapes) : tShape(x, y, zoom, shapes) {};
	virtual void CreateShape() override;
};
class tRoundedRect : public tShape
{
public:
	tRoundedRect(float &x, float &y, float &zoom, vector<Shape*> &shapes) : tShape(x, y, zoom, shapes) {};
	virtual void CreateShape() override;
};
class tEllips : public tShape
{
public:
	tEllips(float &x, float &y, float &zoom, vector<Shape*> &shapes) : tShape(x, y, zoom, shapes) {};
	virtual void CreateShape() override;
};