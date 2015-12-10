#pragma once

#include "Controls.h"
#include "Shapes.h"
#include "Tools.h"


class MainRender: public Render
{
	friend class ImprovedShape;
	friend class Input;
	friend class Control;
	friend class BMButton;
	friend class HoverBox;
public:
	MainRender();
	bool Init() override;
	bool Draw() override;
	void Close() override;
	void Resize() override;

	void SetCurrentTool(byte t);

	void UpdatePos();

private:
	/// «десь € объ€влю
	void InitializeControls(void);
	HidingTab *m_pHidingTab;
	CustomImgRadioButtons *m_pToolsChoose;
	HScrollBar *m_pDrawingAreaHScrollBar;
	VScrollBar *m_pDrawingAreaVScrollBar;
	HoverBox *m_pColorChooser;

	bool m_GUIshow;
	XMFLOAT3 m_pnz;// x, y, zoom
	XMFLOAT4 m_MinXYMaxXY;

	ID3D11Buffer *m_WVP;
	D3D11Framework::Shader *m_pShader;
	vector<Control*> m_controls;
	bool m_isMoving;
	int x0, y0, m_CurrentTool;

	BitmapFont *m_pfont;
	Text *m_ptxt;

	vector<Tools*> tools;
	Tools* currtool;

	vector<Shape*> m_pShapes;
};

class ImprovedShape
{
public:
	ImprovedShape(MainRender* render) : m_pRender(render), m_pShape(NULL), m_pVBLines(nullptr), m_pVBTriangles(nullptr) {};
	ImprovedShape(MainRender* render, Shape *shape) : m_pRender(render), m_pShape(shape), m_pVBLines(nullptr), m_pVBTriangles(nullptr) {};

	void Draw();
	void Close();

	void SetShape(Shape *shape);

	Shape *m_pShape;

protected:
	MainRender *m_pRender;
	ID3D11Buffer *m_pVBLines;
	ID3D11Buffer *m_pVBTriangles;
	size_t lsize, tsize;
};

class Input : public InputListener
{
public:
	Input(MainRender *render);
	bool MousePressed(const MouseEventClick &arg) override;
	bool MouseReleased(const MouseEventClick &arg) override;
	bool MouseMove(const MouseEvent &arg) override;

	bool KeyPressed(const KeyEvent &arg) override;
private:
	MainRender *m_render;
};