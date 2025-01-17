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
	LineTo(g_memDC, int(m_tInfo.vPos.x + m_tInfo.vDir.x * 50), int(m_tInfo.vPos.y + m_tInfo.vDir.y * 50));
}

void CJumpPad::Update()
{
	D3DXMATRIX	matScale,	matTrans, matRotZ, matTransTarget;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);
	
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &matRotZ); // ȸ�� ���⸸ ǥ���Ϸ��� ���. �浹�̳� �̷� ������ ��� ����

	
	m_tInfo.matWorld =  matTrans ; // ������ �̵�

	m_fAngle -= m_fRotateSpeed;
}

void CJumpPad::Initialize()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	
	m_tInfo.vLook = { 0.f,-1.f,0.f };
	
	m_tInfo.vDir = { 0.f, -1.f, 0.f };
	m_eRender = RENDERID::RENDER_GAMEOBJECT;
}

void CJumpPad::LateUpdate()
{
}
