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

	Color colorKey(255, 0, 255); // 초록색 배경을 투명하게 처리
	ImageAttributes attributes;
	attributes.SetColorKey(colorKey, colorKey);

	int width = image.GetWidth();
	int height = image.GetHeight();

	PointF center(m_tInfo.vPos.x + iScrollX , m_tInfo.vPos.y + iScrollY  );


	Matrix matrix;
	matrix.RotateAt(-m_fAngle, center); // 중심 기준으로 회전, radian 아니라 degree로 
	graphics.SetTransform(&matrix);

	// 색상 키를 설정하려면 색상 매개변수를 사용합니다 (예: RGB(0, 255, 0) - 녹색 배경)

	graphics.DrawImage(
		&image,
		Rect(int(m_tInfo.vPos.x + iScrollX - width / 2), // 회전 후 왼쪽 상단 좌표 계산
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

	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &matRotZ); // 회전 방향만 표시하려고 사용. 충돌이나 이런 곳에는 사용 안함


	m_tInfo.matWorld = matTrans; // 중점만 이동

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
