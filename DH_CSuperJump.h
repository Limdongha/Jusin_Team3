#pragma once
#include "UI.h"
class DH_CSuperJump :  public CUI
{
public:
	DH_CSuperJump();
	virtual ~DH_CSuperJump();

public:
	virtual void Render();
	virtual void Update();


public:
	void AlphaBlend
	(Image* _pImage, int _SizeX, int _SizeY, int _iOffsetX, int _iOffsetY, int _CropY, int _Crop, float _alpha);


public:
	float			GetfDot() { return m_fDot; }

private:
	INFO			m_vPointInfo;
	D3DXVECTOR3		m_vPoint[4];
	D3DXVECTOR3		m_vPointOrigin[4];

	INFO			m_vTargetInfo;
	D3DXVECTOR3		m_vTargetPoint;
	D3DXVECTOR3		m_vTargetOriginPoint;


	float m_fAlngle;
	float m_fTargetAlngle;
	float m_fDot;


};

