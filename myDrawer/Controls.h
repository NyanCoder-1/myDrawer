#pragma once

class Event : public vector <function<void()>>
{
	friend Event& operator+=(Event& left, const function<void()> right);
};
class Control
{
	friend class Container;
	friend class Button;
public:
	Control(MainRender *render);

	virtual void Draw() = 0;
	virtual void MouseDown(const MouseEventClick &arg) {};
	virtual void MouseUp(const MouseEventClick &arg) {};
	virtual void MouseMove(const MouseEvent &arg) {};

	virtual void Init(float x, float y, float width, float height) { this->x = x; this->y = y; this->width = width; this->height = height; };
	virtual void Close() {};
protected:
	MainRender *m_render;
	int x, y, width, height;
};
class Controls : public vector <Control*>
{
	friend Controls& operator+=(Controls& left, Control* right);
};

class Container : public Control
{
public:
	void Draw() override;
	void MouseDown(const MouseEventClick &arg) override;
	void MouseUp(const MouseEventClick &arg) override;
	void MouseMove(const MouseEvent &arg) override;

	void Close() override {
		for (Control* c : Controls)
			_CLOSE(c);
	};
	Controls Controls;
private:
};
class BMButton : public Control
{
public:
	void SetImg(wchar_t *imgstr, float width, float height);
	void Draw() override;
	void MouseDown(const MouseEventClick &arg) override;

	void Init(wchar_t *bgstr, float width, float height);
	void Close() override {
		_CLOSE(m_bg);
		_CLOSE(m_img);
	};
	
	Event OnClick;
protected:
	Image *m_bg, *m_img;
};