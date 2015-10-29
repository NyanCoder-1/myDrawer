#pragma once


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
	Triangle(Point point1, Point point2, Point point3)
	{
		points[0] = point1;
		points[1] = point2;
		points[2] = point3;
	};
	Point points[3];
} Triangle;

class Shape
{
public:
	Shape();

	virtual void MouseDown(float x, float y) {};
	virtual void MouseMove(float x, float y) {};
	virtual void MouseUp() {};

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

protected:
	float width;
	int id;
	bool End;

	bool Changed;
	
	vector<Line> Lines;
	vector<Triangle> Triangles;
};


class sPencil : public Shape
{
public:
	sPencil();

	void MouseDown(float x, float y) override;
	void MouseMove(float x, float y) override;
	void MouseUp() override;

	void Render() override;
protected:
	bool is_Drawing;
	std::vector<Point> points;
};

class sPolyline : public Shape
{
public:
	sPolyline();

	void MouseDown(float x, float y) override;
	void MouseMove(float x, float y) override;

	void Render() override;
protected:
	std::vector<Point> points;
};

class sRect : public Shape
{
public:
	sRect();

	void MouseDown(float x, float y) override;
	void MouseMove(float x, float y) override;
	void MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sLine : public Shape
{
public:
	sLine();

	void MouseDown(float x, float y) override;
	void MouseMove(float x, float y) override;
	void MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sRoundRect : public Shape
{
public:
	sRoundRect();

	void MouseDown(float x, float y) override;
	void MouseMove(float x, float y) override;
	void MouseUp() override;

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

	void MouseDown(float x, float y) override;
	void MouseMove(float x, float y) override;
	void MouseUp() override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};
