#include <DirectXMath.h>

using namespace DirectX;

/*
		Camera 구현은 다음 참고
			- https://www.gamedev.net/articles/programming/graphics/directx-11-c-game-camera-r2978/
*/
#pragma once
class MyCamera
{
public:
	MyCamera();
	~MyCamera();

	/*** Camera Define ***/
	XMFLOAT3 mPosition;
	XMFLOAT3 mTarget;
	XMFLOAT3 mUp;

	/*** Projection Define ***/
	float mAngle;
	float mClientWidth;
	float mClientHeight;
	float mNearest;
	float mFarthest;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	// XMFLOAT4X4 mOrtho;

	void Setup(
		const float angle
		, const float clientWidth
		, const float clientHeight
		, const float nearPlane
		, const float farPlane);
};

