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

//���� DC ����
HDC			g_memDC;


CMainGame::CMainGame() : m_dwTime(0), m_iFPS(0), m_wcBuff(L""), m_hBit(0), m_iEventBox(0)
{
}

CMainGame::~CMainGame()
{
	CMainGame::Release();
}

//FPS, ��� �� ������Ʈ���� �Ѹ���
void CMainGame::Progress(HDC _hDC)
{
#pragma region FPS ���

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

	//�̺�Ʈ ó��
	CEventMgr::GetInst()->Update();
}

//������Ʈ �Ѹ���
void CMainGame::Update()
{
	//���콺 ������Ʈ
	CMouseMgr::GetInst()->Update();
	//���Ŵ��� ������Ʈ
	CSceneMgr::GetInst()->Update();
	//�ݶ��̴� ������Ʈ
	CColliderMgr::GetInst()->Update();
	//UI ������Ʈ
	CUIMgr::GetInst()->Update();
	//����Ʈ ������Ʈ
	CEffectMgr::GetInst()->Update();
	//ȭ������� ������Ʈ
	CScrollMgr::Get_Instance()->Update();
}

void CMainGame::LateUpdate()
{
	//Ű�Ŵ��� ������Ʈ
	Ckeymgr::GetInst()->Update();
	//���Ŵ��� ����Ʈ������Ʈ
	CSceneMgr::GetInst()->LateUpdate();

	//����Ʈ �Ŵ���
	CEffectMgr::GetInst()->LateUpdate();
	
	//�̺�Ʈ�ڽ� ��, ����
	if (KEY_TAP(KEY::F1))
	{
		++m_iEventBox;
		if (2 <= m_iEventBox)
			m_iEventBox = 0;
	}
}

//�����Ѹ��� �� ������۸� , �ݸ���Ȯ��
void CMainGame::Render(HDC _hDC)
{
	CSceneMgr::GetInst()->Render();
	CEffectMgr::GetInst()->Render();

	CEventMgr::GetInst()->Render();

	CMouseMgr::GetInst()->Render();
	BitBlt(_hDC, 0, 0, WINCX, WINCY, g_memDC, 0, 0, SRCCOPY);

}

//�̴ϼȶ����� �� �̴ϼ� �Ѹ���
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


//Ŀ�ο�����Ʈ ����
void CMainGame::Release()
{
	ReleaseDC(g_hWnd, g_memDC);

	DeleteDC(g_memDC);
	DeleteObject(m_hBit);
	CScrollMgr::Destroy_Instance();
}

