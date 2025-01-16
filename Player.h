#pragma once
#include "Object.h"
#include "Define.h"

class CTextures; //전방선언
class CUI;
class CEffect;

class CPlayer :  public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void	Render() override;
	virtual void	Update() override;
	virtual void	Initialize() override;
	virtual void	LateUpdate() override;

public:
	virtual void	KeyInput();
	virtual void	Change_Motion();
	void			Offset();

public:
	const wchar_t*	StateToString(STATE state);

public:

	// GET 함수들 //
	
	bool		GetbJumpAttack()	{ return m_bJumpAttack; }
	int			GetiGold()			{ return m_iGold; }
	bool		GetbInvincible()	{ return m_isInvincible; }
	bool		GetbControl()		{ return m_bControl; }
	bool		GetbPlayerDie()		{ return m_bPlayerDie; }
	bool		GetbSwitch()		{ return m_bSwitch; }

public:

	// SET 함수들 //
	
	void		SetfDiaSpeed() { m_fSpeed *= 0.707f; }
	void		SetfSpeed(float _fSpeed)	{ m_fSpeed = _fSpeed; }
	void		SetbJumpAttack(bool _JA)	{ m_bJumpAttack = _JA; }
	void		SetbAttack1(bool _at1)		{ m_bAttack1 = _at1; }
	void		SetbAttack2(bool _at2)		{ m_bAttack2 = _at2; }
	void		SetiGold(int _iGold)		{ m_iGold = _iGold; }
	void		SetbControl(bool _key)		{ m_bControl = _key; }
	void		SetbPlayerDie(bool _Die)	{ m_bPlayerDie = _Die; }
	void		SetbSwitch(bool _Switch)	{ m_bSwitch = _Switch; }

public:

	// ADD 함수들 //

	void		AddiGold(int _iGold) { m_iGold += _iGold; }

public:
	
	// 플레이어 무적시간 //

	virtual void		invincibility();
	void				ActivateInvincibility();

protected: 

	// 각종 기능들 //

	PLAYERSTATE	m_ePlayerState;
	POINT		m_tPoint;
	float		m_fSpeed;
	float		m_fDistance;
	bool		m_bSwitch;
	bool		m_bControl;
	bool		m_bAttack1;
	bool		m_bAttack2;
	bool		m_bJumpAttack;
	bool		m_bAttackBox;
	bool		m_bPlayerDie;
	bool		m_bWalk;
	int			m_PlayerInfo;
	int			m_iGold;
	bool		m_isInvincible;
	float		m_invincibleTime;
	float		m_invincibleDuration;
	ULONG64		m_StartWalkTime;

protected:

	// 플레이어 BMP 이미지 //

	CTextures* m_pPlayerLeft;
	CTextures* m_pPlayerRight;

};

