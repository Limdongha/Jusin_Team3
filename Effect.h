#pragma once
#include "Object.h"

class CEffect : public CObject
{
public:
	CEffect();
	virtual ~CEffect();

public:
	virtual void Render() override;
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void LateUpdate() override;
	virtual void Update() override;

public:
	//이미지 편집
	void		AlphaBlend(int iScaleX, int iScaleY, float fAlpha, Color _ColorA, Color _ColorB);
	void		Change_Motion();

public:
	const wstring EffectNameToString(eEffectName state);

public:
	bool		GetbDelete() { return m_bDelete; }
	void		SetbDelete(bool _del) { m_bDelete = _del; }
	eEffect		GeteEffect() { return m_eEffect; }
	void		SeteEffect(eEffect _eType) { m_eEffect = _eType; }
	FRAME		GettFrame() { return m_tFrame; }
	void		SetbFollow(bool _follow) { m_bFollow = _follow; }
	bool		GetbFollow() { return m_bFollow; }
	void		SetpObj(CObject* _obj) { m_pObj = _obj; }
	bool		GetbRect() { return m_bRect; }
	void		SetbRect(bool _Rect) { m_bRect = _Rect; }
	void		SetbOptimized(bool _m_bOptimized) { m_bOptimized = _m_bOptimized; }
	void		SetbOptimized2(bool _m_bOptimized) { m_bOptimized2 = _m_bOptimized; }

private:
	ULONGLONG	m_currentTime;
	ULONGLONG	m_prevTime;
	eEffect		m_eEffect;
	bool		m_bDelete;
	bool		m_bFollow;
	bool		m_bRect;
	bool		m_bOptimized;
	bool		m_bOptimized2;
	bool		m_bFrameSwitch;
	bool		m_bExcuseOne;
	int			m_iFrame;
	int			m_iCollision;
	float		m_fAlpha;
	CObject*	m_pObj;
};

