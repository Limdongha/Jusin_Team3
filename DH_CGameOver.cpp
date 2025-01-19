#include "pch.h"
#include "DH_CGameOver.h"
#include "PngManager.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "DH_CPlayer.h"
#include "ResMgr.h"
#include "Textures.h"

DH_CGameOver::DH_CGameOver() 
{

	m_pUI = CResMgr::GetInst()->LoadTexture
	(L"UI", L"./\\Content\\Textures\\UI\\UI.bmp");


	m_eRender = RENDERID::RENDER_UI;
}

DH_CGameOver::~DH_CGameOver()
{
}

void DH_CGameOver::Render()
{
	GdiTransparentBlt(
		g_memDC,
		WINCX / 2 - 175,
		WINCY / 2 - 150,
		350,
		300,
		m_pUI->GetDC(),
		0,
		0,
		350,
		300,
		RGB(255, 0, 255)
	);


	CreateText(L" 스코어 : %d", 50, RGB(0, 0, 0), (int)WINCX / 2 - 10, 230, (int)SCROLLY);
	CreateText(L" 별 획득 : %d", 30, RGB(0, 0, 0), (int)WINCX / 2 - 10, 300, 0);
}

void DH_CGameOver::AlphaBlend
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
	Rect sourceRect(0, _CropY, _SizeX, _SizeY - _Crop);

	Rect destRect = {
		int(GetPos().fX - GetScale().fX / 2) - _iOffsetX,
		int(GetPos().fY - GetScale().fY / 2) - _iOffsetY + _CropY + (int)SCROLLY,
		_SizeX,
		_SizeY - _Crop };


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

void DH_CGameOver::CreateText(const wchar_t* _LText, int _TextSize, COLORREF _color, int _PosX, int _PosY, int _InputNum)
{
	SetBkMode(g_memDC, TRANSPARENT); // 배경을 투명하게 설정
	SetTextColor(g_memDC, _color); // 텍스트 색상

	HFONT hFont = CreateFont(
		_TextSize,                // 높이 (글자 크기)
		0,                 // 너비 (0이면 자동 설정)
		0, 0,              // 기울기와 방향 (사용하지 않음)
		FW_BOLD,           // 굵기 (FW_NORMAL, FW_BOLD 등 선택 가능)
		FALSE,             // 이탤릭체 여부
		FALSE,             // 밑줄 여부
		FALSE,             // 취소선 여부
		HANGEUL_CHARSET,   // 문자셋 (한글: HANGEUL_CHARSET)
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"맑은 고딕" // 글꼴 이름
	);

	HFONT hOldFont = (HFONT)SelectObject(g_memDC, hFont);

	wchar_t scoreText01[50];
	swprintf(scoreText01, 50, _LText, _InputNum);

	// 텍스트 크기 계산
	SIZE textSize;
	GetTextExtentPoint32(g_memDC, scoreText01, (int)wcslen(scoreText01), &textSize);

	// 가운데 정렬된 X 좌표 계산
	int alignedX = _PosX - (textSize.cx / 2); // _PosX는 기준 X 좌표
	int alignedY = _PosY;                     // Y 좌표는 그대로

	// 텍스트 출력
	TextOut(g_memDC, alignedX, alignedY, scoreText01, (int)wcslen(scoreText01));

	SelectObject(g_memDC, hOldFont);
	DeleteObject(hFont);
}
