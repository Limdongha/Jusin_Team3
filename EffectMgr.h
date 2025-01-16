#pragma once
class CEffect;

class CEffectMgr
{
	SINGLE(CEffectMgr)

public:
	void		Update();
	void		Initialize();
	void		Release();
	void		Render();
	void		LateUpdate();

public:
	vector<CEffect*>*	GetVecEffect()			{ return m_vecEffect; }

public:
	void				AddVecEffect(CEffect* _Effect, eEffect _eType) 
						{ m_vecEffect[(int)_eType].push_back(_Effect); }

private:
	vector<CEffect*>	m_vecEffect[(int)eEffect::END];
	int					m_iCollision;
};

