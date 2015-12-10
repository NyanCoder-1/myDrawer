#pragma once
#include "Render.h"

namespace D3D11Framework
{
//------------------------------------------------------------------

	class DLL Rectangle
	{
	public:
		Rectangle(Render *render);

		bool Init(float Width, float Height);
		void Draw(float positionX, float positionY);
		void Close();

		bool SetSize(float Width, float Height);
		void SetColor(XMFLOAT4 color) { m_color = color; m_InitBuffers(); };
		XMFLOAT2 GetSize() const { return XMFLOAT2(m_Width, m_Height); };

	protected:
		bool m_InitBuffers();
		void m_SetShaderParameters(float x, float y);

		Render *m_render;

		ID3D11Buffer *m_vertexBuffer;
		ID3D11Buffer *m_indexBuffer;
		ID3D11Buffer *m_constantBuffer;
		Shader *m_shader;

		XMFLOAT4 m_color;

		float m_Width, m_Height;
	};

//------------------------------------------------------------------
}
