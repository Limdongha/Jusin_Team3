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
		return; // UI�� ��Ȱ��ȭ ���¶�� ������Ʈ���� ����

	MouseOnCheck();
	UpdateChild();
}

void CUI::Render()
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ���������� ����
		
	tVec2 vPos = GetPinalPos();


	if (m_bLDown)
	{
		HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);

		//���� ��� �𼭸� �������� ������
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

		//���� ��� �𼭸� �������� ������
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
		return; // UI�� ��Ȱ��ȭ ���¶�� ������Ʈ���� ����


	//UI �� ���� ��ǥ�� ���Ѵ�.
	m_vFinalPos = GetPos();

	if (GetParent())
	{
		//�θ��� ���̳� ��� �ҷ��ͼ� ���� �����ǰ� ������
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
	//���콺 ��ǥ ��������
	tVec2 vMousePos;
	vMousePos.fX = (float)CMouseMgr::GetInst()->GetPos().x;
	vMousePos.fY = (float)CMouseMgr::GetInst()->GetPos().y;

	if (m_vFinalPos.fX <= vMousePos.fX && vMousePos.fX <= m_vFinalPos.fX + GetScale().fX
		&& m_vFinalPos.fY <= vMousePos.fY && vMousePos.fY <= m_vFinalPos.fY + GetScale().fY)
	{
		//���콺�� �ö�����
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
