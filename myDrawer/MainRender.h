#pragma once

#include "Tools.h"


class MainRender: public Render
{
	friend class ImprovedShape;
	friend class Input;
public:
	MainRender();
	bool Init() override;
	bool Draw() override;
	void Close() override;
	void Resize() override;

	void SetCurrentTool(byte t);

	void InitBuffers();

private:
	int cindices;
	bool m_GUIshow;
	XMFLOAT3 m_pnz;// x, y, zoom

	ID3D11Buffer *m_GUIvb;
	ID3D11Buffer *m_GUIib;
	ID3D11Buffer *m_WVP;
	D3D11Framework::Shader *m_pShader;
	Text *m_ptxt;
	Image *m_pimg;
	BitmapFont *m_pbmf;
	bool m_isMoving;
	int x0, y0;

	vector<ImprovedShape*> m_pShapes;
	vector<ImprovedShape*> m_pToolBar;
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