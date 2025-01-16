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
		return; // UI�� ��Ȱ��ȭ ���¶�� ���������� ����

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

	//��� Ŭ���� �ؽ�Ʈ ���;���
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
				//�ܾ� ���� �� (����)
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
				//�ܾ׺����� (����)
				CSoundMgr::GetInst()->StopSound(SOUND_UI);
				CSoundMgr::GetInst()->PlaySound(L"UI_MeneClick.wav", SOUND_UI, g_fVolume);

				m_bBuy = false;
			}
		}
		else
		{
			SetBkMode(g_memDC, TRANSPARENT); // ����� �����ϰ� ����
			SetTextColor(g_memDC, RGB(156, 129, 97)); // �ؽ�Ʈ ����

			wchar_t Price[50];
			swprintf(Price, 50, L"%d ���", 400);

			SIZE textSize;
			GetTextExtentPoint32(g_memDC, Price, int(wcslen(Price)), &textSize); // �ؽ�Ʈ ũ�� ���

			int textX = int(GetPos().fX) + 134 - (textSize.cx / 2); // �ؽ�Ʈ�� �ʺ� �̿��� X ��ǥ ����
			int textY = int(GetPos().fY) + 243; // Y ��ǥ�� �״��

			TextOut(
				g_memDC,
				textX, // ������ X ��ǥ
				textY, // ������ Y ��ǥ
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
	0.0f, 0.0f, 0.0f, alpha, 0.0f, // Alpha (����)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // W
	};

	// ImageAttributes ��ü ����
	ImageAttributes imgAttributes;
	imgAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	Color transparentColorMin(250, 0, 250); // ����� ����� �ּ� ��
	Color transparentColorMax(255, 0, 255); // ������ ���
	imgAttributes.SetColorKey(transparentColorMin, transparentColorMax, ColorAdjustTypeBitmap);

	// �ҽ� �̹������� �ڸ� ���� ���� (x, y, width, height)
	Rect sourceRect(0, 0, iScaleX, iScaleY);

	// ��� ��ġ, ũ�� ���� (x, y, width, height)

	Rect destRect = {
		int(GetPos().fX),
		int(GetPos().fY),
		iScaleX, iScaleY };

	// ȥ�� ��� �� ǰ�� ����
	graphics.SetCompositingMode(CompositingModeSourceOver); // ���� ���� Ȱ��ȭ
	graphics.SetCompositingQuality(CompositingQualityHighQuality);

	graphics.SetSmoothingMode(SmoothingModeNone);                   // ��Ƽ�ٸ���� ����
	graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);  // �⺻ �ؽ�Ʈ ������

	//���� ��������
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// �ڸ� �̹����� ���� �������� ȭ�鿡 �׸���
	graphics.DrawImage(_PNG, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}