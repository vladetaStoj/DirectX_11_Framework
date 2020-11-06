#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "graphics.h"

//Global primitive types declerations go here
struct Vertex_XYZ
{
	Vertex_XYZ() {}
	Vertex_XYZ(float x, float y, float z) : pos(x, y, z) {}

	XMFLOAT3 pos;
};

struct Vertex_XYZRGB  
{
	Vertex_XYZRGB() {}
	Vertex_XYZRGB(float x, float y, float z,
		float cr, float cg, float cb, float ca)
		: pos(x, y, z), color(cr, cg, cb, ca) {}

	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct Vertex_XYZTEX
{
	Vertex_XYZTEX() {}
	Vertex_XYZTEX(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
};

struct Vertex_XYZTEXNORMAL
{
	Vertex_XYZTEXNORMAL() {}
	Vertex_XYZTEXNORMAL(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		:pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};

class Primitives : protected Graphics
{
public:

	Primitives();
	~Primitives();

	//void Init(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon);

	//Primitive generation functions
	void GenerateTriangleGeometry(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D10Blob* VS_Buffer);
	void GenerateCubeGeometry(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D10Blob* VS_Buffer);
	void GenerateTexturedCubeGeometry(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D10Blob* VS_Buffer, LPCWSTR textureFile);

	void StreamGeometry(ID3D11DeviceContext* d3d11DevCon);
	void StreamIndexedGeometry(ID3D11DeviceContext* d3d11DevCon, int indexCount, int startIndexLocation, int baseVertexLocation);
	void StreamIndexedTexturedGeometry(ID3D11DeviceContext* d3d11DevCon, int indexCount, int startIndexLocation, int baseVertexLocation);

	void CleanUp();

private:

	ID3D11InputLayout* vertLayout;

	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertBuffer;

	ID3D11ShaderResourceView* texture;
	ID3D11SamplerState* textureSamplerState;
};

#endif