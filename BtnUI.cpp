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
		return; // UI가 비활성화 상태라면 업데이트하지 않음

	SetPinalPos(GetPos());

	if (GetParent())
	{
		//부모의 파이널 포즈를 불러와서 현재 포지션과 더해줌
		tVec2 vParentPos = GetParent()->GetPinalPos();
		SetPinalPos(GetPos() + vParentPos);
	}
	LateUpdateChild();
}
