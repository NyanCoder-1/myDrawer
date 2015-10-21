#pragma once

class MainRender: public Render
{
public:
	MainRender();
	bool Init(HWND hwnd);
	bool Draw();
	void Close();

private:
	DirectX::XMMATRIX g_matOrtho;

	int cindices;

	ID3D11Buffer *g_GUIvb;
	ID3D11Buffer *g_GUIib;
	ID3D11Buffer *g_WVP;
	Shader *m_pShader;
};

