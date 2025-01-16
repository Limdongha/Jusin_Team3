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
#pragma region ���콺 ��ġ ������Ʈ

	POINT       ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	m_tPos.x = (long)ptMouse.x;
	m_tPos.y = (long)ptMouse.y;

	ShowCursor(false);

#pragma endregion

}

//���콺�� �ش� ������Ʈ �Ÿ����
float CMouseMgr::CulMouseDistance(POINT _mx, tVec2 _px)
{
	return CalDia((float)_mx.x, (float)_px.fX, (float)_mx.y, (float)_px.fY);
}

void CMouseMgr::Render() 
{
	//���Թ�
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


//���� �̻�� �Լ���
void CMouseMgr::Initialize() {}
void CMouseMgr::Release() {}