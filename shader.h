#ifndef _SHADER_H_
#define _SHADER_H_

#include "graphics.h"

class Shader
{
public:

	Shader();
	~Shader();

	void CreateShader(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, LPCWSTR shaderFile);
	void SetShader(ID3D11DeviceContext* d3d11DevCon);
	void CleanUp();

	inline ID3D10Blob* getVSBuffer() { return VS_Buffer; }
	inline ID3D10Blob* getPSBuffer() { return PS_Buffer; }

	inline ID3D11PixelShader* getTextPS() { return D2D_PS; }

private:

	//Defual rendering
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	ID3D11PixelShader* D2D_PS;
	ID3D10Blob* D2D_PS_Buffer;
};

#endif