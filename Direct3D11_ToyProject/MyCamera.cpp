#include "stdafx.h"
#include "MyCamera.h"
#include <DirectXMath.h>
#include "MyUtil.h"

using namespace DirectX;

MyCamera::MyCamera()
{
}


MyCamera::~MyCamera()
{
}

// 카메라 초기 세팅 
void MyCamera::Setup(
	const float angle
	, const float clientWidth
	, const float clientHeight
	, const float nearPlane
	, const float farPlane)
{
	mPosition = XMFLOAT3(0, 0, -1.0f);
	mTarget = XMFLOAT3(0, 0, 0);

	XMFLOAT3 up = XMFLOAT3(0, 1, 0);

	XMVECTOR tempVecPos = MyUtil::Float3ToVec(&mPosition);
	XMVECTOR vecUp = MyUtil::Float3ToVec(&up);

	mUp = MyUtil::VecToFloat3(tempVecPos + vecUp);

	// View Matrix 세팅 
	XMStoreFloat4x4(
		&mView
		, XMMatrixLookAtLH(XMLoadFloat3(&mPosition)
			, XMLoadFloat3(&mTarget)
			, XMLoadFloat3(&up)));

	// Projection Matrix 세팅 
	mAngle = angle;
	mClientWidth = clientWidth;
	mClientHeight = clientHeight;
	mNearest = nearPlane;
	mFarthest = farPlane;

	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(angle, clientWidth / clientHeight, nearPlane, farPlane));
}