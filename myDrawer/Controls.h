#pragma once

class MainRender;

class Event : public vector <function<void()>>
{
	friend Event& operator+=(Event& left, const function<void()> right);
};
class Control
{
	friend class Container;
	friend class Button;
	friend class HidingTab;
	friend class HoverBox;
public:
	Control(MainRender *render);

	virtual void Draw() = 0;
	virtual bool MouseDown(const MouseEventClick &arg) { return false; };
	virtual bool MouseUp(const MouseEventClick &arg) { return false; };
	virtual bool MouseMove(const MouseEvent &arg) { return false; };

	virtual void SetCaption(const wstring text) {};

	void Unvisible() { visible = false; };
	void Visible() { visible = true; };
	bool IsVisible() const { return visible; };

	void SetPos(float x, float y) { this->x = x; this->y = y; };
	void SetSize(float width, float height) { this->width = width; this->height = height; };
	int* GetPos() const { int pos[2] = { x, y }; return pos; };
	int* GetSize() const { int size[2] = { width, height }; return size; };
	virtual void Close() {};
protected:
	MainRender *m_render;
	int x, y, width, height;
	bool visible;
};
class Controls : public vector <Control*>
{
	friend Controls& operator+=(Controls& left, Control* right);
};
class BMButton : public Control
{
public:
	BMButton(MainRender *render);
	void SetImg(wchar_t *imgstr, float width, float height);
	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	void Close() override {
		_CLOSE(m_img);
	};
	
	Event OnClick;
protected:
	bool m_isClicking;
	Image *m_img;
};
class Button : public Control
{
public:
	Button(MainRender *render);
	void SetCaption(const wstring text) override;
	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	void Close() override {
		for (Image * pimg : m_imgs)
			_CLOSE(pimg);
		m_imgs.clear();
		_CLOSE(m_ptxt);
		_CLOSE(m_pfont);
	};

	Event OnClick;
protected:
	bool m_isClicking;
	vector<Image*> m_imgs;
	BitmapFont *m_pfont;
	Text *m_ptxt;
};
class HidingTab : public Control
{
	friend class MainRender;
public:
	HidingTab(MainRender *render);
	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseMove(const MouseEvent &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	void Close() override {
		for (Image * pimg : m_imgs)
			_CLOSE(pimg);
		m_imgs.clear();
		for (Control *c : Controls)
			_CLOSE(c);
		Controls.clear();
	};

	Controls Controls;
protected:
	bool m_isButtonClicking;
	bool m_isHide;
	vector<Image*> m_imgs;
};
class CustomImgRadioButtons : public Control
{
public:
	CustomImgRadioButtons(MainRender *render);
	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	void AddButton(wstring imagename, function<void()> onClick);
	void Close() override {
		for (Image * pimg : m_imgs)
			_CLOSE(pimg);
		m_imgs.clear();
	};

	void SetChoosen(int i) { m_chosen = i; };

	Event OnClick;
protected:
	vector<Image*> m_imgs;
	unsigned int m_chosen;
};

class HScrollBar : public Control
{
public:
	HScrollBar(MainRender *render);
	~HScrollBar();
	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	bool MouseMove(const MouseEvent &arg) override;
	void Close() override {
		for (Image * pimg : m_imgs)
			_CLOSE(pimg);
		m_imgs.clear();
	};
	void SetMax(float max) { if (!IsMoving) HScrollBar::max = max; };
	void SetMin(float min) { if (!IsMoving) HScrollBar::min = min; };
	void SetValue(float &value) { if (!IsMoving) HScrollBar::value = &value; };
	void SetWidthInPercent(float pSize) { if (!IsMoving) HScrollBar::pSize = pSize; };
protected:
	vector<Image*> m_imgs;
	float *value, min, max, pSize, downx, value0;
	bool IsMoving;
	float left, right;
};


class VScrollBar : public Control
{
public:
	VScrollBar(MainRender *render);
	~VScrollBar();
	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	bool MouseMove(const MouseEvent &arg) override;
	void Close() override {
		for (Image * pimg : m_imgs)
			_CLOSE(pimg);
		m_imgs.clear();
	};
	void SetMax(float max) { if (!IsMoving) VScrollBar::max = max; };
	void SetMin(float min) { if (!IsMoving) VScrollBar::min = min; };
	void SetValue(float &value) { if (!IsMoving) VScrollBar::value = &value; };
	void SetWidthInPercent(float pSize) { if (!IsMoving) VScrollBar::pSize = pSize; };
protected:
	vector<Image*> m_imgs;
	float *value, min, max, pSize, downy, value0;
	bool IsMoving;
	float up, down;
};


class HoverBox : public Control
{
public:
	HoverBox(MainRender *render);

	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	bool MouseMove(const MouseEvent &arg) override;
	void Close() override {
		for (Control *c : m_controls)
			_CLOSE(c);
		m_controls.clear();
		_CLOSE(m_rect);
	};

	Controls m_controls;
protected:
	D3D11Framework::Rectangle *m_rect;
	vector<Image*> m_imgs;
};


class ColoredRectButton : public Control
{
public:
	ColoredRectButton(MainRender *render);

	void Draw() override;
	bool MouseDown(const MouseEventClick &arg) override;
	bool MouseUp(const MouseEventClick &arg) override;
	void Close() override {
		_CLOSE(m_rect);
	};

	void SetColor(XMFLOAT4 color);

	Event OnClick;
protected:
	bool m_isClicking;
	D3D11Framework::Rectangle *m_rect;
};