#pragma once
#include "UI.h"
class DH_CGameOver :  public CUI
{
public:
	DH_CGameOver();
	virtual ~DH_CGameOver();

public:
	virtual void Render();


public:
	void AlphaBlend
	(Image* _pImage, int _SizeX, int _SizeY, int _iOffsetX, int _iOffsetY, int _CropY, int _Crop, float _alpha);

	void CreateText(const wchar_t* _LText, int _TextSize, COLORREF _color, int _PosX, int _PosY, int _InputNum);


private:
	CTextures* m_pUI;

};

