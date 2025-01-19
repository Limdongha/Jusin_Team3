#include "pch.h"
#include "DH_CTile.h"
#include "ScrollMgr.h"
#include "MainGame.h"
#include "ResMgr.h"
#include "Textures.h"
#include "PngManager.h"

DH_CTile::DH_CTile() : m_iDrawID(0), m_iOption(0), m_UniqTile(false)
{
	ZeroMemory(&m_vTileInfo, sizeof(INFO));
	ZeroMemory(&m_vTilePos, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vTileOriginPos, sizeof(D3DXVECTOR3));

}

DH_CTile::~DH_CTile()
{
	Release();
}

void DH_CTile::Initialize()
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Cloud\\Cloud.png", L"CloudPNG");
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Cloud\\CloudUniq.png", L"CloudUniqPNG");

	m_pCurPNG = CPngManager::GetInst()->Find_Png(L"CloudPNG");
	m_pRightPNG = CPngManager::GetInst()->Find_Png(L"CloudUniqPNG");


	m_eRender = RENDERID::RENDER_Tile;
}

void DH_CTile::Update()
{


}

void DH_CTile::LateUpdate()
{
}


void DH_CTile::Render()
{
	if(m_UniqTile)
		AlphaBlend(m_pRightPNG, 1.f);
	else
		AlphaBlend(m_pCurPNG, 1.f);


	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	if (CMainGame::GetInst()->GetiEventBox() == 1)
	{
		HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
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


void DH_CTile::Release()
{
}

void DH_CTile::AlphaBlend(Image* _pImage, float _alpha)
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
	Rect sourceRect(0, 0, 230, 100);

	int iOffset = 30;
	Rect destRect = {
		int(GetPos().fX - GetScale().fX / 2) - iOffset + 20,
		int(GetPos().fY - GetScale().fY / 2) - iOffset + (int)SCROLLY,
		230,
		100};


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