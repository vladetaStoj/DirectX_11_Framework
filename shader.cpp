#include "shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::CreateShader(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, LPCWSTR shaderFile)
{
	//Compile Shaders from shader file
	D3DX11CompileFromFile(shaderFile, 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0); 
	D3DX11CompileFromFile(shaderFile, 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);

	//Text rendering
	D3DX11CompileFromFile(shaderFile, 0, 0, "D2D_PS", "ps_4_0", 0, 0, 0, &D2D_PS_Buffer, 0, 0);

	//Create the Shader Objects
	d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//Text rendering
	d3d11Device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
}

void Shader::SetShader(ID3D11DeviceContext* d3d11DevCon)
{
	//Set Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);
}

void Shader::CleanUp()
{
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	D2D_PS->Release();
	D2D_PS_Buffer->Release();
}