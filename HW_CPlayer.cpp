#include "pch.h"
#include "HW_CPlayer.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "HW_CLineMgr.h"

HW_CPlayer::HW_CPlayer() 
	: m_fAngle(0.f),m_fSpeed(0.f), m_fJumpPower(0.f), m_fTime(0.f),
	  m_bJump(false)
{
}

HW_CPlayer::~HW_CPlayer()
{
	Release();
}


void HW_CPlayer::Initialize()
{
	
	m_tInfo.vPos = { 100.f, 400.f, 0.f };
	m_tInfo.vLook = { 0.f, -1.f, 0.f };


	m_vPoint[0] = { -30.f, -30.f, 0.f };
	m_vPoint[1] = { 30.f, -30.f, 0.f };
	m_vPoint[2] = { 30.f, 30.f, 0.f };
	m_vPoint[3] = { -30.f, 30.f, 0.f };

	m_fSpeed = 5.f;
	m_fJumpPower = 28.f;
	
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

	MoveToEx(g_memDC, (int)m_vTemp[0].x + iScrollX, (int)m_vTemp[0].y, nullptr);
	LineTo(g_memDC, m_vTemp[1].x + iScrollX, m_vTemp[1].y);
	LineTo(g_memDC, m_vTemp[2].x + iScrollX, m_vTemp[2].y);
	LineTo(g_memDC, m_vTemp[3].x + iScrollX, m_vTemp[3].y);
	LineTo(g_memDC, m_vTemp[0].x + iScrollX, m_vTemp[0].y);
}

void HW_CPlayer::Release()
{
}

void HW_CPlayer::Key_Input()
{
	if (KEY_HOLD(KEY::RIGHT))
	{
		m_tInfo.vDir = { 1.f, 0.f, 0.f };
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}

	if (KEY_HOLD(KEY::LEFT))
	{
		m_tInfo.vDir = { 1.f, 0.f, 0.f };
		m_tInfo.vPos -= m_tInfo.vDir * m_fSpeed;
	}


	if (KEY_HOLD(KEY::SPACE))
	{
		m_bJump = true;

	}
}

void HW_CPlayer::Jumping()
{

	float fY(0.f);

	bool bLineCol = HW_CLineMgr::Get_Instance()->Collision_Line(m_tInfo.vPos.x, &fY);

	if(m_bJump)
	{

		m_tInfo.vPos.y -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime);
		m_fTime += 0.2f;

		if (bLineCol && (fY < m_tInfo.vPos.y))
		{
			m_bJump = false;
			m_fTime = 0.f;
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
