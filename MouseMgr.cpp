#include "pch.h"
#include "Define.h"
#include "MouseMgr.h"
#include "SceneMgr.h"
#include "Calculation.h"
#include "MouseMgr.h"
#include "ResMgr.h"
#include "Textures.h"

CMouseMgr::CMouseMgr() : m_tPos(POINT{})
{
	m_pMouseTex = CResMgr::GetInst()->LoadTexture
	(L"Mouse", L"./\\Content\\Textures\\Cursor\\Mouse_Cursor.bmp");
}
CMouseMgr::~CMouseMgr()
{
	Release();
}

void CMouseMgr::Update()
{
#pragma region 마우스 위치 업데이트

	POINT       ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	m_tPos.x = (long)ptMouse.x;
	m_tPos.y = (long)ptMouse.y;

	ShowCursor(false);

#pragma endregion

}

//마우스와 해당 오브젝트 거리계산
float CMouseMgr::CulMouseDistance(POINT _mx, tVec2 _px)
{
	return CalDia((float)_mx.x, (float)_px.fX, (float)_mx.y, (float)_px.fY);
}

void CMouseMgr::Render() 
{
	//출입문
	GdiTransparentBlt(
		g_memDC,
		m_tPos.x,
		m_tPos.y,
		32, 32,
		m_pMouseTex->GetDC(),
		0,
		0,
		42, 42,
		RGB(255, 0, 255)
	);
}


//아직 미사용 함수들
void CMouseMgr::Initialize() {}
void CMouseMgr::Release() {}