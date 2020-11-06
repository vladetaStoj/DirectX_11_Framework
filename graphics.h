#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

//This is mostly just used for font rendering
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <dwrite.h>

#include <iostream>
#include <sstream>

using namespace std;

//Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


struct cbPerObject
{
	XMMATRIX  WVP;
	XMMATRIX  World;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct cbPerFrame
{
	Light  light;
	int useLight;
};

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Init(int, int, HWND);
	void Shutdown();

	//Main rendering and animation control fuctions
	bool FrameBegin();
	bool FrameBegin(float r, float g, float b, float a, bool clear);
	bool FrameEnd();

	//Blending frames
	bool FrameRenderAllOpaque();
	bool FrameRenderAllBlending(float r, float g, float b, float a);

	inline ID3D11Device* getDevice() { return  d3d11Device; }
	inline ID3D11DeviceContext* getDevCon() { return  d3d11DevCon; }
	inline ID3D11RenderTargetView* getRenderTargetView() { return renderTargetView; }

	inline IDXGISwapChain* getSwapChain() { return SwapChain; }
	inline DXGI_SWAP_CHAIN_DESC getSwapChainDescription() { return swapChainDesc; }

	inline int getWindowWidth() { return m_width; }
	inline int getWindowHeight() { return m_height; }
	inline void setViewpoint(D3D11_VIEWPORT viewport) { d3d11DevCon->RSSetViewports(1, &viewport); }

	void UpdateCB(XMMATRIX wvp, XMMATRIX world);

	void SetupLighting();
	void UpdateLighting();

	//Aux rendering states/functions/etc
	void SetupAuxRenderStates();
	inline void SetCCWCulling() { d3d11DevCon->RSSetState(CCWcullMode); }
	inline void SetCWCulling() { d3d11DevCon->RSSetState(CWcullMode); }
	inline void SetNoCulling() { d3d11DevCon->RSSetState(NoCullMode); }

	//Render state change
	void ChangeRenderState(int renderState);

	//Font rendering
	void InitD2D_D3D101_DWrite();
	void InitD2DScreenTexture();
	void RenderText(wstring text, float top, float left, float right, float bottom);
	void RenderText(ID3D11PixelShader* pixelShader, wstring text, int timerInfo, float top, float left, float right, float bottom);

private:
	bool RenderBegin();
	bool RenderBegin(float r, float g, float b, float a, bool clear);
	bool RenderEnd();

private:
	IDXGISwapChain* SwapChain;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;

	D3D11_RASTERIZER_DESC m_wfdesc; // render state
	ID3D11RasterizerState* m_renderState; //toggle wireframe

	//blending states
	ID3D11BlendState* Transparency;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;
	ID3D11RasterizerState* NoCullMode;

	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11Buffer* cbPerFrameBuffer;
	cbPerObject cbPerObjAttributes;
	int m_width, m_height;

	//Lighting model
	Light m_light;
	cbPerFrame constbuffPerFramePS;

	//Font rendering specific variables
	IDXGIAdapter1 *Adapter;
	ID3D10Device1 *d3d101Device;
	IDXGIKeyedMutex *keyedMutex11;
	IDXGIKeyedMutex *keyedMutex10;
	ID2D1RenderTarget *D2DRenderTarget;
	ID2D1SolidColorBrush *Brush;
	ID3D11Texture2D *BackBuffer11;
	ID3D11Texture2D *sharedTex11;
	ID3D11Buffer *d2dVertBuffer;
	ID3D11Buffer *d2dIndexBuffer;
	ID3D11ShaderResourceView *d2dTexture;
	IDWriteFactory *DWriteFactory;
	IDWriteTextFormat *TextFormat;
	ID3D11BlendState* FontTransparency;
	ID3D11SamplerState* textSamplerState;

	wstring printText;
};

#endif
