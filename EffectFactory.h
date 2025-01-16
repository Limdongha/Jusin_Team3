#pragma once

class CEffect;

class CEffectFactory
{
public:
	CEffectFactory() {}
	~CEffectFactory() {};

public:
	static CEffect* CreateEffect(float _PosfX, float _PosfY, float _ScaleX, float _ScaleY, eEffect _eType, eEffectName _enType, bool _follow, CObject* _obj);
};