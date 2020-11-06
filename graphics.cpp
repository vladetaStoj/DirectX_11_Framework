#include "graphics.h"
#include "primitives.h"

using namespace std;

Graphics::Graphics()
{
	SwapChain = 0;
	d3d11Device = 0;
	d3d11DevCon = 0;
	renderTargetView = 0;
	depthStencilView = 0;
	depthStencilBuffer = 0;

	m_renderState = 0;

	Transparency = 0;
	CCWcullMode = 0;
	CWcullMode = 0;
	NoCullMode = 0;

	cbPerObjectBuffer = 0;

	Adapter = 0;
	d3d101Device = 0;
	keyedMutex11 = 0;
	keyedMutex10 = 0;
	D2DRenderTarget = 0;
	Brush = 0;
	BackBuffer11 = 0;
	sharedTex11 = 0;
	d2dVertBuffer = 0;
	d2dIndexBuffer = 0;
	d2dTexture = 0;
	DWriteFactory = 0;
	TextFormat = 0;
	FontTransparency = 0;
	textSamplerState = 0;

	cbPerFrameBuffer = 0;
}

Graphics::Graphics(const Graphics& other)
{

}

Graphics::~Graphics()
{

}

bool Graphics::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	m_width = screenWidth;
	m_height = screenHeight;

	//Describe our Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = m_width;
	bufferDesc.Height = m_height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //Appearently, the DXGI_FORMAT_B8G8R8A8_UNORM for text rendering, but using the default DXGI_FORMAT_R8G8B8A8_UNORM works just fine too...
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd; //handle to current window
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	IDXGIFactory1 *DXGIFactory;

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);

	DXGIFactory->EnumAdapters1(0, &Adapter);

	DXGIFactory->Release();

	//Create our Direct3D 11 Device and SwapChain//////////////////////////////////////////////////////////////////////////
	D3D11CreateDeviceAndSwapChain(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	//Initialize Direct2D, Direct3D 10.1, DirectWrite
	InitD2D_D3D101_DWrite();

	//Release the Adapter interface
	Adapter->Release();
	
	//Depth buffer description
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	//MVP buffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT; //IMPORTANT: Use D3D11_USAGE_DYNAMIC if something needs to be updated 
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0; // IMPORTANT: THis should also be set to D3D11_CPU_ACCESS_WRITE if any data is passed from the cpu...
	cbbd.MiscFlags = 0;

	d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);

	//Create our BackBuffer
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer11);

	//Create our Render Target
	d3d11Device->CreateRenderTargetView(BackBuffer11, NULL, &renderTargetView);
	//BackBuffer11->Release();

	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//Setup render states
	ZeroMemory(&m_wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	m_wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	m_wfdesc.CullMode = D3D11_CULL_NONE;
	d3d11Device->CreateRasterizerState(&m_wfdesc, &m_renderState);

	//Enable wireframe rendering
	d3d11DevCon->RSSetState(m_renderState);

	return true;
}

void Graphics::SetupLighting()
{
	//Setup scene lighting parameters 
	m_light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	m_light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_light.diffuse = XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
}

void Graphics::InitD2D_D3D101_DWrite()
{
	D3D10CreateDevice1(Adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT,
		D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &d3d101Device);

	//Create Shared Texture that Direct3D 10.1 will render on//////////////////////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC sharedTexDesc;

	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	sharedTexDesc.Width = m_width;
	sharedTexDesc.Height = m_height;
	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels = 1;
	sharedTexDesc.ArraySize = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	d3d11Device->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);

	// Get the keyed mutex for the shared texture (for D3D11)///////////////////////////////////////////////////////////////
	sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);

	// Get the shared handle needed to open the shared texture in D3D10.1///////////////////////////////////////////////////
	IDXGIResource *sharedResource10;
	HANDLE sharedHandle10;

	sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);

	sharedResource10->GetSharedHandle(&sharedHandle10);

	sharedResource10->Release();

	// Open the surface for the shared texture in D3D10.1///////////////////////////////////////////////////////////////////
	IDXGISurface1 *sharedSurface10;

	d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));

	sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);

	// Create D2D factory///////////////////////////////////////////////////////////////////////////////////////////////////
	ID2D1Factory *D2DFactory;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

	sharedSurface10->Release();
	D2DFactory->Release();

	// Create a solid color brush to draw something with        
	D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f), &Brush);

	//DirectWrite///////////////////////////////////////////////////////////////////////////////////////////////////////////
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DWriteFactory));

	DWriteFactory->CreateTextFormat(
		L"Script",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
		&TextFormat
	);

	TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Graphics::InitD2DScreenTexture()
{
	Vertex_XYZTEX v[] =
	{
		// Front Face
		Vertex_XYZTEX(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex_XYZTEX(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex_XYZTEX(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex_XYZTEX(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,
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
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex_XYZTEX) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);

	//Create A shader resource view from the texture D2D will render to,
	//So we can use it to texture a square which overlays our scene
	d3d11Device->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);

	//Set the d2d Index buffer
	d3d11DevCon->IASetIndexBuffer(d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Set the d2d vertex buffer
	UINT stride = sizeof(Vertex_XYZTEX);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &d2dVertBuffer, &stride, &offset);
}

void Graphics::RenderText(wstring text, float top, float left, float right, float bottom)
{
	//Release the D3D 11 Device
	keyedMutex11->ReleaseSync(0);

	//Use D3D10.1 device
	keyedMutex10->AcquireSync(0, 5);

	//Draw D2D content        
	D2DRenderTarget->BeginDraw();

	//Clear D2D Background
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	//Create our string
	wostringstream printString;
	printString << text;
	printText = printString.str();

	//Set the Font Color
	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	//Set the brush color D2D will use to draw with
	Brush->SetColor(FontColor);

	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, right, bottom);

	//Draw the Text
	D2DRenderTarget->DrawText(
		printText.c_str(),
		wcslen(printText.c_str()),
		TextFormat,
		layoutRect,
		Brush
	);

	D2DRenderTarget->EndDraw();

	//Release the D3D10.1 Device
	keyedMutex10->ReleaseSync(1);

	//Use the D3D11 Device
	keyedMutex11->AcquireSync(1, 5);

	//Set the blend state for D2D render target texture objects
	d3d11DevCon->OMSetBlendState(FontTransparency, NULL, 0xffffffff);

	XMMATRIX wvp = XMMatrixIdentity();
	cbPerObjAttributes.WVP = XMMatrixTranspose(wvp);
	cbPerObjAttributes.World = XMMatrixTranspose(wvp);

	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObjAttributes, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &d2dTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &textSamplerState);

	d3d11DevCon->DrawIndexed(6, 0, 0);
}

void Graphics::RenderText(ID3D11PixelShader* pixelShader, wstring text, int timerInfo, float top, float left, float right, float bottom)
{
	//Release the D3D 11 Device
	keyedMutex11->ReleaseSync(0);

	//Use D3D10.1 device
	keyedMutex10->AcquireSync(0, 5);

	//Draw D2D content        
	D2DRenderTarget->BeginDraw();

	//Clear D2D Background
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	//Create our string
	wostringstream printString;
	printString << text;

	wostringstream infoString;
	infoString << timerInfo;

	wostringstream infoString2;
	double mfps = 1.0 / timerInfo;
	infoString2 << mfps;

	printText = printString.str() + infoString.str() + L", " + infoString2.str();

	//Set the Font Color
	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	//Set the brush color D2D will use to draw with
	Brush->SetColor(FontColor);

	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, right, bottom);

	//Draw the Text
	D2DRenderTarget->DrawText(
		printText.c_str(),
		wcslen(printText.c_str()),
		TextFormat,
		layoutRect,
		Brush
	);

	D2DRenderTarget->EndDraw();

	//Release the D3D10.1 Device
	keyedMutex10->ReleaseSync(1);

	//Use the D3D11 Device
	keyedMutex11->AcquireSync(1, 5);

	//Set the blend state for D2D render target texture objects
	d3d11DevCon->OMSetBlendState(FontTransparency, NULL, 0xffffffff);

	d3d11DevCon->PSSetShader(pixelShader, 0, 0);

	XMMATRIX wvp = XMMatrixIdentity();
	cbPerObjAttributes.WVP = XMMatrixTranspose(wvp);
	cbPerObjAttributes.World = XMMatrixTranspose(wvp);

	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObjAttributes, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &d2dTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &textSamplerState);

	d3d11DevCon->DrawIndexed(6, 0, 0);
}

void Graphics::ChangeRenderState(int renderState)
{
	//Add additional render state changes as required (e.g., culling on/off)
	switch (renderState)
	{
	case 0:
		ZeroMemory(&m_wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		m_wfdesc.FillMode = D3D11_FILL_SOLID;
		m_wfdesc.CullMode = D3D11_CULL_NONE;
		d3d11Device->CreateRasterizerState(&m_wfdesc, &m_renderState);
		d3d11DevCon->RSSetState(m_renderState);

		//cout << "Render stated changed to: D3D11_FILL_SOLID" << endl;
		break;
	case 1:

		ZeroMemory(&m_wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		m_wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		m_wfdesc.CullMode = D3D11_CULL_NONE;
		d3d11Device->CreateRasterizerState(&m_wfdesc, &m_renderState);
		d3d11DevCon->RSSetState(m_renderState);

		//cout << "Render stated changed to: D3D11_FILL_WIREFRAME" << endl;
		break;
	default:
		//cout << "Invald render stated set" << endl;
		break;
	}
}

void Graphics::SetupAuxRenderStates()
{
	//First set blending on
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR; //D3D11_BLEND_BLEND_FACTOR is default, or D3D11_BLEND_INV_SRC_ALPHA if text needs to be rendered specifically (though also works fine with the former parameter...so not sure why the later is needed for text rendering)
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	d3d11Device->CreateBlendState(&blendDesc, &Transparency);

	//Texture for font rendering
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
	d3d11Device->CreateSamplerState(&sampDesc, &textSamplerState);

	//Then set back culling on
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);

	//No culling
	D3D11_RASTERIZER_DESC rastDescNoCull;
	ZeroMemory(&rastDescNoCull, sizeof(D3D11_RASTERIZER_DESC));
	rastDescNoCull.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	d3d11Device->CreateRasterizerState(&rastDescNoCull, &NoCullMode);
}

void Graphics::Shutdown()
{
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
	cbPerObjectBuffer->Release();
	m_renderState->Release();
	Transparency->Release();
	CCWcullMode->Release();
	CWcullMode->Release();
	NoCullMode->Release();

	Adapter->Release();
	d3d101Device->Release();
	keyedMutex11->Release();
	keyedMutex10->Release();
	D2DRenderTarget->Release();
	Brush->Release();
	BackBuffer11->Release();
	sharedTex11->Release();
	d2dVertBuffer->Release();
	d2dIndexBuffer->Release();
	d2dTexture->Release();
	DWriteFactory->Release();
	TextFormat->Release();
	FontTransparency->Release();
	textSamplerState->Release();

	cbPerFrameBuffer->Release();
}

void Graphics::UpdateCB(XMMATRIX wvp, XMMATRIX world)
{
	cbPerObjAttributes.WVP = XMMatrixTranspose(wvp);
	cbPerObjAttributes.World = XMMatrixTranspose(world);

	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObjAttributes, 0, 0);

	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
}

//Note: Blending operations only work if they are called called in-between FrameBegin() and FrameEnd() functions
bool Graphics::FrameRenderAllOpaque()
{
	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);

	return true;
}

bool Graphics::FrameRenderAllBlending(float r, float g, float b, float a)
{
	float blendFactor[] = { r, g, b, a };

	d3d11DevCon->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	return true;
}

bool Graphics::FrameBegin()
{
	RenderBegin();

	return true;
}

bool Graphics::FrameBegin(float r, float g, float b, float a, bool clear)
{
	RenderBegin(r, g, b, a, clear);

	return true;
}

bool Graphics::FrameEnd()
{
	RenderEnd();

	return true;
}

void Graphics::UpdateLighting()
{
	//Default directional lighting 
	constbuffPerFramePS.light = m_light;
	constbuffPerFramePS.useLight = 1;

	d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFramePS, 0, 0);
	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);

	//This has to be updated using the mapped resource method (D3D11_MAPPED_SUBRESOURCE)

	//D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Lock the constant buffer so it can be written to.
	//d3d11DevCon->Map(cbPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	//cbPerFrame* dataPtr = (cbPerFrame*)mappedResource.pData;

	//memcpy(dataPtr, constbuffPerFramePS, sizeof(cbPerFrame));

	// Unlock the constant buffer.
	//d3d11DevCon->Unmap(cbPerFrameBuffer, 0);
}

bool Graphics::RenderBegin()
{
	D3DXCOLOR bgColor(0.2f, 0.2f, 0.2f, 1.0f);

	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return true;
}

bool Graphics::RenderBegin(float r, float g, float b, float a, bool clear)
{
	D3DXCOLOR bgColor(r, g, b, a);

	if (clear)
	{
		d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
		d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	return true;
}

bool Graphics::RenderEnd()
{
	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);

	return true;
}