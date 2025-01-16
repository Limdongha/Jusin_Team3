#pragma once
#include "UI.h"

class CUIMgr
{
	SINGLE(CUIMgr)

public:
	void Update();
	void Initialize();
	void SetFocusedUI(CUI* _pUI);
	


private:
	CUI* GetTargetedUI(CUI* _pParentUI);
	CUI* GetFocusUI();

public:
	void SetUION(bool on) { m_UION = on; }
	bool GetUION() { return m_UION; }

private:
	CUI* m_FocusedUI;
	bool m_UION;

};

