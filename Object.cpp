#include "pch.h"
#include "Object.h"
#include "Define.h"
#include "Effect.h"
#include "EffectMgr.h"
#include "SceneMgr.h"

CObject::CObject() :
	m_tObjInfo(tObjInfo{ tVec2{100,100},tVec2{100,100} }), m_pCurTex(nullptr),
	m_bArrive(true), m_rRect(RECT{}), m_pLeftTex(nullptr), m_pRightTex(nullptr), m_iDirection(0),
	m_eCurState(STATE::IDLE), m_ePreState(STATE::IDLE), m_tFrame(FRAME{}), m_bFalling(false),
	m_pCurTile(nullptr), m_vCurTileVector(),m_bJump(false), m_fJumpSpeed(16.f), m_fTime(0.f),
	m_pCurPNG(nullptr), m_pLeftPNG(nullptr), m_pRightPNG(nullptr), m_fGravity(3.6f), m_bDoubleJump(false),
	m_ePreEffect(eEffectName::END), m_eCurEffect(eEffectName::END), m_eRender(RENDERID::RENDER_END),
	m_iHP(100), m_fSpeed(3.0f), m_fCusDeltaTime(0.667f), m_fRealDeltaTime(0.01667f), m_iDieTime(4),
	m_bDie(false), m_fCurve(0.f), m_fCurve2(0.f), m_fDoubleCurve(0.f), m_bJumpSwitch(false), m_bMoveFrame(true), m_fRotateSpeed(0.f), m_fAngle(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
}										   

CObject::~CObject()
{
	Release();
}

//������Ʈ �׸���
void CObject::Render()
{
}

void CObject::Release()
{
}

void CObject::Move_Frame()
{
	if (m_bMoveFrame)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

//���ùڽ� ����
void CObject::CreateAttackBox
(float _PosfX, float _PosfY, float _ScaleX, float _ScaleY, eEffect _eType, eEffectName _enType,
bool _follow, float _fOffsetX, float _fOffsetY, bool _Rect)
{
	CEffect* AttackBox = new CEffect;

	if (GetiDirection() < 0)
		AttackBox->SetPos(tVec2{ _PosfX - _fOffsetX, _PosfY + _fOffsetY });
	if (GetiDirection() > 0)
		AttackBox->SetPos(tVec2{ _PosfX + _fOffsetX, _PosfY + _fOffsetY });

	AttackBox->SetScale(tVec2{ _ScaleX, _ScaleY });
	AttackBox->SeteEffect(_eType);
	AttackBox->SetbFollow(_follow);
	AttackBox->Initialize();
	AttackBox->SetCurEffect(_enType);
	AttackBox->SetbRect(_Rect);
	CEffectMgr::GetInst()->AddVecEffect(AttackBox, _eType);
}

void CObject::LateUpdate() 
{
	Move_Frame();
}

void CObject::DrawPolygon(int _PointNum, float centerX, float centerY, float radius, COLORREF _Color)
{
	// �� �迭 ���� (���ݸ��缭 ����)
	vector<D3DXVECTOR3> points(_PointNum);
	GenerateRegularPolygon(points.data(), _PointNum, centerX, centerY, radius);

	// POINT�� ��ȯ
	vector<POINT> screenPoints(_PointNum);
	for (int i = 0; i < _PointNum; ++i)
	{
		screenPoints[i] = { (LONG)points[i].x, (LONG)points[i].y };
	}

	// �귯�� ����
	HBRUSH hBrush = CreateSolidBrush(_Color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(g_memDC, hBrush);

	// �ٰ��� �׸���
	Polygon(g_memDC, screenPoints.data(), _PointNum);

	// �귯�� ���� �� ����
	SelectObject(g_memDC, hOldBrush);
	DeleteObject(hBrush);
}

void CObject::GenerateRegularPolygon(D3DXVECTOR3* points, int numPoints, float centerX, float centerY, float radius)
{
	float angleStep = 360.0f / numPoints; // �� ������ ����
	for (int i = 0; i < numPoints; ++i) 
	{
		float angle = angleStep * i * (D3DX_PI / 180.0f); // ������ �������� ��ȯ
		points[i].x = centerX + radius * cos(angle); // X ��ǥ
		points[i].y = centerY + radius * sin(angle); // Y ��ǥ
		points[i].z = 0; // Z�� 2D�� 0
	}
}

void CObject::DrawPolygonCustom(D3DXVECTOR3* _Point, int _PointNum, COLORREF _Color)
{
	// POINT �迭 ����
	vector<POINT> points(_PointNum);
	for (int i = 0; i < _PointNum; ++i)
	{
		points[i] = { (LONG)_Point[i].x, (LONG)_Point[i].y };
	}

	// �귯�� ���� (ä��� ���� ����)
	HBRUSH hBrush = CreateSolidBrush(_Color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(g_memDC, hBrush);

	// �ٰ��� ���� �� ä���
	Polygon(g_memDC, points.data(), _PointNum);

	// �귯�� ���� �� ����
	SelectObject(g_memDC, hOldBrush);
	DeleteObject(hBrush);
}


// ���� �̻�� �Լ���
void CObject::Update(){}
void CObject::Initialize(){}
