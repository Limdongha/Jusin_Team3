#pragma once
#include "PanelUI.h"
class CStoreUI : public CPanelUI
{
public:
	CStoreUI();
	virtual ~CStoreUI();

public:
	virtual void Render();

	void	AlphaBlend(int iScaleX, int iScaleY, float fAlpha, Image* _PNG);


public:
	bool m_bGold;
	bool m_bBuy;

};

