#pragma once


enum ShapeTypes {
	t_float,
	t_color,
};

typedef struct Color
{
	Color() : r(0), g(0), b(0), a(0)
	{}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
	{};
	float r, g, b, a;
} Color;

typedef struct Point
{
	Point(float x, float y) :x(x), y(y) {}
	Point() { Point(0, 0); };
	float x, y;
} Point;

typedef struct Line
{
	Line()
	{
		points[0] = Point();
		points[1] = Point();
	}
	Line(Point point1, Point point2)
	{
		points[0] = point1;
		points[1] = point2;
	};
	Point points[2];
} Line;

typedef struct Triangle
{
	Triangle()
	{
		points[0] = Point();
		points[1] = Point();
		points[2] = Point();
	}
	Triangle(Point point1, Point point2, Point point3, Color color) : color(color)
	{
		points[0] = point1;
		points[1] = point2;
		points[2] = point3;
	};
	Point points[3];
	Color color;
} Triangle;

typedef struct Parameter
{
	void *pointers;
	ShapeTypes type;
	std::string name;
} Parameter;

class Shape
{
public:
	Shape();

	virtual bool MouseDown(float x, float y) { return false; };
	virtual bool MouseMove(float x, float y) { return false; };
	virtual bool MouseUp() { return false; };

	virtual void SetWidth(float w) { width = w; Changed = true; };
	virtual void SetRound(float r) {};

	virtual void Render() = 0;

	float GetWidth() const { return width; };
	int GetID() const { return id; };
	vector<Line> GetLines() const { return Lines; }; 
	vector<Triangle> GetTriangles() const { return Triangles; };

	bool IsEnd() const { return End; };
	bool IsChanged() {
		bool result = Changed;
		Changed = false;
		return result;
	};
	
	bool CanIDraw() const { return DrawIt; };

	void DontDraw() { DrawIt = false; };
	void YouCanDraw() { DrawIt = true; };

protected:
	float width;
	int id;
	bool End;

	bool Changed, DrawIt;
	
	vector<Line> Lines;
	vector<Triangle> Triangles;
};


class sPencil : public Shape
{
public:
	sPencil();

	bool MouseDown(float x, float y) override;
	bool MouseMove(float x, float y) override;
	bool MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sPolyline : public Shape
{
public:
	sPolyline();

	bool MouseDown(float x, float y) override;
	bool MouseMove(float x, float y) override;

	void Render() override;
protected:
	std::vector<Point> points;
};

class sRect : public Shape
{
public:
	sRect();

	bool MouseDown(float x, float y) override;
	bool MouseMove(float x, float y) override;
	bool MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sLine : public Shape
{
public:
	sLine();

	bool MouseDown(float x, float y) override;
	bool MouseMove(float x, float y) override;
	bool MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sRoundRect : public Shape
{
public:
	sRoundRect();

	bool MouseDown(float x, float y) override;
	bool MouseMove(float x, float y) override;
	bool MouseUp() override;

	void SetRound(float r) override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
	float radius;
};

class sEllips : public Shape
{
public:
	sEllips();

	bool MouseDown(float x, float y) override;
	bool MouseMove(float x, float y) override;
	bool MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};
