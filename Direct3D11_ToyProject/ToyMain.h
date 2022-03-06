#include "MyCamera.h"

#pragma once
class ToyMain
{
public:
	ToyMain();
	~ToyMain();

	MyCamera* pCam;

	void Setup(HWND hWnd);
	void Release();
};

