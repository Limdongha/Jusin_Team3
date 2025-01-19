#pragma once
#include "UI.h"

class DH_CGage :  public CUI
{
public:
	DH_CGage();
	virtual ~DH_CGage();

public:
	virtual void Render();


public:
	void AlphaBlend
	(Image* _pImage, int _SizeX, int _SizeY, int _iOffsetX, int _iOffsetY, int _CropY, int _Crop, float _alpha);

};

