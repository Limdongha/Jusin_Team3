#include "pch.h"
#include "MainGame.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "ColliderMgr.h"
#include "MouseMgr.h"
#include "ResMgr.h"
#include "EventMgr.h"
#include "Textures.h"
#include "ScrollMgr.h"
#include "EffectMgr.h"
#include "SoundMgr.h"
#include "UIMgr.h"

//전역 DC 선언
HDC			g_memDC;


CMainGame::CMainGame() : m_dwTime(0), m_iFPS(0), m_wcBuff(L""), m_hBit(0), m_iEventBox(0)
{
}

CMainGame::~CMainGame()
{
	CMainGame::Release();
}

//FPS, 배경 및 업데이트렌더 뿌리기
void CMainGame::Progress(HDC _hDC)
{
#pragma region FPS 출력

	++m_iFPS;
	if (m_dwTime + 2000 < GetTickCount64())
	{
		swprintf_s(m_wcBuff, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_wcBuff);

		m_iFPS = 0;
		m_dwTime = GetTickCount64();
	}

#pragma endregion

	CMainGame::Update();
	CMainGame::LateUpdate();
	CMainGame::Render(_hDC);

	//이벤트 처리
	CEventMgr::GetInst()->Update();
}

//업데이트 뿌리기
void CMainGame::Update()
{
	//마우스 업데이트
	CMouseMgr::GetInst()->Update();
	//씬매니저 업데이트
	CSceneMgr::GetInst()->Update();
	//콜라이더 업데이트
	CColliderMgr::GetInst()->Update();
	//UI 업데이트
	CUIMgr::GetInst()->Update();
	//이펙트 업데이트
	CEffectMgr::GetInst()->Update();
	//화면흔들기용 업데이트
	CScrollMgr::Get_Instance()->Update();
}

void CMainGame::LateUpdate()
{
	//키매니저 업데이트
	Ckeymgr::GetInst()->Update();
	//씬매니저 레이트업데이트
	CSceneMgr::GetInst()->LateUpdate();

	//이펙트 매니저
	CEffectMgr::GetInst()->LateUpdate();
	
	//이벤트박스 온, 오프
	if (KEY_TAP(KEY::F1))
	{
		++m_iEventBox;
		if (2 <= m_iEventBox)
			m_iEventBox = 0;
	}
}

//렌더뿌리기 및 더블버퍼링 , 콜리전확인
void CMainGame::Render(HDC _hDC)
{
	CSceneMgr::GetInst()->Render();
	CEffectMgr::GetInst()->Render();

	CEventMgr::GetInst()->Render();

	CMouseMgr::GetInst()->Render();
	BitBlt(_hDC, 0, 0, WINCX, WINCY, g_memDC, 0, 0, SRCCOPY);

}

//이니셜라이즈 및 이니셜 뿌리기
void CMainGame::Initialize(HDC _hDC)
{
	m_hBit = CreateCompatibleBitmap(_hDC, WINCX, WINCY);
	g_memDC = CreateCompatibleDC(_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(g_memDC, m_hBit);
	DeleteObject(hOldBit);

	CResMgr::GetInst()->Initialize();
	CSceneMgr::GetInst()->Initialize();
	Ckeymgr::GetInst()->Initialize();
	CSoundMgr::GetInst()->Initialize();
	CUIMgr::GetInst()->Initialize();
}


//커널오브젝트 종료
void CMainGame::Release()
{
	ReleaseDC(g_hWnd, g_memDC);

	DeleteDC(g_memDC);
	DeleteObject(m_hBit);
	CScrollMgr::Destroy_Instance();
}

