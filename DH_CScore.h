#pragma once
#include "UI.h"
class DH_CScore :  public CUI
{
public:
	DH_CScore();
	virtual ~DH_CScore();

public:
	virtual void Render();


public:
	void AlphaBlend
	(Image* _pImage, int _SizeX, int _SizeY, int _iOffsetX, int _iOffsetY, int _CropY, int _Crop, float _alpha);

public:
	void CreateText(int _TextSize, COLORREF _color, int _PosX, int _PosY, int _InputNum);


};

