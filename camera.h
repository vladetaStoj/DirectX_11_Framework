#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "graphics.h"

class Camera
{
public:

	Camera();

	~Camera();

	void Init(int width, int height);

	XMMATRIX UpdateWVP();

	//Accessor functions
	inline XMMATRIX getWorld()			{ return  World; }
	inline XMMATRIX getCamView()		{ return  camView; }
	inline XMMATRIX getCamProjection()	{ return  camProjection; }

	inline XMVECTOR getCamPosition()	{ return  camPosition; }
	inline XMVECTOR getCamTarget()		{ return camTarget; }
	inline XMVECTOR getCamUp()			{ return  camUp; }

private:

	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
};

#endif