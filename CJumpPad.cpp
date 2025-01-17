#include "pch.h"
#include "CJumpPad.h"

CJumpPad::CJumpPad()
{
}

CJumpPad::~CJumpPad()
{
}

void CJumpPad::Render()
{
	Ellipse(g_memDC,
		int(m_tInfo.vPos.x - 50.f),
		int(m_tInfo.vPos.y - 50.f),
		int(m_tInfo.vPos.x + 50.f),
		int(m_tInfo.vPos.y + 50.f));
	MoveToEx(g_memDC, int(m_tInfo.vPos.x), int(m_tInfo.vPos.y), nullptr);
	LineTo(g_memDC, int(m_tInfo.vPos.x + m_vGunPoint.x), int(m_tInfo.vPos.y + m_vGunPoint.y));
}

void CJumpPad::Update()
{
	D3DXMATRIX	matScale,	matTrans, matRotZ;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vDir, &matRotZ);

	D3DXVec3TransformCoord(&m_vGunPoint, &m_vOriginGunPoint, &(matRotZ));

	m_tInfo.matWorld = matScale * matRotZ * matTrans ;

	m_fAngle -= m_fRotateSpeed;
}

void CJumpPad::Initialize()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_vTargetPos = { 0.f, 0.f, 0.f };
	
	m_vGunPoint = { 0.f,-50.f,0.f };
	m_vOriginGunPoint = m_vGunPoint;
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_eRender = RENDERID::RENDER_GAMEOBJECT;
}

void CJumpPad::LateUpdate()
{
}
