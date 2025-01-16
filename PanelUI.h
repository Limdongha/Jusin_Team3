#pragma once
#include "UI.h"

class CPanelUI : public CUI
{
public:
	CPanelUI();
	virtual ~CPanelUI();

public:
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

public:
	tVec2	m_vDragStart;

};

