#include "pch.h"
#include "UI.h"
#include "MouseMgr.h"

CUI::CUI() : m_vFinalPos(tVec2{}), m_pParnetUI(nullptr), m_bMouseOn(false), m_bLDown(false),
m_bVisible(false)
{
}

CUI::~CUI()
{
	if (0 < m_vecChildUI.size())
	{
		for (auto& Child : m_vecChildUI)
		{
			Safe_Delete(Child);
		}
	}
}

void CUI::Update()
{
	if (!m_bVisible) 
		return; // UI가 비활성화 상태라면 업데이트하지 않음

	MouseOnCheck();
	UpdateChild();
}

void CUI::Render()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 렌더링하지 않음
		
	tVec2 vPos = GetPinalPos();


	if (m_bLDown)
	{
		HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);

		//왼쪽 상단 모서리 기준으로 나오기
		Rectangle(
			g_memDC,
			int(vPos.fX),
			int(vPos.fY),
			int(vPos.fX + GetScale().fX),
			int(vPos.fY + GetScale().fY)
		);

		SelectObject(g_memDC, oldPen);
		SelectObject(g_memDC, oldBrush);

		DeleteObject(newBrush);
		DeleteObject(newPen);
	}
	else
	{
		HPEN newPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);

		//왼쪽 상단 모서리 기준으로 나오기
		Rectangle(
			g_memDC,
			int(vPos.fX),
			int(vPos.fY),
			int(vPos.fX + GetScale().fX),
			int(vPos.fY + GetScale().fY)
		);

		SelectObject(g_memDC, oldPen);
		SelectObject(g_memDC, oldBrush);

		DeleteObject(newBrush);
		DeleteObject(newPen);
	}

	RenderChild();
}

void CUI::Initialize()
{
	m_eRender = RENDERID::RENDER_UI;
}

void CUI::LateUpdate()
{
	if (!m_bVisible)
		return; // UI가 비활성화 상태라면 업데이트하지 않음


	//UI 의 최종 좌표를 구한다.
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		//부모의 파이널 포즈를 불러와서 현재 포지션과 더해줌
		tVec2 vParentPos = GetParent()->GetPinalPos();
		m_vFinalPos = m_vFinalPos + vParentPos;
	}
	LateUpdateChild();
}

void CUI::MouseOn()
{

}

void CUI::MouseLDown()
{

}

void CUI::MouseLUp()
{

}

void CUI::MouseLClicked()
{

}


void CUI::MouseOnCheck()
{
	//마우스 좌표 가져오기
	tVec2 vMousePos;
	vMousePos.fX = (float)CMouseMgr::GetInst()->GetPos().x;
	vMousePos.fY = (float)CMouseMgr::GetInst()->GetPos().y;

	if (m_vFinalPos.fX <= vMousePos.fX && vMousePos.fX <= m_vFinalPos.fX + GetScale().fX
		&& m_vFinalPos.fY <= vMousePos.fY && vMousePos.fY <= m_vFinalPos.fY + GetScale().fY)
	{
		//마우스가 올라가있음
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::UpdateChild()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void CUI::RenderChild()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render();
	}
}

void CUI::LateUpdateChild()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->LateUpdate();
	}
}
