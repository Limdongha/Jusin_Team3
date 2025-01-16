#include "pch.h"
#include "Effect.h"
#include "EffectMgr.h"

CEffectMgr::CEffectMgr() : m_iCollision(1)
{

}
CEffectMgr::~CEffectMgr()
{
	Release();
}

void CEffectMgr::Update()
{
	for (int i = 0; i < (int)eEffect::END; i++)
	{
		for (const auto& Effect : m_vecEffect[i])
		{
			if (Effect)
			{
				Effect->Update();
			}
		}
	}


	auto& PlayerAttack = m_vecEffect[(int)eEffect::PLAYERATTACK];
	//플레이어 어택 제거
	if (PlayerAttack.size() > 0)
	{
		for (auto it = PlayerAttack.begin(); it != PlayerAttack.end();)
		{
			if ((*it)->GetbDelete())
			{
				Delete_Object(*it);
				it = PlayerAttack.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	//몬스터 어택 제거
	auto& MonsterAttack = m_vecEffect[(int)eEffect::MONSTERATTACK];
	if (MonsterAttack.size() > 0)
	{
		for (auto it = MonsterAttack.begin(); it != MonsterAttack.end();)
		{
			if ((*it)->GetbDelete())
			{
				Delete_Object(*it);
				it = MonsterAttack.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	//이펙트 제거
	auto& Effect = m_vecEffect[(int)eEffect::EFFECT];
	if (Effect.size() > 0)
	{
		for (auto it = Effect.begin(); it != Effect.end();)
		{
			if ((*it)->GettFrame().iFrameStart + 1 >= (*it)->GettFrame().iFrameEnd)
			{
				Delete_Object(*it);
				it = Effect.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	
}

void CEffectMgr::Initialize()
{
}

void CEffectMgr::Release()
{
	//이펙트 제거
	auto& Effect = m_vecEffect[(int)eEffect::EFFECT];
	if (Effect.size() > 0)
	{
		for (auto it = Effect.begin(); it != Effect.end();)
		{
			delete *it;
			it = Effect.erase(it);
		}
	}

	//몬스터 어택 제거
	auto& MonsterAttack = m_vecEffect[(int)eEffect::MONSTERATTACK];
	if (MonsterAttack.size() > 0)
	{
		for (auto it = MonsterAttack.begin(); it != MonsterAttack.end();)
		{
			delete *it;
			it = MonsterAttack.erase(it);
		}
	}
}

void CEffectMgr::Render()
{
	for (int i = 0; i < (int)eEffect::END; i++)
	{
		for (const auto& Effect : m_vecEffect[i])
		{
			if (Effect)
			{
				Effect->Render();
			}
		}
	}
}

void CEffectMgr::LateUpdate()
{
	for (int i = 0; i < (int)eEffect::END; i++)
	{
		for (const auto& Effect : m_vecEffect[i])
		{
			if (Effect)
			{
				Effect->LateUpdate();
			}
		}
	}
}
