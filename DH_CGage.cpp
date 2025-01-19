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
	0.0f, 0.0f, 0.0f, alpha, 0.0f, // Alpha (투명도)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // W
	};

	// ImageAttributes 객체 생성
	ImageAttributes imgAttributes;
	imgAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	Color transparentColorMin(255, 0, 255); // 흰색에 가까운 최소 값
	Color transparentColorMax(255, 0, 255); // 완전한 흰색
	imgAttributes.SetColorKey(transparentColorMin, transparentColorMax, ColorAdjustTypeBitmap);

	// 소스 이미지에서 자를 영역 정의 (x, y, width, height)
	Rect sourceRect(0, _CropY, _SizeX, _SizeY -_Crop);

	Rect destRect = {
		int(GetPos().fX - GetScale().fX / 2) - _iOffsetX,
		int(GetPos().fY - GetScale().fY / 2) - _iOffsetY + _CropY + (int)SCROLLY,
		_SizeX,
		_SizeY -_Crop };


	// 혼합 모드 및 품질 설정
	graphics.SetCompositingMode(CompositingModeSourceOver); // 알파 블렌딩 활성화
	graphics.SetCompositingQuality(CompositingQualityHighQuality);

	graphics.SetSmoothingMode(SmoothingModeNone);                   // 안티앨리어싱 제거
	graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);  // 기본 텍스트 렌더링

	//보간 금지설정
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// 자른 이미지를 알파 블렌딩으로 화면에 그리기
	graphics.DrawImage(_pImage, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}