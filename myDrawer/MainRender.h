#pragma once

#include "Tools.h"


class MainRender: public Render
{
	friend class ImprovedShape;
	friend class Input;
public:
	MainRender();
	bool Init(HWND hwnd) override;
	bool Draw() override;
	void Close() override;
	void Resize(int height, int width) override;

	void SetCurrentTool(byte t);

	void InitBuffers();

private:
	DirectX::XMMATRIX g_matOrtho;

	int cindices;
	bool m_GUIshow;

	ID3D11Buffer *m_GUIvb;
	ID3D11Buffer *m_GUIib;
	ID3D11Buffer *m_WVP;
	myD3D11Framework::Shader *m_pShader;

	vector<ImprovedShape*> m_pShapes;
	vector<ImprovedShape*> m_pToolBar;
};

class ImprovedShape
{
public:
	ImprovedShape(MainRender* render) : m_pRender(render), m_pShape(NULL), m_pVB(nullptr) {};
	ImprovedShape(MainRender* render, Shape *shape) : m_pRender(render), m_pShape(shape), m_pVB(nullptr) {};

	void Draw();
	void Close();

	void SetShape(Shape *shape);

	Shape *m_pShape;

protected:
	MainRender *m_pRender;
	ID3D11Buffer *m_pVB;
	size_t size;
};

class Input : public InputListener
{
public:
	Input(MainRender *render);
	bool MousePressed(const MouseEventClick &arg) override;
	bool MouseReleased(const MouseEventClick &arg) override;
	bool MouseMove(const MouseEvent &arg) override;
private:
	MainRender *m_render;
};