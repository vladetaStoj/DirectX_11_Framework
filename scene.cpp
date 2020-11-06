#include "scene.h"

using namespace std;

Scene::Scene()
{
	m_Graphics = 0;
	m_Primitive_0 = 0;
	m_Primitive_1 = 0;
	m_Primitive_2 = 0;
	m_Camera = 0;
	m_Shader_0 = 0;

	rot = 0.01f;
}

Scene::~Scene()
{

}

void Scene::InitScene(int width, int height, HWND hwnd)
{
	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new Graphics;

	// Initialize the graphics object.
	m_Graphics->Init(width, height, hwnd);

	if (!m_Graphics)
	{
		cout << "Graphics device initialization error!" << endl;
	}

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_Graphics->getWindowWidth();
	viewport.Height = m_Graphics->getWindowHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_Graphics->setViewpoint(viewport);

	m_Camera = new Camera;
	m_Camera->Init(m_Graphics->getWindowWidth(), m_Graphics->getWindowHeight());

	//Note: Current implementations requires that the shaders need to be set specifically for each primitive instance prior to rendering.
	//Additionally, the D3D11_INPUT_ELEMENT_DESC layout[] for each primitive type MUST match the vertex shader input layouts for the selected shader.
	
	//Initialize font system
	m_Graphics->InitD2DScreenTexture();

	m_Graphics->SetupLighting();

	//Main object shader
	m_Shader_0 = new Shader;
	m_Shader_0->CreateShader(m_Graphics->getDevice(), m_Graphics->getDevCon(), L"Effects_Final.fx");

	//Prepare scene primitives
	m_Primitive_0 = new Primitives;
	m_Primitive_0->GenerateTexturedCubeGeometry(m_Graphics->getDevice(), m_Graphics->getDevCon(), m_Shader_0->getVSBuffer(), L"face.jpg");

	m_Primitive_1 = new Primitives;
	m_Primitive_1->GenerateTexturedCubeGeometry(m_Graphics->getDevice(), m_Graphics->getDevCon(), m_Shader_0->getVSBuffer(), L"glass.jpg");

	//Clipped cube primitive
	m_Primitive_2 = new Primitives;
	m_Primitive_2->GenerateTexturedCubeGeometry(m_Graphics->getDevice(), m_Graphics->getDevCon(), m_Shader_0->getVSBuffer(), L"clip_tex.png");

	//Setup blending/clipping/culling states
	m_Graphics->SetupAuxRenderStates();


}

void Scene::UpdateScene(double time)
{
	rot += 1.0f * time;

	if (rot > 6.28f)
	{
		rot = 0.0f;
	}

	//Reset cube1World
	cube1World = XMMatrixIdentity();

	//Define cube1's world space matrix
	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, rot);
	Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	//Set cube1's world space using the transformations
	cube1World = Translation * Rotation;

	//Reset cube2World
	cube2World = XMMatrixIdentity();

	//Define cube2's world space matrix
	Rotation = XMMatrixRotationAxis(rotaxis, -rot);
	Scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	//Set cube2's world space matrix
	cube2World = Rotation * Scale;

	cube3World = XMMatrixIdentity();

	Translation = XMMatrixTranslation(5.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotaxis, -rot);
	Scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	cube3World = Translation * Rotation * Scale;
}

void Scene::RenderScene(int fps)
{
	m_Graphics->FrameBegin(0.5, 0.5, 0.5, 1.0, true);

	//Render text 
	//m_Graphics->RenderText(m_Shader_0->getTextPS(), L"FPS, MFPS: ", fps, 0, 0, m_Graphics->getWindowWidth(), m_Graphics->getWindowHeight());

	//Update camera and draw first cube
	XMMATRIX temp_WVP = cube1World * m_Camera->getCamView() * m_Camera->getCamProjection();

	m_Graphics->UpdateCB(temp_WVP, cube1World);

	m_Graphics->UpdateLighting();

	m_Shader_0->SetShader(m_Graphics->getDevCon());

	//Opaque objects get rendered here
	m_Graphics->FrameRenderAllOpaque();
	m_Primitive_0->StreamIndexedTexturedGeometry(m_Graphics->getDevCon(), 36, 0, 0);

	//Draw clipped texture box
	temp_WVP = cube3World * m_Camera->getCamView() * m_Camera->getCamProjection();
	m_Graphics->UpdateCB(temp_WVP, cube3World);

	m_Primitive_2->StreamIndexedTexturedGeometry(m_Graphics->getDevCon(), 36, 0, 0);

	//Blended objects get rendered here - note that blending is expensive, since objects needs to be rendered twice with different culling configurations
	m_Graphics->FrameRenderAllBlending(0.5, 0.5, 0.5, 1.0);

	//Update camera and draw second cube transperent
	temp_WVP = cube2World * m_Camera->getCamView() * m_Camera->getCamProjection();

	//m_Graphics->UpdateLighting(0);

	m_Graphics->UpdateCB(temp_WVP, cube2World);

	m_Graphics->SetCCWCulling();

	m_Primitive_1->StreamIndexedTexturedGeometry(m_Graphics->getDevCon(), 36, 0, 0);

	m_Graphics->SetCWCulling();

	m_Primitive_1->StreamIndexedTexturedGeometry(m_Graphics->getDevCon(), 36, 0, 0);

	m_Graphics->FrameEnd();
}

void Scene::ShutdownScene()
{
	// Release the graphics object.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Primitive_0)
	{
		m_Primitive_0->CleanUp();
		delete m_Primitive_0;
		m_Primitive_0 = 0;
	}

	if (m_Primitive_1)
	{
		m_Primitive_1->CleanUp();
		delete m_Primitive_1;
		m_Primitive_1 = 0;
	}

	if (m_Primitive_2)
	{
		m_Primitive_2->CleanUp();
		delete m_Primitive_2;
		m_Primitive_2 = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Shader_0)
	{
		m_Shader_0->CleanUp();
		delete m_Shader_0;
		m_Shader_0 = 0;
	}
}