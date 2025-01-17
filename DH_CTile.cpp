#include "pch.h"
#include "DH_CTile.h"
#include "ScrollMgr.h"
#include "MainGame.h"

DH_CTile::DH_CTile() : m_iDrawID(0), m_iOption(0)
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
	// ·è º¤ÅÍ , µð·º¼Ç º¤ÅÍ ÃÊ±âÈ­
	m_vTileInfo.vDir = { 0, 0, 0 };
	m_vTileInfo.vLook = { 0, -1, 0 };

	m_eRender = RENDERID::RENDER_Tile;
}

void DH_CTile::Update()
{
	m_vTileInfo.vPos = { GetPos().fX ,GetPos().fY , 0.f };


}

void DH_CTile::LateUpdate()
{
}


void DH_CTile::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	if (0 == m_iOption)
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
