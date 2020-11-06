#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Init(int width, int height)
{
	camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//Set the Projection matrix
	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, width / height, 1.0f, 1000.0f);
}

XMMATRIX Camera::UpdateWVP()
{
	World = XMMatrixIdentity();

	WVP = World * camView * camProjection;

	return WVP;
}