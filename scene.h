#ifndef _SCENE_H_
#define _SCENE_H_

//System includes 
#include<iostream>

//Project includes
#include "graphics.h"
#include "primitives.h"
#include "camera.h"
#include "shader.h"

class Scene
{
public:
	Scene();
	~Scene();

	void InitScene(int width, int height, HWND hwnd);
	void UpdateScene(double time);
	void RenderScene(int fps);
	void ShutdownScene();

	//Debug
	inline void ChangeRenderState(int state) { m_Graphics->ChangeRenderState(state); }

private:

	Graphics* m_Graphics;
	Primitives* m_Primitive_0;
	Primitives* m_Primitive_1;
	Primitives* m_Primitive_2;
	Camera* m_Camera;
	Shader* m_Shader_0;

	//Test scene variables
	XMMATRIX cube1World;
	XMMATRIX cube2World;
	XMMATRIX cube3World;

	XMMATRIX Rotation;
	XMMATRIX Scale;
	XMMATRIX Translation;
	float rot;
};

#endif
