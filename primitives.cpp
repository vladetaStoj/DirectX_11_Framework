#include "primitives.h"

Primitives::Primitives()
{

}

Primitives::~Primitives()
{

}

void Primitives::GenerateTriangleGeometry(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D10Blob* VS_Buffer)
{
	//Create the vertex buffer
	Vertex_XYZRGB v[] =
	{
		Vertex_XYZRGB(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex_XYZRGB(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex_XYZRGB(0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex_XYZRGB(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
	};

	DWORD indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex_XYZRGB) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex_XYZRGB);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);

	//Create the Input Layout - this should be modularized...or include complete setup for COLOR, UV and NORMAL
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	d3d11DevCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Primitives::GenerateCubeGeometry(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D10Blob* VS_Buffer)
{
	Vertex_XYZRGB v[] =
	{
		Vertex_XYZRGB(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex_XYZRGB(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex_XYZRGB(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex_XYZRGB(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
		Vertex_XYZRGB(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex_XYZRGB(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
		Vertex_XYZRGB(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex_XYZRGB(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	};

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex_XYZRGB) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex_XYZRGB);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);

	//Create the Input Layout - this should be modularized...or include complete setup for COLOR, UV and NORMAL
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	d3d11DevCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Primitives::GenerateTexturedCubeGeometry(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, ID3D10Blob* VS_Buffer, LPCWSTR textureFile)
{
	Vertex_XYZTEXNORMAL v[] =
	{
		// Front Face
		Vertex_XYZTEXNORMAL(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		Vertex_XYZTEXNORMAL(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex_XYZTEXNORMAL(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex_XYZTEXNORMAL(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex_XYZTEXNORMAL(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		Vertex_XYZTEXNORMAL(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex_XYZTEXNORMAL(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex_XYZTEXNORMAL(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		Vertex_XYZTEXNORMAL(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex_XYZTEXNORMAL(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		Vertex_XYZTEXNORMAL(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex_XYZTEXNORMAL(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex_XYZTEXNORMAL(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		Vertex_XYZTEXNORMAL(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex_XYZTEXNORMAL(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex_XYZTEXNORMAL(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	d3d11DevCon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex_XYZTEXNORMAL) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex_XYZTEXNORMAL);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	d3d11DevCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Load and assign texture
	D3DX11CreateShaderResourceViewFromFile(d3d11Device, textureFile, NULL, NULL, &texture, NULL);

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //Experiment with no or point filtering for old-skool look
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	d3d11Device->CreateSamplerState(&sampDesc, &textureSamplerState);
}

void Primitives::StreamGeometry(ID3D11DeviceContext* d3d11DevCon)
{
	d3d11DevCon->Draw(3, 0);
}

void Primitives::StreamIndexedGeometry(ID3D11DeviceContext* d3d11DevCon, int indexCount, int startIndexLocation, int baseVertexLocation)
{
	d3d11DevCon->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Primitives::StreamIndexedTexturedGeometry(ID3D11DeviceContext* d3d11DevCon, int indexCount, int startIndexLocation, int baseVertexLocation)
{
	d3d11DevCon->PSSetShaderResources(0, 1, &texture);
	d3d11DevCon->PSSetSamplers(0, 1, &textureSamplerState);

	d3d11DevCon->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Primitives::CleanUp()
{
	vertBuffer->Release();
	indexBuffer->Release();
	vertLayout->Release();
}