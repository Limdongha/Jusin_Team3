#pragma once

#ifndef PCH_H
#define PCH_H

// ���⿡ �̸� �������Ϸ��� ��� �߰�
#define NOMINMAX

//���̷�Ʈ 9 ����
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
// Debug �� ��
#pragma comment(lib, "fmodL_vc.lib")
// Release �� ��
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


// GDI+ �ڵ忡�� ��ũ�� ��Ȱ��ȭ
#ifdef _DEBUG
#undef new
#endif
#endif //PCH_H







