#include "pch.h"
#include "PanelUI.h"
#include "MouseMgr.h"

CPanelUI::CPanelUI() : m_vDragStart(tVec2{})
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::MouseOn()
{
	if (IsbLDown())
	{
		tVec2 vDiff;
		vDiff.fX = (float)CMouseMgr::GetInst()->GetPos().x - m_vDragStart.fX;
		vDiff.fY = (float)CMouseMgr::GetInst()->GetPos().y - m_vDragStart.fY;

		tVec2 vCurPos = GetPos();
		vCurPos = vCurPos + vDiff;
		SetPos(vCurPos);

		m_vDragStart.fX = (float)CMouseMgr::GetInst()->GetPos().x;
		m_vDragStart.fY = (float)CMouseMgr::GetInst()->GetPos().y;
	}
}

void CPanelUI::MouseLDown()
{
	m_vDragStart.fX = (float)CMouseMgr::GetInst()->GetPos().x;
	m_vDragStart.fY = (float)CMouseMgr::GetInst()->GetPos().y;
}

void CPanelUI::MouseLUp()
{
}

void CPanelUI::MouseLClicked()
{
}
