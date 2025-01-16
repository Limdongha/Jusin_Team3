#include "pch.h"
#include "StoreUI.h"
#include "PngManager.h"
#include "BtnUI.h"
#include "Player.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "HeadItem.h"


CStoreUI::CStoreUI() : m_bGold(false), m_bBuy(false)
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\UI\\Store.png", L"Store");

	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\UI\\Store_Clicked.png", L"Store_Clicked");

	m_pLeftPNG = CPngManager::GetInst()->Find_Png(L"Store");
	m_pRightPNG = CPngManager::GetInst()->Find_Png(L"Store_Clicked");
}

CStoreUI::~CStoreUI()
{
}

void CStoreUI::Render()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 렌더링하지 않음

	if (m_bLDown)
	{
		m_pCurPNG = m_pRightPNG;
		AlphaBlend(280, 500, 1.0f, m_pCurPNG);
	}
	else
	{
		m_pCurPNG = m_pLeftPNG;
		AlphaBlend(280, 500, 1.0f, m_pCurPNG);
	}

	//헤드 클릭시 텍스트 나와야함
	for (auto& ChildUI : GetChildUI())
	{
		if (ChildUI->GetName() == L"StoreUI_Slot01")
		{
			if (dynamic_cast<CBtnUI*>(ChildUI)->GetbClicked())
			{
				CSoundMgr::GetInst()->StopSound(SOUND_UI);
				CSoundMgr::GetInst()->PlaySound(L"UI_MeneClick.wav", SOUND_UI, g_fVolume);

				dynamic_cast<CBtnUI*>(ChildUI)->SetbClicked(false);
				m_bGold = true;
			}
		}
		if (ChildUI->GetName() == L"StoreUIBuy")
		{
			if (dynamic_cast<CBtnUI*>(ChildUI)->GetbClicked())
			{
				dynamic_cast<CBtnUI*>(ChildUI)->SetbClicked(false);
				m_bBuy = true;
			}
		}
	}
	if (m_bGold)
	{
		if (m_bBuy)
		{
			GETPLAYER;
			if (dynamic_cast<CPlayer*>(&Player)->GetiGold() > 400)
			{
				//잔액 있을 시 (사운드)
				CSoundMgr::GetInst()->StopSound(SOUND_UI);
				CSoundMgr::GetInst()->PlaySound(L"UI_StoreBuy.wav", SOUND_UI, g_fVolume);

				dynamic_cast<CPlayer*>(&Player)->AddiGold(-400);

				CHeadItem* Head = new CHeadItem;
				Head->SetName(L"BerserkerHead");
				Head->SetPos(tVec2{ 82 ,112 });
				Head->SetScale(tVec2{ 70, 38 });
				Head->Initialize();
				auto& Iven = *dynamic_cast<CPlayer*>(&Player)->GetInven();
				for (auto& ChildUI : Iven.GetChildUI())
				{
					if (ChildUI->GetName() == L"InventoryChildUI1")
					{
						ChildUI->AddParent(Head);
						Head->SetVisible(true);
					}
				}
				m_bBuy = false;
			}
			else if (dynamic_cast<CPlayer*>(&Player)->GetiGold() < 400)
			{
				//잔액부족시 (사운드)
				CSoundMgr::GetInst()->StopSound(SOUND_UI);
				CSoundMgr::GetInst()->PlaySound(L"UI_MeneClick.wav", SOUND_UI, g_fVolume);

				m_bBuy = false;
			}
		}
		else
		{
			SetBkMode(g_memDC, TRANSPARENT); // 배경을 투명하게 설정
			SetTextColor(g_memDC, RGB(156, 129, 97)); // 텍스트 색상

			wchar_t Price[50];
			swprintf(Price, 50, L"%d 골드", 400);

			SIZE textSize;
			GetTextExtentPoint32(g_memDC, Price, int(wcslen(Price)), &textSize); // 텍스트 크기 계산

			int textX = int(GetPos().fX) + 134 - (textSize.cx / 2); // 텍스트의 너비를 이용해 X 좌표 조정
			int textY = int(GetPos().fY) + 243; // Y 좌표는 그대로

			TextOut(
				g_memDC,
				textX, // 수정된 X 좌표
				textY, // 수정된 Y 좌표
				Price,
				int(wcslen(Price))
			);
		}
	}
	RenderChild();
}

void CStoreUI::AlphaBlend(int iScaleX, int iScaleY, float fAlpha, Image* _PNG)
{
	tVec2 vPos = GetPinalPos();

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
	Color transparentColorMin(250, 0, 250); // 흰색에 가까운 최소 값
	Color transparentColorMax(255, 0, 255); // 완전한 흰색
	imgAttributes.SetColorKey(transparentColorMin, transparentColorMax, ColorAdjustTypeBitmap);

	// 소스 이미지에서 자를 영역 정의 (x, y, width, height)
	Rect sourceRect(0, 0, iScaleX, iScaleY);

	// 대상 위치, 크기 정의 (x, y, width, height)

	Rect destRect = {
		int(GetPos().fX),
		int(GetPos().fY),
		iScaleX, iScaleY };

	// 혼합 모드 및 품질 설정
	graphics.SetCompositingMode(CompositingModeSourceOver); // 알파 블렌딩 활성화
	graphics.SetCompositingQuality(CompositingQualityHighQuality);

	graphics.SetSmoothingMode(SmoothingModeNone);                   // 안티앨리어싱 제거
	graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);  // 기본 텍스트 렌더링

	//보간 금지설정
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// 자른 이미지를 알파 블렌딩으로 화면에 그리기
	graphics.DrawImage(_PNG, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}