#include "pch.h"
#include "Define.h"
#include "ColliderMgr.h"
#include "SceneMgr.h"
#include "Calculation.h"
#include "Object.h"
#include "Player.h"
#include "Func.h"
#include "ScrollMgr.h"
#include "Tile.h"
#include "Monster.h"
#include "EffectMgr.h"
#include "EffectFactory.h"
#include "Define.h"
#include "UIMgr.h"
#include "SoundMgr.h"
#include "Effect.h"
#include "DH_CPlayer.h"
#include "KL_CPlayer.h"
#include "CJumpPad.h"

CColliderMgr::CColliderMgr() : m_Gravity(9.8f), m_fTime(0), m_bEffectCreated(false), m_bPlayerDead(false),
m_bJumpLimit(false), m_bSound(false), m_SoundStart(0), m_bCollisionCooldown(true)
{

}
CColliderMgr::~CColliderMgr()
{
}

void CColliderMgr::Update()
{
	auto& player = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::PLAYER];
	auto& monsters = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::MONSTER];
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	auto& PlayerAttackBox = CEffectMgr::GetInst()->GetVecEffect()[(int)eEffect::PLAYERATTACK];
	auto& MonsterAttackBox = CEffectMgr::GetInst()->GetVecEffect()[(int)eEffect::MONSTERATTACK];

}


bool CColliderMgr::Check_Rect(CObject* _player, CObject* _mob, float* _fX, float* _fY)
{
	float fX = abs(_player->GetPos().fX - _mob->GetPos().fX);
	float fY = abs(_player->GetPos().fY - _mob->GetPos().fY);

	float fScaleX = (_player->GetScale().fX + _mob->GetScale().fX) * 0.5f;
	float fScaleY = (_player->GetScale().fY + _mob->GetScale().fY) * 0.5f;

	if (fScaleX > fX && fScaleY > fY)
	{
		*_fX = fScaleX - fX;
		*_fY = fScaleY - fY;
		return true;
	}
	return false;
}
void CColliderMgr::CollisionEx(vector<CObject*> _Dst, vector<CObject*> _Src)
{
	float	fX(0.f), fY(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY)
				{
					if (Dst->GetPos().fY - Dst->GetScale().fY / 2 <= Src->GetPos().fY + Src->GetScale().fY / 2 &&
						Dst->GetPos().fY - Dst->GetScale().fY / 2 > Src->GetPos().fY - Src->GetScale().fY / 2)
					{
					}

					else
					{
						Dst->SetPos(tVec2{
							Dst->GetPos().fX,
							Src->GetPos().fY - (Src->GetScale().fY / 2) - (Dst->GetScale().fY / 2)
							});
						return;
					}
				}

				else
				{
					if (Dst->GetPos().fX < Src->GetPos().fX)
					{
						//Dst->AddPos(tVec2{ -fX, 0 });
					}
					else
					{
						//Dst->AddPos(tVec2{ fX, 0 });
					}
				}
			}
		}
	}
};

bool CColliderMgr::KL_CheckCircle(CObject* _Src, CObject* _Dst)
{

	D3DXVECTOR3 temp = _Dst->GetInfo().vPos - _Src->GetInfo().vPos;

	float fRadius = 115.f; // 반지름 합

	float fDiagonal = D3DXVec3Length(&temp); // 두 중점 거리

	return fRadius >= fDiagonal;
}
void CColliderMgr::KL_CollisionCircle(CObject* _Src, vector<CObject*> _Dst)
{
	if (static_cast<KL_CPlayer*>(_Src)->GetbJump())
		return;


	static_cast<KL_CPlayer*>(_Src)->SetTarget(nullptr);
	for (auto& Dst : _Dst)
	{
		if (KL_CheckCircle(_Src, Dst))
		{
			(_Src)->SetRotateSpeed(Dst->GetfRotateSpeed());

			static_cast<KL_CPlayer*>(_Src)->SetTarget(Dst);
			
			return;
		}
	}
	
	CSceneMgr::GetInst()->GetCurScene()->DeletePlayerGroup();
	
}


bool CColliderMgr::CollisionCircle(CObject* lhs, CObject* rhs, float _ScrollX, float _ScrollY)
{
	float radiusObj = lhs->GetScale().fX * 0.5f;
	float radiusObj1 = rhs->GetScale().fY * 0.5f;

	float collisionDis = radiusObj + radiusObj1;// 두 오브젝트의 반지름 더한 값(충돌 기준 거리)

	float x1 = lhs->GetPos().fX + _ScrollX, x2 = rhs->GetPos().fX + _ScrollX;
	float y1 = lhs->GetPos().fY + _ScrollY, y2 = rhs->GetPos().fY + _ScrollY;

	float dx = y2 - y1;
	float dy = x2 - x1;
	float squareDeltaDis = dx * dx + dy * dy;

	return (collisionDis * collisionDis) >= squareDeltaDis;
}

void CColliderMgr::PlayerGravityEx(vector<CObject*> _ObjectGroup)
{
	const float MAX_GRAVITY = 25.f;

	for (CObject* Object : _ObjectGroup)
	{
		// Falling 
		if (Object->GetbFalling() && !Object->GetbJump() && !Object->GetbDoubleJump())
		{
			if (dynamic_cast<CPlayer*>(Object)->GetbJumpAttack())
			{
				Object->SeteState(STATE::JUMPATTACK);
			}
			
			m_fTime += 0.2f;

			Object->SetfCure2((9.8f * m_fTime * m_fTime) * 0.5f);

			if (Object->GetfCurve2() > MAX_GRAVITY)
				Object->SetfCure2(MAX_GRAVITY);

			if (!dynamic_cast<CPlayer*>(Object)->GetbJumpAttack())
			{
				Object->SeteState(STATE::FALLING);
			}

			Object->AddPos(tVec2{ 0, Object->GetfCurve2() });
		}

		else if (!Object->GetbFalling() && !Object->GetbJump())
		{
			m_fTime = 0.f;
			dynamic_cast<CPlayer*>(Object)->SetbJumpAttack(false);
		}
		if (Object->GetbFalling())
		{
			dynamic_cast<CPlayer*>(Object)->SetbAttack1(false);
			dynamic_cast<CPlayer*>(Object)->SetbAttack2(false);
		}
	}
}
void CColliderMgr::ObjectGravityEx(vector<CObject*> _ObjectGroup)
{
	const float MAX_GRAVITY = 25.f; 

	for (CObject* Object : _ObjectGroup)
	{
		if (Object->GetbFalling())
		{
			Object->AddfTime(0.4f);
			
			Object->SetfCure2((9.8f * Object->GetfTime() * Object->GetfTime()) * 0.5f);
			
			if (Object->GetfCurve2() > MAX_GRAVITY)
				Object->SetfCure2(MAX_GRAVITY);

			Object->AddPos(tVec2{ 0, Object->GetfCurve2()});

			if (Object->GetPos().fY + Object->GetScale().fY / 2 >= Object->GetpCurTile()->GetPos().fY - Object->GetpCurTile()->GetScale().fY / 2)
			{
				Object->SetbFalling(false);
				Object->SetfGravity(3.6f);
			}
		}
		else
		{
			Object->SetfTime(0.f);
		}
	}
}

void CColliderMgr::GetCurObjectTile(vector<CObject*> _ObjectGroup)
{
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	for (CObject* Object : _ObjectGroup)
	{
		for (CObject* Tile : Tiles)
		{
			if (CheckXRange(Tile, Object))
			{
				Object->SetvCurTileList(Tile);
			}
		}
		if ((*Object).GetvCurTileList()->size() > 1)
		{
			SetClosestTile(Object, *Object->GetvCurTileList());
			(*Object).GetvCurTileList()->clear();
		}
		else if ((*Object).GetvCurTileList()->size() == 1)
		{
			Object->SetpCurTile((*Object).GetvCurTileList()->front());
			(*Object).GetvCurTileList()->clear();
		}
	}
}

void CColliderMgr::GravityCondition(vector<CObject*> _ObjectGroup, vector<CObject*> _Tiles)
{
	
	for (auto& Object : _ObjectGroup)
	{
		float playerY = Object->GetPos().fY + Object->GetScale().fY / 2;
		float TileY = Object->GetpCurTile()->GetPos().fY - Object->GetpCurTile()->GetScale().fY / 2;

		if (playerY < TileY)
		{
			if (Object->GetName() == L"Player")
			{
				Object->SetbFalling(true);
			}
			else
				Object->SetbFalling(true);
		}
		else if (playerY >= TileY)
		{
			Object->SetbFalling(false);
			Object->SetbJumpSwitch(false);
		}
	}
	auto& TileList = _Tiles;

	for (auto& Object : _ObjectGroup)
	{
		for (auto it = TileList.begin(); it != prev(TileList.end()); ++it)
		{
			auto& Tile = *it;

			if (Object->GetpCurTile() == *it)
			{
				auto nextIt = next(it);
				if (nextIt != TileList.end())
				{
					auto& nextTile = *nextIt;
					if (Object->GetpCurTile()->GetPos().fY < nextTile->GetPos().fY)
					{
						if (Object->GetPos().fX >= Object->GetpCurTile()->GetPos().fX + Object->GetpCurTile()->GetScale().fX / 2)
						{
							Object->SetbFalling(true);
						}
					}

				}
				bool isGreater = true;
				for (const auto& tile : TileList)
				{
					if (Object->GetpCurTile()->GetPos().fX < tile->GetPos().fX)
					{
						isGreater = false;
						break;
					}
				}

				if (isGreater)
				{
					if (Object->GetPos().fX - Object->GetScale().fX/2 > Object->GetpCurTile()->GetPos().fX + Object->GetpCurTile()->GetScale().fX / 2)
					{
						Object->SetbFalling(true);
					}
				}

				if (it != TileList.begin())
				{
					auto prevIt = prev(it);
					auto& prevTile = *prevIt;
				
					if (Object->GetpCurTile()->GetPos().fY < prevTile->GetPos().fY)
					{
						
						if (Object->GetPos().fX <= Object->GetpCurTile()->GetPos().fX - Object->GetpCurTile()->GetScale().fX / 2)
						{
							Object->SetbFalling(true);
						}
					}
				}
				else if (it == TileList.begin())
				{
					if (Object->GetPos().fX <= Object->GetpCurTile()->GetPos().fX - Object->GetpCurTile()->GetScale().fX / 2)
					{
						Object->SetbFalling(true);
					}
				}
			}
		}
	}
}
void CColliderMgr::GravityCondition(CObject* _ObjectGroup, vector<CObject*> _Tiles)
{

	float playerY = _ObjectGroup->GetPos().fY + _ObjectGroup->GetScale().fY / 2;
	float TileY = _ObjectGroup->GetpCurTile()->GetPos().fY - _ObjectGroup->GetpCurTile()->GetScale().fY / 2;

	if (playerY < TileY)
	{
		_ObjectGroup->SetbFalling(true);
	}
	else if (playerY >= TileY)
	{
		_ObjectGroup->SetbFalling(false);
	}
	auto& TileList = _Tiles;

	for (auto it = TileList.begin(); it != prev(TileList.end()); ++it)
	{
		auto& Tile = *it;

		if (_ObjectGroup->GetpCurTile() == *it)
		{
			auto nextIt = next(it);
			if (nextIt != TileList.end())
			{
				auto& nextTile = *nextIt;
				if (_ObjectGroup->GetpCurTile()->GetPos().fY < nextTile->GetPos().fY)
				{
					if (_ObjectGroup->GetPos().fX >= _ObjectGroup->GetpCurTile()->GetPos().fX + _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
					{
						_ObjectGroup->SetbFalling(true);
					}
				}

			}
			bool isGreater = true;
			for (const auto& tile : TileList)
			{
				if (_ObjectGroup->GetpCurTile()->GetPos().fX < tile->GetPos().fX)
				{
					isGreater = false;
					break; 
				}
			}

			if (isGreater)
			{
				if (_ObjectGroup->GetPos().fX - _ObjectGroup->GetScale().fX / 2 > _ObjectGroup->GetpCurTile()->GetPos().fX + _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
				{
					_ObjectGroup->SetbFalling(true);
				}
			}

			if (it != TileList.begin())
			{
				auto prevIt = prev(it);
				auto& prevTile = *prevIt;
				
				if (_ObjectGroup->GetpCurTile()->GetPos().fY < prevTile->GetPos().fY)
				{
					if (_ObjectGroup->GetPos().fX <= _ObjectGroup->GetpCurTile()->GetPos().fX - _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
					{
						_ObjectGroup->SetbFalling(true);
					}
				}
			}
			else if (it == TileList.begin())
			{
				if (_ObjectGroup->GetPos().fX <= _ObjectGroup->GetpCurTile()->GetPos().fX - _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
				{
					_ObjectGroup->SetbFalling(true);
				}
			}
		}
	}
}

void CColliderMgr::PlayerJump(vector<CObject*> _ObjectGroup)
{
	for (auto& object : _ObjectGroup)
	{
		if (object->GetbJump())
		{
			m_bCollisionCooldown = false;

			object->SetbJumpSwitch(true);

			m_fTime += 0.6f;
			
			object->SetfCurve((object->GetfJumpSpeed() * sinf(90.f) * m_fTime) - (object->GetfGravity() * m_fTime * m_fTime) * 0.5f);
			object->MinusfCurve();

			float maxHeight = CalculateVelocityY(object->GetfJumpSpeed(), object->GetfGravity(), m_fTime);
			if (maxHeight <= -10)
			{
					m_bJumpLimit = true;
			}

			if (m_bJumpLimit)
			{
				m_bCollisionCooldown = true;
				object->SetbFalling(true);
				object->SetfCurve(0.f);
				object->SetbJump(false);
				object->SeteState(STATE::FALLING);
				m_bJumpLimit = false;
			}
			
			if (object->GeteState() != STATE::FALLING)
			{
				object->AddPos(tVec2{ 0, dynamic_cast<CPlayer*>(object)->GetfCurve() });
			}

			if (m_bJumpLimit && object->GetPos().fY + object->GetScale().fY / 2 >= object->GetpCurTile()->GetPos().fY - object->GetpCurTile()->GetScale().fY / 2)
			{
				m_bCollisionCooldown = true;
				object->SetbJumpSwitch(false);
				object->SetbJump(false);
				object->SetfCurve(0.f);
				object->SetfGravity(3.6f);
				object->SeteState(STATE::IDLE);
			}
		}
		if (object->GetbDoubleJump())
		{
			m_fTime += 0.6f;

			object->SetfDoubleCurve((object->GetfJumpSpeed() * sinf(90.f) * m_fTime) - (object->GetfGravity() * m_fTime * m_fTime) * 0.5f);
			object->MinusfDoubleCurve();

			float maxHeight = CalculateVelocityY(object->GetfJumpSpeed(), object->GetfGravity(), m_fTime);

			if (maxHeight <= -10)
			{
				m_bJumpLimit = true;
			}

			if (m_bJumpLimit)
			{
				object->SetfCurve(0.f);
				object->SetbJump(false);
				object->SetbDoubleJump(false);
				m_bJumpLimit = false;
			}
			object->AddPos(tVec2{ 0, object->GetfDoubleCurve() });
			
			if (object->GetPos().fY + object->GetScale().fY / 2 >= object->GetpCurTile()->GetPos().fY - object->GetpCurTile()->GetScale().fY / 2)
			{
				object->SetbJumpSwitch(false);
				object->SetfCurve(0.f);
				object->SetfDoubleCurve(0.f);
				object->SetbDoubleJump(false);
				object->SetfGravity(3.6f);
				object->SeteState(STATE::IDLE);
			}
		}
	}
}
void CColliderMgr::ObjectJump(vector<CObject*> _ObjectGroup)
{
	const float MAX_GRAVITY = 20.f;

	for (auto& object : _ObjectGroup)
	{
		if (object->GetbJump())
		{
			object->AddfTime(0.4f);

			object->SetfCurve((object->GetfJumpSpeed() * sinf(90.f) * object->GetfTime()) - (object->GetfGravity() * object->GetfTime() * object->GetfTime()) * 0.5f);
			object->MinusfCurve();

			if (object->GetfCurve() < 0)
			{
				object->AddfGravity(0.5f);
			}
			else if (object->GetfCurve() > -100)
			{
				object->SetfCurve(MAX_GRAVITY);
			}

			object->AddPos(tVec2{ 0, object->GetfCurve()});

			if (object->GetPos().fY + object->GetScale().fY / 2 >= object->GetpCurTile()->GetPos().fY - object->GetpCurTile()->GetScale().fY / 2)
			{
				object->SetbJump(false);
				object->SetfGravity(3.6f);
			}

		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////

bool CColliderMgr::CheckXRange(CObject* tile, CObject* object)
{
	float left = tile->GetPos().fX - tile->GetScale().fX / 2;
	float right = tile->GetPos().fX + tile->GetScale().fX / 2;
	float objectX = object->GetPos().fX;

	return (left <= objectX && objectX <= right);
}

bool CColliderMgr::TileCompare(CObject* _Player,CObject* _CurrentTile, CObject* _NextTile)
{
	float playerY = _Player->GetPos().fY + _Player->GetScale().fY / 2;
	
	float CurrY = _CurrentTile->GetPos().fY - _CurrentTile->GetScale().fY / 2;
	float NextY = _NextTile->GetPos().fY - _NextTile->GetScale().fY / 2;

	if (abs(playerY - CurrY) <= abs(playerY - NextY))
	{
		return false;
	}
	else if (abs(playerY - CurrY) > abs(playerY - NextY))
	{
		return true;
	}
	return false;
}

void CColliderMgr::SetClosestTile(CObject* object, vector<CObject*> Tiles)
{
	CObject* CurTile = nullptr;

	for (CObject* TileList : Tiles)
	{
		if (CurTile == nullptr || TileCompare(object, CurTile, TileList))
		{
			if(object->GetPos().fY < TileList->GetPos().fY)
				CurTile = TileList; 
		}
	}

	if (CurTile != nullptr)
	{
		object->SetpCurTile(CurTile);
	}
}


float CColliderMgr::CalculateVelocityY(float jumpSpeed, float gravity, float time) 
{
	return jumpSpeed - (gravity * time);
}

void CColliderMgr::Initialize() {}
void CColliderMgr::Release(){}

