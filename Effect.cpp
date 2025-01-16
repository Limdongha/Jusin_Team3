#include "pch.h"
#include "Effect.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "MainGame.h"
#include "PngManager.h"
#include "SceneMgr.h"

CEffect::CEffect() : m_currentTime(0), m_prevTime(0), m_bDelete(false), m_iFrame(0), m_iCollision(1), m_bFrameSwitch(false)
,m_eEffect(eEffect::END), m_bFollow(false), m_fAlpha(1.f), m_pObj(nullptr), m_bRect(false), m_bOptimized(false), m_bExcuseOne(false)
, m_bOptimized2(false)
{
}

CEffect::~CEffect()
{
	Release();
}


////////////////////////////////////////
// 
//         �߰��� ���� ����
// 
// 1. �̴ϼ� ������� �ε� �߰�(PNG)
// 
// 2. ���¹�ȯ�Լ��� �����߰� 
// 
// 3. ����Ʈ ��ǿ� ��ϵǾ��ִ��� Ȯ��
// 
////////////////////////////////////////

void CEffect::Render()
{
	if (m_bFollow)
	{
		auto Player = CSceneMgr::GetInst()->GetPlayer();
		auto PlayerPos = CSceneMgr::GetInst()->GetPlayer()->GetPos();

		if(m_pObj)
		{
			SetPos(m_pObj->GetPos());
		}
	}
	if (m_eEffect == eEffect::EFFECT)
	{
		AlphaBlend
		((int)GetScale().fX, (int)GetScale().fY, m_fAlpha, Color{250, 0, 250}, Color{ 255, 0, 255 });
	}

#pragma region �ݸ��� Ȯ�ιڽ�

	if (CMainGame::GetInst()->GetiEventBox() == 1)
	{
		//�÷��̾� ����
		if (m_eEffect == eEffect::PLAYERATTACK)
		{
			int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
			int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

			HPEN newPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 140));
			HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

			HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
			HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);


			Ellipse(g_memDC,
				int(GetPos().fX - GetScale().fX / 2 + iScrollX),
				int(GetPos().fY - GetScale().fY / 2 + iScrollY),
				int(GetPos().fX + GetScale().fX / 2 + iScrollX),
				int(GetPos().fY + GetScale().fY / 2 + iScrollY));

			SelectObject(g_memDC, oldPen);
			SelectObject(g_memDC, oldBrush);

			DeleteObject(newBrush);
			DeleteObject(newPen);
		}
		//���� ����
		if (m_eEffect == eEffect::MONSTERATTACK)
		{
			HPEN newPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 140));
			HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

			HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
			HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);


			Ellipse(g_memDC,
				int(GetPos().fX - GetScale().fX / 2 + SCROLLX),
				int(GetPos().fY - GetScale().fY / 2 + SCROLLY),
				int(GetPos().fX + GetScale().fX / 2 + SCROLLX),
				int(GetPos().fY + GetScale().fY / 2 + SCROLLY));

			SelectObject(g_memDC, oldPen);
			SelectObject(g_memDC, oldBrush);

			DeleteObject(newBrush);
			DeleteObject(newPen);
		}
		//����Ʈ
		if (m_eEffect == eEffect::EFFECT)
		{
			int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
			int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

			HPEN newPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 140));
			HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

			HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
			HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);


			Rectangle(g_memDC,
				int(GetPos().fX - GetScale().fX / 2 + iScrollX),
				int(GetPos().fY - GetScale().fY / 2 + iScrollY),
				int(GetPos().fX + GetScale().fX / 2 + iScrollX),
				int(GetPos().fY + GetScale().fY / 2 + iScrollY));

			SelectObject(g_memDC, oldPen);
			SelectObject(g_memDC, oldBrush);

			DeleteObject(newBrush);
			DeleteObject(newPen);
		}
		
	}

#pragma endregion
}

void CEffect::Initialize()
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Player\\doubleJump\\DoubleJumpEx.png", L"PlayerDoubleJumpLeft");
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Player\\doubleJump\\DoubleJumpEx.png", L"PlayerDoubleJumpRight");


	wstring strPNGLeft = EffectNameToString(m_eCurEffect) + L"Left";
	wstring strPNGRight = EffectNameToString(m_eCurEffect) + L"Right";
	m_pLeftPNG = CPngManager::GetInst()->Find_Png(strPNGLeft);
	m_pRightPNG = CPngManager::GetInst()->Find_Png(strPNGRight);
	
	m_pCurPNG = m_pRightPNG;

	m_tFrame.dwTime = GetTickCount64();
	m_eRender = RENDERID::RENDER_EFFECT;
}

void CEffect::Update()
{
	Change_Motion();
	CObject::Move_Frame();

	if (m_eEffect == eEffect::EFFECT)
	{
		if (m_pObj->GetiDirection() < 0)
			m_pCurPNG = m_pLeftPNG;
		if (m_pObj->GetiDirection() > 0)
			m_pCurPNG = m_pRightPNG;
	}

	//�״� �ð�
	m_currentTime = GetTickCount64();
	ULONGLONG total = m_currentTime - m_prevTime;
	if (total >= 50)
	{
		m_prevTime = m_currentTime;

		++m_iFrame;
		if (m_iFrame > GetiDieTime())
		{
			m_bDelete = true;
			m_iFrame = 0;
		}
	}
}

void CEffect::LateUpdate()
{
	
}

void CEffect::AlphaBlend(int iScaleX, int iScaleY, float fAlpha, Color _ColorA, Color _ColorB)
{
	Gdiplus::Graphics graphics(g_memDC);
	float alpha = fAlpha;

	ColorMatrix colorMatrix = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Red
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Green
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Blue
	0.0f, 0.0f, 0.0f, alpha, 0.0f, // Alpha (����)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // W
	};

	// ImageAttributes ��ü ����
	ImageAttributes imgAttributes;
	imgAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	// �ҽ� �̹������� �ڸ� ���� ���� (x, y, width, height)
	Rect sourceRect(0 + (100 * m_tFrame.iFrameStart), 0 + (100 * m_tFrame.iMotion), 100, 100);

	// ��� ��ġ, ũ�� ���� (x, y, width, height)
	Rect destRect = { 
		int(GetPos().fX - iScaleX * 0.5 + SCROLLX),
		int(GetPos().fY - iScaleY * 0.55 + SCROLLY),
		iScaleX, iScaleY };

	// ȥ�� ��� �� ǰ�� ����
	graphics.SetCompositingMode(CompositingModeSourceOver); // ���� ���� Ȱ��ȭ
	graphics.SetCompositingQuality(CompositingQualityHighSpeed); // ��Ӹ��

	//���� ��������
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// �ڸ� �̹����� ���� �������� ȭ�鿡 �׸���
	graphics.DrawImage(m_pCurPNG, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}


//����Ʈ ���
void CEffect::Change_Motion()
{
	//����������̼�
	if (m_ePreEffect != m_eCurEffect)
	{
		switch (m_eCurEffect)
		{
		case eEffectName::DOUBLEJUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}
		m_ePreEffect = m_eCurEffect;
	}
}

//���¹�ȯ �Լ�
const wstring CEffect::EffectNameToString(eEffectName state)
{
	switch (state)
	{
	case eEffectName::DOUBLEJUMP:				return L"PlayerDoubleJump";

	default:                  return L"UNKNOWN";
	}
}


//�̻�� �Լ�
void CEffect::Release()
{
}
