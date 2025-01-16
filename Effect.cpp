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
//         추가시 주의 사항
// 
// 1. 이니셜 라이즈에서 로딩 추가(PNG)
// 
// 2. 상태반환함수에 상태추가 
// 
// 3. 이펙트 모션에 등록되어있는지 확인
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

#pragma region 콜리젼 확인박스

	if (CMainGame::GetInst()->GetiEventBox() == 1)
	{
		//플레이어 어택
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
		//몬스터 어택
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
		//이펙트
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

	//죽는 시간
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
	0.0f, 0.0f, 0.0f, alpha, 0.0f, // Alpha (투명도)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // W
	};

	// ImageAttributes 객체 생성
	ImageAttributes imgAttributes;
	imgAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	// 소스 이미지에서 자를 영역 정의 (x, y, width, height)
	Rect sourceRect(0 + (100 * m_tFrame.iFrameStart), 0 + (100 * m_tFrame.iMotion), 100, 100);

	// 대상 위치, 크기 정의 (x, y, width, height)
	Rect destRect = { 
		int(GetPos().fX - iScaleX * 0.5 + SCROLLX),
		int(GetPos().fY - iScaleY * 0.55 + SCROLLY),
		iScaleX, iScaleY };

	// 혼합 모드 및 품질 설정
	graphics.SetCompositingMode(CompositingModeSourceOver); // 알파 블렌딩 활성화
	graphics.SetCompositingQuality(CompositingQualityHighSpeed); // 고속모드

	//보간 금지설정
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// 자른 이미지를 알파 블렌딩으로 화면에 그리기
	graphics.DrawImage(m_pCurPNG, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}


//이펙트 모션
void CEffect::Change_Motion()
{
	//모션인포메이션
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

//상태반환 함수
const wstring CEffect::EffectNameToString(eEffectName state)
{
	switch (state)
	{
	case eEffectName::DOUBLEJUMP:				return L"PlayerDoubleJump";

	default:                  return L"UNKNOWN";
	}
}


//미사용 함수
void CEffect::Release()
{
}
