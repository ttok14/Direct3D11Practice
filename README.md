History

- Windows Application 프로젝트 생성
- Direct3D11 라이브러리 참조 (소스코드)
	#include <windows.h>
	#include <d3d11.h>	// D3D interface
	#include <dxgi.h>	// DirectX 	driver interface
	#include <d3dcompiler.h>	// shader compiler

	#pragma comment( lib, "user32" )	// link against the win32 library
	#pragma comment( lib, "d3d11.lib" )	// direct3D library
	#pragma comment( lib, "dxgi.lib" )	// directx graphics interface
	#pragma comment( lib, "d3dcompiler.lib" )	// shader compiler
- GetMessage() => PeekMessage() 로 전환 ( GetMessage() 는 Msg 반환할때까지 Blocking 이므로
	게임에서는 NonBlocking 으로 함

	MSG msg = {};

	bool shouldClose = false;

	while (!shouldClose)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		/*** TODO: 이 위치에서 Direct3D 로 Rendering 수행 ***/
	}

- LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 함수에
	switch 문에 case WM_PAINT: 제거
	- Direct3D API 를 통해 직접 Drawing 을 할 것이기 때문임 
	