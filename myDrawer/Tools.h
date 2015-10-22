#pragma once


typedef struct Point
{
	Point(int x, int y) :x(x), y(y) {}
	Point() { Point(0, 0); };
	int x, y;
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

class Shape
{
public:
	Shape();

	virtual void MouseDown(int x, int y) = 0;
	virtual void MouseMove(int x, int y) = 0;
	virtual void MouseUp() = 0;

	virtual void SetWidth(unsigned char w) = 0;
	virtual void SetRound(int r) = 0;

	virtual void Render() = 0;

	int GetID() const { return id; };
	vector<Line> GetLines() const { return Lines; };

	bool IsEnd() const { return End; };

protected:
	unsigned char width;
	int id;
	bool End;
	
	vector<Line> Lines;
};


class sPencil : public Shape
{
public:
	sPencil();

	void MouseDown(int x, int y) override;
	void MouseMove(int x, int y) override;
	void MouseUp() override;

	void SetWidth(unsigned char w) override;
	void SetRound(int r) override {};

	void Render() override;
protected:
	bool is_Drawing;
	std::vector<Point> points;
};

class sPolyline : public Shape
{
public:
	sPolyline();

	void MouseDown(int x, int y) override;
	void MouseMove(int x, int y) override;
	void MouseUp() override;

	void SetWidth(unsigned char w) override;
	void SetRound(int r) override {};

	void Render() override;
protected:
	std::vector<Point> points;
};

class sRect : public Shape
{
public:
	sRect();

	void MouseDown(int x, int y) override;
	void MouseMove(int x, int y) override;
	void MouseUp() override;

	void SetWidth(unsigned char w) override;
	void SetRound(int r) override {};

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sLine : public Shape
{
public:
	sLine();

	void MouseDown(int x, int y) override;
	void MouseMove(int x, int y) override;
	void MouseUp() override;

	void SetWidth(unsigned char w) override;
	void SetRound(int r) override {};

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};

class sRoundRect : public Shape
{
public:
	sRoundRect();

	void MouseDown(int x, int y) override;
	void MouseMove(int x, int y) override;
	void MouseUp() override;

	void SetWidth(unsigned char w) override;
	void SetRound(int r) override;

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
	int radius;
};

class sEllips : public Shape
{
public:
	sEllips();

	void MouseDown(int x, int y) override;
	void MouseMove(int x, int y) override;
	void MouseUp() override;

	void SetWidth(unsigned char w) override;
	void SetRound(int r) override {};

	void Render() override;
protected:
	std::vector<Point> points;
	bool is_Drawing;
};
