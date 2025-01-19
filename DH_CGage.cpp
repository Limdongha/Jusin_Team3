#include "pch.h"
#include "DH_CGage.h"
#include "PngManager.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "DH_CPlayer.h"

DH_CGage::DH_CGage()
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\UI\\Gage_Bar.png", L"Gage_Bar");
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\UI\\Gages.png", L"Gages");

	m_pLeftPNG = CPngManager::GetInst()->Find_Png(L"Gage_Bar");
	m_pRightPNG = CPngManager::GetInst()->Find_Png(L"Gages");

	m_eRender = RENDERID::RENDER_UI;
}

DH_CGage::~DH_CGage()
{
}

void DH_CGage::Render()
{
	GETPLAYER;
	if (dynamic_cast<DH_CPlayer*>(&Player)->GetbCharge())
	{
		int iGage = (int)((Player.GetfJumpSpeed() / 35.0f) * 200);

		AlphaBlend(m_pLeftPNG, 150, 150, -270, -170, 0,0, 1.f);
		AlphaBlend(m_pRightPNG, 150, 150, -270, -170, 150, 150 + iGage, 1.f);
	}
}

void DH_CGage::AlphaBlend
(Image* _pImage, int _SizeX, int _SizeY, int _iOffsetX, int _iOffsetY, int _CropY, int _Crop, float _alpha)
{
	Gdiplus::Graphics graphics(g_memDC);
	float alpha = _alpha;

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
	Color transparentColorMin(255, 0, 255); // ����� ����� �ּ� ��
	Color transparentColorMax(255, 0, 255); // ������ ���
	imgAttributes.SetColorKey(transparentColorMin, transparentColorMax, ColorAdjustTypeBitmap);

	// �ҽ� �̹������� �ڸ� ���� ���� (x, y, width, height)
	Rect sourceRect(0, _CropY, _SizeX, _SizeY -_Crop);

	Rect destRect = {
		int(GetPos().fX - GetScale().fX / 2) - _iOffsetX,
		int(GetPos().fY - GetScale().fY / 2) - _iOffsetY + _CropY + (int)SCROLLY,
		_SizeX,
		_SizeY -_Crop };


	// ȥ�� ��� �� ǰ�� ����
	graphics.SetCompositingMode(CompositingModeSourceOver); // ���� ���� Ȱ��ȭ
	graphics.SetCompositingQuality(CompositingQualityHighQuality);

	graphics.SetSmoothingMode(SmoothingModeNone);                   // ��Ƽ�ٸ���� ����
	graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);  // �⺻ �ؽ�Ʈ ������

	//���� ��������
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// �ڸ� �̹����� ���� �������� ȭ�鿡 �׸���
	graphics.DrawImage(_pImage, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}