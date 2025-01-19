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


	CreateText(L" ���ھ� : %d", 50, RGB(0, 0, 0), (int)WINCX / 2 - 10, 230, (int)SCROLLY);
	CreateText(L" �� ȹ�� : %d", 30, RGB(0, 0, 0), (int)WINCX / 2 - 10, 300, 0);
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
	Rect sourceRect(0, _CropY, _SizeX, _SizeY - _Crop);

	Rect destRect = {
		int(GetPos().fX - GetScale().fX / 2) - _iOffsetX,
		int(GetPos().fY - GetScale().fY / 2) - _iOffsetY + _CropY + (int)SCROLLY,
		_SizeX,
		_SizeY - _Crop };


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

void DH_CGameOver::CreateText(const wchar_t* _LText, int _TextSize, COLORREF _color, int _PosX, int _PosY, int _InputNum)
{
	SetBkMode(g_memDC, TRANSPARENT); // ����� �����ϰ� ����
	SetTextColor(g_memDC, _color); // �ؽ�Ʈ ����

	HFONT hFont = CreateFont(
		_TextSize,                // ���� (���� ũ��)
		0,                 // �ʺ� (0�̸� �ڵ� ����)
		0, 0,              // ����� ���� (������� ����)
		FW_BOLD,           // ���� (FW_NORMAL, FW_BOLD �� ���� ����)
		FALSE,             // ���Ÿ�ü ����
		FALSE,             // ���� ����
		FALSE,             // ��Ҽ� ����
		HANGEUL_CHARSET,   // ���ڼ� (�ѱ�: HANGEUL_CHARSET)
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"���� ���" // �۲� �̸�
	);

	HFONT hOldFont = (HFONT)SelectObject(g_memDC, hFont);

	wchar_t scoreText01[50];
	swprintf(scoreText01, 50, _LText, _InputNum);

	// �ؽ�Ʈ ũ�� ���
	SIZE textSize;
	GetTextExtentPoint32(g_memDC, scoreText01, (int)wcslen(scoreText01), &textSize);

	// ��� ���ĵ� X ��ǥ ���
	int alignedX = _PosX - (textSize.cx / 2); // _PosX�� ���� X ��ǥ
	int alignedY = _PosY;                     // Y ��ǥ�� �״��

	// �ؽ�Ʈ ���
	TextOut(g_memDC, alignedX, alignedY, scoreText01, (int)wcslen(scoreText01));

	SelectObject(g_memDC, hOldFont);
	DeleteObject(hFont);
}
