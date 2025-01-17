#include "pch.h"
#include "KL_CPlayer.h"

KL_CPlayer::KL_CPlayer() :m_pTarget(nullptr), m_bCol(false), m_bJump(false)
{
	for (int i = 0; i < 4; ++i)
	{
		ZeroMemory(m_vPoint[i], sizeof(m_vPoint[i]));
		m_vOriginPoint[i] = m_vPoint[i];
	}
	

	ZeroMemory(m_vGunPoint, sizeof(m_vGunPoint));

	m_vOriginGunPoint = m_vGunPoint;
}

KL_CPlayer::~KL_CPlayer()
{
	Release();
}

void KL_CPlayer::Render()
{
	MoveToEx(g_memDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y, nullptr);

	for (int i = 0; i < 4; ++i)
	{
		LineTo(g_memDC, (int)m_vPoint[i].x, (int)m_vPoint[i].y);

		if (i > 0)
			continue;

		Ellipse(g_memDC,
			int(m_vPoint[i].x - 5.f),
			int(m_vPoint[i].y - 5.f),
			int(m_vPoint[i].x + 5.f),
			int(m_vPoint[i].y + 5.f));
	}

	LineTo(g_memDC, (int)m_vPoint[0].x, (int)m_vPoint[0].y);

	// 포신

	/*MoveToEx(g_memDC, (int)m_tInfo.vPos.x, (int)m_tInfo.vPos.y, nullptr);
	LineTo(g_memDC, (int)m_vGunPoint.x, (int)m_vGunPoint.y);*/
}

void KL_CPlayer::Update()
{
	m_fAngle -= D3DXToRadian(m_fRotateSpeed);

	D3DXMATRIX		matScale, matRotZ, matTrans, matRevZ, matParent;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, -m_fAngle);
	D3DXMatrixRotationZ(&matRevZ, -m_fAngle / 2.f);
	if(nullptr != m_pTarget)
		D3DXMatrixTranslation(&matTrans,  - 30, 0.f, 0.f);

	if (m_pTarget != nullptr)
	{
		matParent = m_pTarget->GetInfo().matWorld;
	}
	

	for (int i = 0; i < 4; ++i)

	{
		m_vPoint[i] = m_vOriginPoint[i];
		
		

		m_tInfo.matWorld = matScale * matTrans * matRevZ * matParent;
	

		D3DXVec3TransformCoord(&m_vPoint[i], &m_vOriginPoint[i], &m_tInfo.matWorld);
	}

	

	m_tInfo.matWorld = matScale * matRotZ  * matRevZ * matParent;
	// 포신
	m_vGunPoint = m_vOriginGunPoint;

	
	

	D3DXVec3TransformCoord(&m_vGunPoint, &m_vOriginGunPoint, &m_tInfo.matWorld);
	

	KeyInput();
}

void KL_CPlayer::Initialize()
{
	m_tInfo.vPos = { 375.f, 300.f, 0.f };
	SetfSpeed(3.f);
	m_tInfo.vLook = { 0.f, -1.f, 0.f };

	m_vOriginPos = m_tInfo.vPos;

	m_vPoint[0] = {  - 20.f, - 20.f, 0.f };
	m_vPoint[1] = { + 20.f,  - 20.f, 0.f };
	m_vPoint[2] = {  + 20.f,  + 20.f, 0.f };
	m_vPoint[3] = { - 20.f,  + 20.f, 0.f };

	for (int i = 0; i < 4; ++i)
		m_vOriginPoint[i] = m_vPoint[i];

	m_vGunPoint = {0.f, - 40.f , 0.f };

	m_vOriginGunPoint = m_vGunPoint;

	m_eRender = RENDERID::RENDER_PLAYER;
}

void KL_CPlayer::LateUpdate()
{

}

void KL_CPlayer::KeyInput()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		m_bJump = true;

	}
}

void KL_CPlayer::Change_Motion()
{
}

void KL_CPlayer::Offset()
{

}

void KL_CPlayer::JumpIng()
{
}
