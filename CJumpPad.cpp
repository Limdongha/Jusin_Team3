#include "pch.h"
#include "CJumpPad.h"
#include "ScrollMgr.h"
#include "PngManager.h"

CJumpPad::CJumpPad() 
{
}

CJumpPad::~CJumpPad()
{
}

void CJumpPad::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = int(CScrollMgr::Get_Instance()->Get_ScrollY());

	
	

	Graphics graphics(g_memDC);

	Image image(L"./\\Content\\Textures\\Leaf\\Leaf.bmp");

	Color colorKey(255, 0, 255); // �ʷϻ� ����� �����ϰ� ó��
	ImageAttributes attributes;
	attributes.SetColorKey(colorKey, colorKey);

	int width = image.GetWidth();
	int height = image.GetHeight();

	PointF center(m_tInfo.vPos.x + iScrollX , m_tInfo.vPos.y + iScrollY  );


	Matrix matrix;
	matrix.RotateAt(-m_fAngle, center); // �߽� �������� ȸ��, radian �ƴ϶� degree�� 
	graphics.SetTransform(&matrix);

	// ���� Ű�� �����Ϸ��� ���� �Ű������� ����մϴ� (��: RGB(0, 255, 0) - ��� ���)

	graphics.DrawImage(
		&image,
		Rect(int(m_tInfo.vPos.x + iScrollX - width / 2), // ȸ�� �� ���� ��� ��ǥ ���
			int(m_tInfo.vPos.y + iScrollY - height / 2),
			width, height),  // dest rect
		0, 0, image.GetWidth(), image.GetHeight(),          // source rect
		UnitPixel,
		&attributes);


	graphics.ResetTransform();
}

void CJumpPad::Update()
{
	
	D3DXMATRIX	matScale, matTrans, matRotZ, matTransTarget;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &matRotZ); // ȸ�� ���⸸ ǥ���Ϸ��� ���. �浹�̳� �̷� ������ ��� ����


	m_tInfo.matWorld = matTrans; // ������ �̵�

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
