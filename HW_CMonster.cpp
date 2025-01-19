#include "pch.h"
#include "HW_CMonster.h"
#include "SceneMgr.h"
#include "HW_CPlayer.h"
#include "ScrollMgr.h"


HW_CMonster::HW_CMonster() : m_fSpeed(0.f), m_fMonsterRadius(0.f), m_fTopY(0.f), m_fBottomY(0.f)
{ 
}

HW_CMonster::~HW_CMonster()
{
	Release();
}

void HW_CMonster::Initialize()
{
	m_tInfo.vPos = { 300.f, 420.f, 0.f };
	m_tInfo.vLook = {1.f, 0.f, 0.f};
	m_fSpeed = 2.f;
	m_fMonsterRadius = 20.f; // 몬스터 반지름 고정

	m_eRender = RENDERID::RENDER_MONSTER;	
}


void HW_CMonster::Update()
{
	// 플레이어 이동
	m_tInfo.vDir = { -1.f, 0.f, 0.f };
	//m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	
	m_fTopY = m_tInfo.vPos.y - m_fMonsterRadius;
	m_fBottomY = m_tInfo.vPos.y + m_fMonsterRadius;


	#pragma region 추적
	//GETPLAYER
	//m_tInfo.vDir = Player.GetInfo().vPos - m_tInfo.vPos;
	//
	//D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	//D3DXVec3Normalize(&m_tInfo.vLook, &m_tInfo.vLook);

	//float fDot = D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook);
	//float fAngle = acosf(fDot);

	//if (m_tInfo.vPos.y < Player.GetInfo().vPos.y)
	//{
	//	fAngle = 2.f * D3DX_PI - fAngle;
	//}

	//m_tInfo.vPos.x += cosf(fAngle) * m_fSpeed;
	//m_tInfo.vPos.y -= sinf(fAngle) * m_fSpeed;
	#pragma endregion
}

void HW_CMonster::LateUpdate()
{
}

void HW_CMonster::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	

	if (HW_CPlayer::m_bColliderBox)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(g_memDC, hPen);
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(g_memDC, hBrush);

		Rectangle(
			g_memDC,
			int(m_tInfo.vPos.x - m_fMonsterRadius) + +iScrollX,
			int(m_tInfo.vPos.y + m_fMonsterRadius),
			int(m_tInfo.vPos.x + m_fMonsterRadius) + iScrollX,
			int(m_tInfo.vPos.y - m_fMonsterRadius)
		);
		// 이전 펜과 브러시 복원
		SelectObject(g_memDC, hOldPen);
		SelectObject(g_memDC, hOldBrush);

		// 생성한 펜 삭제
		DeleteObject(hPen);
	}

	Ellipse(g_memDC,
		int(m_tInfo.vPos.x - m_fMonsterRadius) + +iScrollX,
		int(m_tInfo.vPos.y + m_fMonsterRadius),
		int(m_tInfo.vPos.x + m_fMonsterRadius) + iScrollX,
		int(m_tInfo.vPos.y - m_fMonsterRadius)
		);

	SetBkMode(g_memDC, TRANSPARENT); // 배경을 투명하게 설정
	SetTextColor(g_memDC, RGB(255, 0, 140)); // 텍스트 색상

	wchar_t scoreText01[30];
	swprintf(scoreText01, 30, L"몬스터 반지름: %.2f", m_fMonsterRadius);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		160,
		scoreText01,
		int(wcslen(scoreText01)));

	wchar_t scoreText02[30];
	swprintf(scoreText02, 30, L"몬스터 Y 상: %.2f", m_tInfo.vPos.y - m_fMonsterRadius);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		180,
		scoreText02,
		int(wcslen(scoreText02)));


	wchar_t scoreText03[30];
	swprintf(scoreText03, 30, L"몬스터 Y 하: %.2f", m_tInfo.vPos.y + m_fMonsterRadius);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		200,
		scoreText03,
		int(wcslen(scoreText03)));


}

void HW_CMonster::Release()
{
}
