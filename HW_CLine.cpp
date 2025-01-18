#include "pch.h"
#include "HW_CLine.h"
#include "ScrollMgr.h"
HW_CLine::HW_CLine()
{
}

HW_CLine::HW_CLine(LINEPOINT& tLeft, LINEPOINT& tRight) : m_tInfo(tLeft, tRight)
{
}

HW_CLine::~HW_CLine()
{
}

void HW_CLine::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	MoveToEx(g_memDC, (int)m_tInfo.tLPoint.fX + iScrollX, (int)m_tInfo.tLPoint.fY, nullptr);
	LineTo(g_memDC, (int)m_tInfo.tRPoint.fX + iScrollX, (int)m_tInfo.tRPoint.fY);
}
