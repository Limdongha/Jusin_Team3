#include "pch.h"
#include "Tile.h"
#include "ScrollMgr.h"
#include "MainGame.h"

CTile::CTile() : m_iDrawID(0), m_iOption(0)
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	SetScale(tVec2{ TILECX , TILECY });
	m_eRender = RENDERID::RENDER_Tile;
}

void CTile::Update()
{
}

void CTile::LateUpdate()
{
}


void CTile::Render()
{
	if (CMainGame::GetInst()->GetiEventBox() == 1)
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
		if (1 == m_iOption)
		{
			HPEN newPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
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
}

void CTile::Release()
{
}
