#pragma once

class Buffer
{
public:
	static ID3D11Buffer* CreateVertexBuffer(int size, bool dynamic, const void *Mem);
	static ID3D11Buffer* CreateIndexBuffer(int size, bool dynamic, const void *Mem);
	static ID3D11Buffer* CreateConstantBuffer(int size, bool dynamic);
};