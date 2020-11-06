#include "system.h"
#include <iostream>

using namespace std;

System::System()
{
	m_Input = 0;
	m_Scene = 0;
	m_Timer = 0;
	m_screenHeight = 600;
	m_screenWidth = 800;
	m_renderState = false;

	frameCount = 0;
	fps = 0;
	frameTime = 0.0;
}

System::System(int screenWidth, int screenHeight)
{
	m_Input = 0;
	m_Scene = 0;
	m_Timer = 0;
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;
	m_renderState = false;

	frameCount = 0;
	fps = 0;
	frameTime = 0.0;
}

System::System(const System& other)
{
	m_Input = 0;
	m_Scene = 0;
	m_Timer = 0;
	m_renderState = false;

	frameCount = 0;
	fps = 0;
	frameTime = 0.0;
}

System::~System()
{

}

bool System::Init()
{
	bool result;

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new Input;

	if (!m_Input)
	{
		cout << "Input device initialization error" << endl;
	}

	// Initialize the input object.
	m_Input->Initialize();

	// Initialize the windows api.
	InitializeWindows();

	//Initialize scene
	m_Scene = new Scene;

	m_Scene->InitScene(m_screenWidth, m_screenHeight, m_hwnd);

	m_Timer = new Auxillary;

	return true;
}

void System::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();

			if (!result)
			{
				done = true;
			}
		}

	}
}

void System::Shutdown()
{	
	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	if (m_Scene)
	{
		m_Scene->ShutdownScene();
		delete m_Scene;
		m_Scene = 0;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
}

void System::InitDebug()
{
	cout << "Debug test" << endl;

	//Print any startup diagnostics here 
}

void System::Debug()
{
	//Print any debug info during runtime of application
	cout << "Debug output :" << endl;
}

LRESULT System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		//DEBUG

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void System::UpdateInput()
{
	//Debug
	if (m_Input->IsKeyDown(VK_TAB))
	{
		m_renderState = !m_renderState;
	}
}

bool System::Frame()
{
	bool result;

	// Check if the user pressed escape and wants to exit the application.
	
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//Update additional input
	UpdateInput();

	//Debug
	m_Scene->ChangeRenderState(m_renderState);

	//Perform any logic updates here
	frameCount++;

	if (m_Timer->GetTime() > 1.0f)
	{
		fps = frameCount;
		frameCount = 0;
		m_Timer->StartTimer();
	}

	frameTime = m_Timer->GetFrameTime();

	//m_Scene->UpdateScene();
	
	m_Scene->UpdateScene(frameTime); //With high resultion timer

	//Render scene here
	//m_Scene->RenderScene();

	m_Scene->RenderScene(fps); //With stats

	return true;
}

void System::InitializeWindows()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"DX11 Framework";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_screenWidth, m_screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}