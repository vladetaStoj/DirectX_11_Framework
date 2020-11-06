#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

//System includes
#include<windows.h>
#include<iostream>

#include "input.h"
#include "scene.h"
#include "auxillary.h"

class System
{

public:
	System();
	System(int, int);
	System(const System&);

	~System();

	bool Init();
	void Run();
	void Shutdown();

	void InitDebug();
	void Debug();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	bool Frame();
	void UpdateInput();
	void InitializeWindows();
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	int m_screenWidth, m_screenHeight;

	bool m_renderState;

	int frameCount;
	int fps;
	double frameTime;

	Input* m_Input;
	Scene* m_Scene;
	Auxillary* m_Timer;
};

//Static function prototyptes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Static globals
static System* ApplicationHandle = 0;

#endif
