#pragma once

#include "resource.h"

// extern 이란?

//		=> 참고 : https://stackoverflow.com/questions/10422034/when-to-use-extern-in-c
// - 쉽게 말하면 서로 다른 File 들끼리 (module) Variable 에 접근애 사용할 수 있게
//		Global Variable 로 만들어 주는 keyword 

//	- 이 위치에서 최초 변수를 선언.
//			=> extern 으로 선언 
//	- 이 변수를 참조할 다른 File 들에서는 해당 변수의 '존재(exsistence)' 만을 Compiler 에게
//		알려주기 위해 Function 안이 아닌 외부에 해당 Variable 재선언후 사용 가능 (extern 없이, e.g. HWND g_hWnd)
//	- 원리는 Compiler 가 Compile 을 할때 해당 Variable 이 extern 이라면 선언까지만 확인 후, 
//		Linker 가 Link 를 할때 실제로 참조하는 곳을 Linking 함. 
extern HWND g_hWnd;