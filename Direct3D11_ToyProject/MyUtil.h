#include <string>
#include <DirectXMath.h>

// DirectXMath 사용위함 
using namespace DirectX;
// string 및 기타 사용 위함
using namespace std;

#pragma once
/* 테스트 프로젝트에 필요한 관련 유틸성 함수 싹다 여기로 집합 */
class MyUtil
{
public:
	MyUtil();
	~MyUtil();

	/******** DirectXMath 관련 ********/
	/*		DirectXMath 문서
				- https://tainaandme.tistory.com/entry/DirectXMath%EC%9D%98-%EA%B8%B0%EB%B3%B8-%ED%95%A8%EC%88%98
				- https ://docs.microsoft.com/ko-kr/windows/win32/dxmath/directxmath-portal
	*/

	// Float3 -> Vector
	//		=> inline 은 C++ 의 ODR (One Definition Rule) 에 따라 Header 에 선언 권장 
	//				=> https://en.cppreference.com/w/cpp/language/definition
	static inline XMVECTOR Float3ToVec(XMFLOAT3* val)
	{
		return XMLoadFloat3(val);
	}

	// Vector -> Float3
	static inline XMFLOAT3 VecToFloat3(XMVECTOR vec)
	{
		XMFLOAT3 val;
		XMStoreFloat3(&val, vec);
		return val;
	}
	/*******************************************************/

	/**** String 관련 *****/
	static inline void Print(string str)
	{
		OutputDebugStringA((str.c_str()));
	}

	static inline void Print(float value)
	{
		string str = to_string(value);
		Print(str);
	}

	static inline void Print(int value)
	{
		string str = to_string(value);
		Print(str);
	}

	/*******************************************************/
};

