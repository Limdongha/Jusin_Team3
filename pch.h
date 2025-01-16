#pragma once

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#define NOMINMAX

//다이렉트 9 포함
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <map>
#include <list>
#include "framework.h"
#include "ole2.h"
#include <chrono>
#include <ctime>
#include "resource.h"
#include "Func.h"
#include <future>
#include <atomic>
#include <time.h>
#include <shlwapi.h>
#include <typeinfo>
#include <commdlg.h>
#include<unordered_set>
#include<unordered_map>
#include <Vfw.h>




#pragma comment(lib, "vfw32.lib")
#pragma comment (lib, "Msimg32.lib")

#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
using namespace std::chrono;
using namespace std;

//fmod
#include <io.h>
#include "fmod.h"
#include "fmod.hpp"
//#pragma comment(lib, "fmodex_vc.lib")
// Debug 일 때
#pragma comment(lib, "fmodL_vc.lib")
// Release 일 때
#pragma comment(lib, "fmod_vc.lib")


extern GdiplusStartupInput g_GdiplusStartupInput;
extern ULONG_PTR g_GdiplusToken;



#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif
#endif


// GDI+ 코드에서 매크로 비활성화
#ifdef _DEBUG
#undef new
#endif
#endif //PCH_H







