#include "pch.h"
#include "DH_CPlayer.h"
#include "KeyMgr.h"

DH_CPlayer::DH_CPlayer()
{
	ZeroMemory(&m_vPoint, sizeof(LPD3DXVECTOR3));
	ZeroMemory(&m_vOriginPoint, sizeof(LPD3DXVECTOR3));
	ZeroMemory(&m_vPlayerInfo, sizeof(INFO));
}

DH_CPlayer::~DH_CPlayer()
{
	Release();
}

void DH_CPlayer::Render()
{
	MoveToEx(g_memDC ,(int)m_vPoint[0].x, (int)m_vPoint[0].y, nullptr);
	for (int i = 1; i < 3; ++i)
	{
		LineTo(g_memDC, (int)m_vPoint[i].x, (int)m_vPoint[i].y);
	}
	Ellipse(g_memDC, 100, 100, 200, 200);
}

void DH_CPlayer::Update()
{
}

void DH_CPlayer::Initialize()
{
	//플레이어 인포 초기화
	m_vPlayerInfo.vPos = { 300.f , 400.f, 0.f };
	m_vPlayerInfo.vLook = { 0.f , -1.f, 0.f };
	m_vPlayerInfo.vDir = { 0.f , 0.f , 0.f };

	m_vPoint[0] = { m_vPlayerInfo.vPos.x - 50,  m_vPlayerInfo.vPos.y - 50 , 0.f };
	m_vPoint[1] = { m_vPlayerInfo.vPos.x + 50,  m_vPlayerInfo.vPos.y - 50 , 0.f };
	m_vPoint[2] = { m_vPlayerInfo.vPos.x - 50,  m_vPlayerInfo.vPos.y + 50 , 0.f };
	m_vPoint[3] = { m_vPlayerInfo.vPos.x + 50,  m_vPlayerInfo.vPos.y + 50 , 0.f };


	for (int i = 0; i < 4; ++i)
		m_vOriginPoint[i] = m_vPoint[i];


}

void DH_CPlayer::LateUpdate()
{
	m_eRender = RENDERID::RENDER_PLAYER;
}

void DH_CPlayer::KeyInput()
{
	if (KEY_HOLD(KEY::W))
	{

	}
	if (KEY_HOLD(KEY::A))
	{

	}
	if (KEY_HOLD(KEY::S))
	{

	}
	if (KEY_HOLD(KEY::D))
	{

	}
}

void DH_CPlayer::Change_Motion()
{
}

void DH_CPlayer::Offset()
{
}
