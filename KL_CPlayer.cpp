#include "pch.h"
#include "KL_CPlayer.h"
#include "ScrollMgr.h"

KL_CPlayer::KL_CPlayer() :m_pTarget(nullptr), m_bCol(false), m_fRadius(0.f)
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
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	

	
	vector<POINT> points(8);
	for (int i = 0; i < 8; ++i)
	{
		points[i] = {LONG(m_vPoint[i].x +m_tInfo.vPos.x) + iScrollX, LONG(m_vPoint[i].y + m_tInfo.vPos.y)};
	}

	// 브러시 생성 (채우기 색상 지정)
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 191, 255));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(g_memDC, hBrush);

	// 다각형 내부 색 채우기
	Polygon(g_memDC, points.data(), 8);

	// 브러시 복원 및 삭제
	SelectObject(g_memDC, hOldBrush);
	DeleteObject(hBrush);

	Ellipse(g_memDC,
	int(m_vPoint[0].x + m_tInfo.vPos.x - 5.f) + iScrollX,
	int(m_vPoint[0].y + m_tInfo.vPos.y - 5.f),
	int(m_vPoint[0].x + m_tInfo.vPos.x + 5.f) + iScrollX,
	int(m_vPoint[0].y + m_tInfo.vPos.y + 5.f));
	


	// 포신

	MoveToEx(g_memDC, (int)m_tInfo.vPos.x + iScrollX, (int)m_tInfo.vPos.y, nullptr);
	LineTo(g_memDC, int(m_vGunPoint.x + m_tInfo.vPos.x) + iScrollX, int(m_vGunPoint.y + m_tInfo.vPos.y));
	
}

void KL_CPlayer::Update()
{
	
	if (!GetbJump())
	{
		if (nullptr != m_pTarget)
		{
			

			D3DXMATRIX	matScale, matRotZ, matTrans, matRevZ, matParent;
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			m_fAngle -= D3DXToRadian(m_fRotateSpeed * 60);

			D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(m_fAngle));

			D3DXMatrixRotationZ(&matRevZ, -D3DXToRadian(m_fAngle) /2.f );

			D3DXVECTOR3 targetPos = m_pTarget->GetInfo().vPos;
			//targetPos.x += iScrollX;
			
			D3DXMatrixTranslation(&matTrans, -targetPos.x - m_fRadius  , -targetPos.y - m_fRadius, 0.f);

			D3DXMatrixTranslation(&matParent, targetPos.x , targetPos.y, 0.f);
		
			m_tInfo.matWorld = matScale* matTrans * matRevZ * matParent;


			for (int i = 0; i < 8; ++i)
			{
				D3DXVec3TransformNormal(&m_vPoint[i], &m_vOriginPoint[i], &m_tInfo.matWorld);
			}

			

			D3DXVec3TransformCoord(&m_tInfo.vPos, &m_vOriginPos, &m_tInfo.matWorld);

			m_tInfo.matWorld = matScale * matRotZ * matTrans * matRevZ * matParent;

			D3DXVec3TransformNormal(&m_vGunPoint, &m_vOriginGunPoint, &m_tInfo.matWorld);


			m_tInfo.vDir = m_vGunPoint / 40.f;
		

			m_fRadius += 0.2f;
		}
	}

	else
 		JumpIng();

	
}

void KL_CPlayer::Initialize()
{
	m_tInfo.vPos = { 100.f, 350.f, 0.f };
	SetfSpeed(3.f);
	m_tInfo.vLook = { 0.f, -1.f, 0.f };

	m_vOriginPos = m_tInfo.vPos;

	for (int i = 0; i < 8; ++i)
	{
		m_vPoint[i] = { 40.f * cosf(PI / 4 * i), 40.f * sinf(PI / 4 * i), 0.f };
	}
	

	for (int i = 0; i < 8; ++i)
		m_vOriginPoint[i] = m_vPoint[i];

	m_vGunPoint = {0.f, - 40.f , 0.f };

	m_vOriginGunPoint = m_vGunPoint;

	m_eRender = RENDERID::RENDER_PLAYER;
}

void KL_CPlayer::LateUpdate()
{

	KeyInput();

	Offset();
}

void KL_CPlayer::KeyInput()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		SetbJump(true);

	}
}

void KL_CPlayer::Change_Motion()
{
}

void KL_CPlayer::Offset()
{

	int		iOffSetminX = 0;
	int		iOffSetmaxX = 400;

	
	
	int		iOffSetminY = 350;
	int		iOffSetmaxY = 400;

	if (!CScrollMgr::Get_Instance()->GetbShake01())
	{
		int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

		if (iOffSetminX > m_vOriginPos.x + iScrollX)
			CScrollMgr::Get_Instance()->Add_ScrollX(5);
		
		if(iOffSetmaxX < m_vOriginPos.x + iScrollX)
			CScrollMgr::Get_Instance()->Add_ScrollX(-5);
		//if (iOffSetmaxY < GetPos().fY + iScrollY)
			//CScrollMgr::Get_Instance()->Add_ScrollY(-m_fSpeed);
	}


	
}

void KL_CPlayer::JumpIng()
{
	AddfTime(0.2f);

	m_tInfo.vPos += m_tInfo.vDir * fabs(m_fRotateSpeed) * 7.5f;
	
	

	if (GetfTime() > 2.f)
	{
		m_vOriginPos = m_tInfo.vPos;
		m_fRadius = 0.f;
		m_fAngle = 0.f;
		SetbJump(false);
		SetfTime(0.f);
	}
		

}
