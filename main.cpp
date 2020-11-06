///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DiretX 11 Demo
// Written by Vladeta Stojanovic (vladeta_stojanovic@yahoo.co.uk)
// Version: 110620_01
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TO FIX:

// Figure out why a runtime exception error is being thrown if an extra parameter of "useLight" is not included in the perFrameCB?!!!!!!
// Figure out how and why the ambient color is somehow blue/purple?!!
// Figure out why lighting flag parameter is not having an effect
// Figure out why screen is black when attempting to render text

//FRAMEWORK:

//Part 1:
// - Blending (DONE)
// - Clipping (DONE)
// - Font rendering (DONE)
// - High resolution timer (DONE)
// - Stats - FPS, MFPS (DONE)
// - Simple directional lighting model (DONE)
// - Fullscreen switching
// - DirectInput support
// - FPS camera
// - Plane primitive (textured)
// - Skybox rendering
// - OBJ model loading and rendering

//Part 2:
// - Animated MS3D model loading and rendering
// - freeview fps / 3rd person camera switching 
// - Planar shadows for non-static models
// - Debug rendering: wireframe bounding boxes, lines, bounding spheres, bounding elipsoids, etc
// - Collision detection routines
// - Character/World collison - "sliding ellipsoid method" 
// - Camera world collision
// - AABBs, OOBBs, bounding spheres
// - Camera frustum OOBB / bounding elipsoid intersection
// - Basic scene graph scene management (http://archive.gamedev.net/archive/reference/programming/features/scenegraph/index.html)

//Part 3:
// - Simple particle system
// - Fog
// - Multi-shader management system
// - Music playback
// - Spatial 3D audio for sound effects

//Part 4: 
// - Refactor framework and make into "engine" - https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-static-library-cpp?view=vs-2019
// - Implement simple scripting system  
// - Implement unit tests / examples using the engine for all main features
// - Write documentation 

//Part 5 - GAME:
// - Implement a simple Zombie hunting game (kill as many zombies as possible in given time limit)
// - Basic game menu with game state transitions 
// - Basic game loop
// - Scripted game enetities 
// - Scene management system
// - Basic weapon projectile
// - Basic AI for zombie NPCs (FSM with waypoints)

#include "system.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	System* sys;
	bool result;
	
	// Create the system object.
	sys = new System(800, 600);

	if(!sys)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = sys->Init();
	sys->InitDebug();

	if(result)
	{
		sys->Run();
	}

	// Shutdown and release the system object.
	sys->Shutdown();
	delete sys;
	sys = 0;

	return 0;
}

//Console debug subsystem
int main()
{
	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}