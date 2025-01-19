#include "pch.h"
#include "HW_CPlayer.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "HW_CLineMgr.h"
#include "MainGame.h"
#include "ColliderMgr.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "HW_CMonster.h"

bool HW_CPlayer::m_bColliderBox = false;

HW_CPlayer::HW_CPlayer()
	: m_fAngle(0.f), m_fSpeed(0.f), m_fJumpPower(0.f),  fPlayerRadius(0.f), m_iJumpCount(0), m_bDoubleJump(false)
{
}

HW_CPlayer::~HW_CPlayer()
{
	Release();
}

void HW_CPlayer::Initialize()
{
	m_tInfo.vPos = { 100.f, 400.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };


	m_vPoint[0] = { -25.f, -25.f, 0.f };
	m_vPoint[1] = { 25.f, -25.f, 0.f };
	m_vPoint[2] = { 25.f, 25.f, 0.f };
	m_vPoint[3] = { -25.f, 25.f, 0.f };

	m_fSpeed = 4.f;
	m_fJumpPower = 28.f;
	
	fPlayerRadius = 25.f;

	m_eRender = RENDERID::RENDER_PLAYER;
}

void HW_CPlayer::Update()
{
	D3DXMATRIX matScale, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&m_vTemp[i], &m_vPoint[i], &m_tInfo.matWorld);
	}

	Key_Input();
}

void HW_CPlayer::LateUpdate()
{
	Jumping();
	OffSet();
}

void HW_CPlayer::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	
	if (m_bColliderBox)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(g_memDC, hPen);
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(g_memDC, hBrush);

		Rectangle(
			g_memDC,
			(int)m_vTemp[0].x + iScrollX,
			(int)m_vTemp[1].y,
			(int)m_vTemp[2].x + iScrollX,
			(int)m_vTemp[3].y
		);
		// 이전 펜과 브러시 복원
		SelectObject(g_memDC, hOldPen);
		SelectObject(g_memDC, hOldBrush);

		// 생성한 펜 삭제
		DeleteObject(hPen);
	}

	Ellipse(
		g_memDC,                                  // DC 핸들
		(int)m_vTemp[0].x + iScrollX,            // 왼쪽 상단 X 좌표
		(int)m_vTemp[0].y,                       // 왼쪽 상단 Y 좌표
		(int)m_vTemp[2].x + iScrollX,            // 오른쪽 하단 X 좌표
		(int)m_vTemp[2].y                        // 오른쪽 하단 Y 좌표
	);

	for (int i = 0; i < m_iJumpCount; ++i)
	{
		Rectangle(g_memDC,
			10 + (i * 20),
			10,
			30 + (i * 20),
			30);
	}
	
	SetBkMode(g_memDC, TRANSPARENT); // 배경을 투명하게 설정
	SetTextColor(g_memDC, RGB(255, 0, 140)); // 텍스트 색상

	wchar_t scoreText01[30];
	swprintf(scoreText01, 30, L"플레이어 반지름: %.2f", fPlayerRadius);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		80,
		scoreText01,
		int(wcslen(scoreText01)));


	wchar_t scoreText02[30];
	swprintf(scoreText02, 30, L"플레이어 vPos.y: %.2f", m_tInfo.vPos.y);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		100,
		scoreText02,
		int(wcslen(scoreText02)));

	wchar_t scoreText03[30];
	swprintf(scoreText03, 30, L"Y 콜라이더 상 : %.2f", m_vTemp[1].y);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		120,
		scoreText03,
		int(wcslen(scoreText03)));

	wchar_t scoreText04[30];
	swprintf(scoreText04, 30, L"Y 콜라이더 하 : %.2f", m_vTemp[3].y);
	TextOut(g_memDC,
		WINCX / 2 + 200,
		140,
		scoreText04,
		int(wcslen(scoreText04)));
}

void HW_CPlayer::Release()
{
}

void HW_CPlayer::Key_Input()
{
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);

	//m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;

	#pragma region 테스트중
	if (KEY_HOLD(KEY::RIGHT))
	{
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}

	if (KEY_HOLD(KEY::LEFT))
	{
		m_tInfo.vPos -= m_tInfo.vDir * m_fSpeed;
	}
	#pragma endregion

	if (KEY_TAP(KEY::SPACE))
	{
		if (!GetbJump())
		{
			SetbJump(true);
		}
		else
		{
			if (m_iJumpCount > 0)
			{
				
				if(m_bDoubleJump)
				{
					--m_iJumpCount;
					SetbJump(true);
					SetfTime(0.f);
					m_bDoubleJump = false;
				}



			}
		}
		
		

	}

	if (KEY_HOLD(KEY::F1))
	{
		m_bColliderBox = !m_bColliderBox;
	}
}

void HW_CPlayer::Jumping()
{
	float fY(0.f);

	bool bLineCol = HW_CLineMgr::Get_Instance()->Collision_Line(m_tInfo.vPos.x, &fY);

	if(GetbJump())
	{
		m_tInfo.vPos.y -= (m_fJumpPower * sinf(45.f) * GetfTime()) - (9.8f * GetfTime() * GetfTime());

		AddfTime(0.2f);

		if (bLineCol && (fY < m_tInfo.vPos.y))
		{
			SetbJump(false);
			SetfTime(0.f);
			m_tInfo.vPos.y = fY;
		}
	}

	else if (bLineCol)
	{
		m_tInfo.vPos.y = fY;
	}
}

void HW_CPlayer::OffSet()
{
	int		iOffSetminX = 100;
	int		iOffSetmaxX = 500;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffSetminX > m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Add_ScrollX(m_fSpeed);

	if (iOffSetmaxX < m_tInfo.vPos.x + iScrollX)
		CScrollMgr::Get_Instance()->Add_ScrollX(-m_fSpeed);
}
