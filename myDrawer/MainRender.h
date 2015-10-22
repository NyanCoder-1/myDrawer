#pragma once

#include "Tools.h"


class MainRender: public Render
{
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

	vector<Shape*> m_pShapes;
	vector<Shape*> m_pToolBar;
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