#include "pch.h"
#include "EffectFactory.h"
#include "Define.h"
#include "Effect.h"
#include "EffectMgr.h"

CEffect* CEffectFactory::CreateEffect(float _PosfX, float _PosfY, float _ScaleX, float _ScaleY, eEffect _eType, eEffectName _enType, bool _follow, CObject* _obj)
{
	{
		CEffect* EffectBox = new CEffect;
		EffectBox->SetPos(tVec2{ _PosfX, _PosfY });
		EffectBox->SetScale(tVec2{ _ScaleX, _ScaleY });
		EffectBox->SeteEffect(_eType);
		EffectBox->SetCurEffect(_enType);
		EffectBox->SetbFollow(_follow);
		EffectBox->SetpObj(_obj);
		EffectBox->Initialize();
		CEffectMgr::GetInst()->AddVecEffect(EffectBox, _eType);

		return EffectBox;
	}
}
