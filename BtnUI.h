#pragma once
#include "UI.h"
class CBtnUI : public CUI
{
public:
	CBtnUI();
	virtual ~CBtnUI();

public:
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

public:
	virtual void LateUpdate();

public:
	void		SetbClicked(bool _Clicked) { m_bClicked = _Clicked; }
	bool		GetbClicked() { return m_bClicked; }

private:
	bool		m_bClicked;
};

