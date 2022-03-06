#include "stdafx.h"
#include "ToyMain.h"
#include "MyCamera.h"
#include "MyUtil.h"

ToyMain::ToyMain()
{
}


ToyMain::~ToyMain()
{
}

void ToyMain::Setup(HWND hWnd)
{
	pCam = new MyCamera();

	RECT winRect;
	GetClientRect(hWnd, &winRect);

	int width = winRect.right - winRect.left;
	int height = winRect.bottom - winRect.top;

	// Camera ¼¼ÆÃ 
	pCam->Setup(20, width, height, 1, 100);
}

void ToyMain::Release()
{
	if (pCam)
	{
		delete pCam;
		pCam = NULL;
	}
}