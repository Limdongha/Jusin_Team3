#pragma once
#include "Player.h"

class DH_CPlayer :  public CPlayer
{
public:
	DH_CPlayer();
	virtual ~DH_CPlayer();

public:
	virtual void	Render() override;
	virtual void	Update() override;
	virtual void	Initialize() override;
	virtual void	LateUpdate() override;

public:
	virtual void	KeyInput();
	virtual void	Change_Motion();
	void			Offset();
	void			Physics();
	void			JumpNormalVector();

	void			AlphaBlend(Image* _pImage, int _Size, int _CustomSizeY, int _iOffsetX, int _iOffsetY, float _alpha);


public:
	void			UpdateAngle();

public:
	bool			GetbCharge() { return m_bCharge; }

private:
	//로컬 , 월드 좌표
	INFO			m_vPlayerInfo;
	INFO			m_vPosinInfo;
	D3DXVECTOR3		m_vPlayerPoint[8];
	D3DXVECTOR3		m_vPlayerOriginPoint[8];

	D3DXVECTOR3		m_vPosinPoint[3];
	D3DXVECTOR3		m_vPosinOriginPoint[3];


	float			m_fAlngle;
	float			m_fPosinAlngle;
	float			m_DHfSpeed;
	float			m_DHfX;

private:
	int				m_iPointNum;
	float			m_fRadius;
	float			m_fAngleStep;
	float			m_fCenterX;
	float			m_fCenterY;
	float			m_fAngleLimit;
	float			m_fOscillationSpeed;
	float			m_fCurrentTime;
	bool			m_bAngleIncreasing;
	bool			m_bCharge;
};

