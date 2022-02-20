// Direct3D11_ToyProject.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Direct3D11_ToyProject.h"
#include <assert.h>
#include <windows.h>
#include <d3d11.h>										// D3D interface
#include <dxgi.h>											// DirectX driver interface
#include <d3dcompiler.h>								// shader compiler

// Direct3D11 라이브러리 코드에서 참조하기
//		=> 프로젝트 속성에서 링크로 설정하는 방법도 있음. 
#pragma comment( lib, "user32" )					// link against the win32 library
#pragma comment( lib, "d3d11.lib" )				// direct3D library
#pragma comment( lib, "dxgi.lib" )					// directx graphics interface
#pragma comment( lib, "d3dcompiler.lib" )	// shader compiler

#define MAX_LOADSTRING 100

// ** 함수 전방선언 **//
void InitializeDirect3D();
void SetupShader();
HRESULT CompileShader(LPCWSTR shaderFileName, LPCSTR entryPoint, LPCSTR target, UINT flags, ID3DBlob** ppCode, ID3DBlob** ppErrorBlob);

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Direct3D Device 는 Direct3D 로 구동시키는 애플리케이션에서 최소 하나가 필요
// 가장 Driver 에 가까운 존재 , Direct3D 의 '본체' 
ID3D11Device* pDevice = NULL;
// Device Context 즉 문맥임. 그래서 보면 '현재 상태' 관련된 메서드들이 많음. 
// '현재 상태' 를 어떻게 설정하고 그 상태로 드로우를 하고 등.. 
ID3D11DeviceContext* pDeviceContext = NULL;
// SwapChain 은 모니터 화면에 보이는 FrontBuffer (사실은 Texture2D 임) 와 그 뒤에서 
// 다음 Frame 위해 드로잉이 되고 있는 BackBuffer 등 렌더링 관련 여러 Buffer 들의 그룹을 
// SwapChain 이라함 
IDXGISwapChain* pSwapChain = NULL;
// ??
ID3D11RenderTargetView* pRenderTargetView = NULL;

HWND g_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DIRECT3D11TOYPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT3D11TOYPROJECT));

	InitializeDirect3D();
	SetupShader();

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

	return (int)msg.wParam;
}

// Direct3D 기본 초기화 함수
void InitializeDirect3D()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// DXGI_FORMAT_B8G8R8A8_UNORM 는 Gamma Correction 적용 X
	//		=> Gamma Correction 적용을 위해서는 
	//			  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB 로 대체 가능
	//		=> Gamma Correction 이란 sRGB 색상 공간에서 본래의 색상보다 
	//			밝아진 상태로 저장된 이미지들을 원래로 돌리기 위해 특정한
	//			감마 공식을 색상들에 적용하는 것임 
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// [BackBuffer 개수] . 일반적으로 FrontBuffer 1 개 BackBuffer 1 개로 SwapChain 구성 
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = g_hWnd;
	swapChainDesc.Windowed = true;

	D3D_FEATURE_LEVEL feature_level;
	// Single Threaded
	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined (_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Direct3D Device 생성하고 SwapChain 까지 생성.
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		// Hardware => Software => Ref 순으로 속도 차이발생 
		//		=> Ref 는 거의 못사용할 정도로 느리다함 .
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		// TODO : FeatureLevel 자료 검색해서 추가할것 
		//		=> 정확하진 않지만 D3D9 에서는 Caps 어쩌거 있었던거같은데
		//			그게 너무 복잡하고 해서 이거로 대체됐다함 . 
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		&feature_level,
		&pDeviceContext
	);

	// 참고 - #include <assert.h> 포함해야함
	assert(hr == S_OK && pSwapChain && pDevice && pDeviceContext);

	// Here I have added extra debug output to the function flags 
	// when the program is built in debug mode. If all went well, 
	// you should be able to now compile and run it, without an assertion 
	// triggering. Otherwise check the parameter values carefully. 
	ID3D11Texture2D* framebuffer;
	// Direct3D 에서 드로잉 결과물 이미지들은 Render Targets 라고 불림 
	// SwapChain 에서 View Pointer 를 가져올 수가 있음.
	hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&framebuffer
	);

	assert(SUCCEEDED(hr));

	hr = pDevice->CreateRenderTargetView(framebuffer, 0, &pRenderTargetView);
	// COM 오브젝트들은 객체의 할당에 있어서 Reference Counting 을 사용하기 때문에 
	// 위에서 발생한 Ref 카운트 증가를 하나 깍아주기 위해 Release() 를 호출함. 
	framebuffer->Release();

	assert(SUCCEEDED(hr));
}

void SetupShader()
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined ( DEBUG ) || defined (  _DEBUG ) 
	flags |= D3DCOMPILE_DEBUG;
#endif 

	// Blob 은 Binary Large Object 를 의미
	ID3DBlob* pVsBlob = NULL;
	ID3DBlob* pPsBlob = NULL;
	ID3DBlob* errorBlob = NULL;

	// VertexShader, PixelShader 를 Compile 해서 blob(Big binary object) 형태로 캐싱
	//			=> D3DCompileFromFile 메서드가 수행 
	LPCWSTR shaderFileName = L"shaders.hlsl";

	// Vertex Shader 컴파일
	CompileShader(L"shaders.hlsl", "vs_main", "vs_5_0", flags, &pVsBlob, &errorBlob);

	// Pixel Shader 컴파일 
	CompileShader(L"shaders.hlsl", "ps_main", "ps_5_0", flags, &pPsBlob, &errorBlob);
}

HRESULT CompileShader(LPCWSTR shaderFileName, LPCSTR entryPoint, LPCSTR target, UINT flags, ID3DBlob** ppCode, ID3DBlob** ppErrorBlob)
{
	// Pixel Shader 컴파일 
	HRESULT hr = D3DCompileFromFile(
		shaderFileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, // 해당 Shader 파일 안에 정의해둔 Vertex / Pixel Shader 함수와 이름이 일치해야함 
		target,
		flags,
		0,
		ppCode,
		ppErrorBlob
	);

	// 컴파일 실패 처리
	if (FAILED(hr))
	{
		if (ppErrorBlob)
		{
			OutputDebugStringA((char*)(*ppErrorBlob)->GetBufferPointer());
			(*ppErrorBlob)->Release();
		}

		if (ppCode)
		{
			(*ppCode)->Release();
		}

		assert(false);
	}

	return hr;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT3D11TOYPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DIRECT3D11TOYPROJECT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
