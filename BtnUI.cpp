#include "pch.h"
#include "BtnUI.h"

CBtnUI::CBtnUI() : m_bClicked(false)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseLDown()
{
}

void CBtnUI::MouseLUp()
{
}

void CBtnUI::MouseLClicked()
{
	if (IsVisible())
		m_bClicked = true;
}

void CBtnUI::LateUpdate()
{
	if (!m_bVisible)
		return; // UI�� ��Ȱ��ȭ ���¶�� ������Ʈ���� ����

	SetPinalPos(GetPos());

	if (GetParent())
	{
		//�θ��� ���̳� ��� �ҷ��ͼ� ���� �����ǰ� ������
		tVec2 vParentPos = GetParent()->GetPinalPos();
		SetPinalPos(GetPos() + vParentPos);
	}
	LateUpdateChild();
}
