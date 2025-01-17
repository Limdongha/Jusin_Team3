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
#include "KL_CPlayer.h"
#include "CJumpPad.h"

CColliderMgr::CColliderMgr() : m_Gravity(9.8f), m_fTime(0), m_bEffectCreated(false), m_bPlayerDead(false),
m_bJumpLimit(false), m_bSound(false), m_SoundStart(0)
{

}
CColliderMgr::~CColliderMgr()
{
}

void CColliderMgr::Update()
{
	//현재씬의 오브젝트 그룹
	auto& player = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::PLAYER];
	auto& monsters = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::MONSTER];
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	auto& PlayerAttackBox = CEffectMgr::GetInst()->GetVecEffect()[(int)eEffect::PLAYERATTACK];
	auto& MonsterAttackBox = CEffectMgr::GetInst()->GetVecEffect()[(int)eEffect::MONSTERATTACK];

}


//렉트EX 밀어내기
bool CColliderMgr::Check_Rect(CObject* _player, CObject* _mob, float* _fX, float* _fY)
{
	//매개변수 : 두 오브젝트의 반대로 가해질 힘값
	
	// 두 물체간의 밑변, 높이
	float fX = abs(_player->GetPos().fX - _mob->GetPos().fX);
	float fY = abs(_player->GetPos().fY - _mob->GetPos().fY);
	// 두 물체의 반지름을 각각 더한 값
	float fScaleX = (_player->GetScale().fX + _mob->GetScale().fX) * 0.5f;
	float fScaleY = (_player->GetScale().fY + _mob->GetScale().fY) * 0.5f;

	// 두 물체가 겹쳤을 때
	if (fScaleX > fX && fScaleY > fY)
	{
		// 밀어내는 힘을 셋팅
		*_fX = fScaleX - fX;
		*_fY = fScaleY - fY;
		return true;
	}
	return false;
}
void CColliderMgr::CollisionEx(vector<CObject*> _Dst, vector<CObject*> _Src)
{
	//두 그룹을 순회 (_Dst 가 밀려나는 그룹)
	float	fX(0.f), fY(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			//겹치는지 확인
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				// 상 하 충돌
				if (fX > fY)
				{
					// 상 충돌
					if (Dst->GetPos().fY + Dst->GetScale().fY / 2 >= Src->GetPos().fY - Src->GetScale().fY / 2 &&
						Dst->GetPos().fY + Dst->GetScale().fY / 2 < Src->GetPos().fY)
					{
						Dst->SetPos(tVec2{
							Dst->GetPos().fX,
							Src->GetPos().fY - (Src->GetScale().fY / 2) - (Dst->GetScale().fY / 2)
							});
						return;
					}
					// 하 충돌
					else
					{
						Dst->AddPos(tVec2{ 0, +fY });
					}
				}

				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (Dst->GetPos().fX < Src->GetPos().fX)
					{
						Dst->AddPos(tVec2{ -fX, 0 });
					}
					// 우 충돌
					else
					{
						Dst->AddPos(tVec2{ fX, 0 });
					}
				}
			}
		}
	}
}

bool CColliderMgr::KL_CheckCircle(CObject* _Src, CObject* _Dst)
{
	D3DXVECTOR3 temp = _Dst->GetInfo().vPos - _Src->GetInfo().vPos;

	float fRadius = 70.f; // 반지름 합

	float fDiagonal = D3DXVec3Length(&temp); // 두 중점 거리

	return fRadius >= fDiagonal;
}

void CColliderMgr::KL_CollisionCircle(CObject* _Src, vector<CObject*> _Dst)
{
	for (auto& Dst : _Dst)
	{
		if (KL_CheckCircle(_Src, Dst))
		{
			(_Src)->SetRotateSpeed(Dst->GetfRotateSpeed());

			static_cast<KL_CPlayer*>(_Src)->SetTarget(Dst);
			
			return;
		}
	}
}


//원충돌
bool CColliderMgr::CollisionCircle(CObject* lhs, CObject* rhs, float _ScrollX, float _ScrollY)
{
	float radiusObj = lhs->GetScale().fX * 0.5f;
	float radiusObj1 = rhs->GetScale().fY * 0.5f;

	float collisionDis = radiusObj + radiusObj1; // 두 오브젝트의 반지름 더한 값(충돌 기준 거리)

	float x1 = lhs->GetPos().fX + _ScrollX, x2 = rhs->GetPos().fX + _ScrollX;
	float y1 = lhs->GetPos().fY + _ScrollY, y2 = rhs->GetPos().fY + _ScrollY;

	// 각 오브젝트의 중점 기준 좌표 거리 계산
	float dx = y2 - y1;
	float dy = x2 - x1;
	float squareDeltaDis = dx * dx + dy * dy;

	return (collisionDis * collisionDis) >= squareDeltaDis;
}

//중력적용 함수
void CColliderMgr::PlayerGravityEx(vector<CObject*> _ObjectGroup)
{
	const float MAX_GRAVITY = 25.f; // 최대 중력 가속도 값 설정

	for (CObject* Object : _ObjectGroup)
	{
		// Falling 할 때만
		if (Object->GetbFalling() && !Object->GetbJump() && !Object->GetbDoubleJump())
		{
			if (dynamic_cast<CPlayer*>(Object)->GetbJumpAttack())
			{
				Object->SeteState(STATE::JUMPATTACK);
			}
			
			m_fTime += 0.2f;

			// 중력 계산
			Object->SetfCure2((9.8f * m_fTime * m_fTime) * 0.5f);

			// 중력 제한 적용
			if (Object->GetfCurve2() > MAX_GRAVITY)
				Object->SetfCure2(MAX_GRAVITY);

			//Falling 조건
			if (!dynamic_cast<CPlayer*>(Object)->GetbJumpAttack())
			{
				Object->SeteState(STATE::FALLING);
			}

			// 중력 적용
			Object->AddPos(tVec2{ 0, Object->GetfCurve2() });
		}

		//점프하는것도 아니고 떨어지는 것도 아닐 때
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
	const float MAX_GRAVITY = 25.f; // 최대 중력 가속도 값 설정

	for (CObject* Object : _ObjectGroup)
	{
		// Falling 할 때만
		if (Object->GetbFalling())
		{
			Object->AddfTime(0.4f);
			
			// 중력 계산
			Object->SetfCure2((9.8f * Object->GetfTime() * Object->GetfTime()) * 0.5f);
			
			// 중력 제한 적용
			if (Object->GetfCurve2() > MAX_GRAVITY)
				Object->SetfCure2(MAX_GRAVITY);

			// 중력 적용
			Object->AddPos(tVec2{ 0, Object->GetfCurve2()});

			if (Object->GetPos().fY + Object->GetScale().fY / 2 >= Object->GetpCurTile()->GetPos().fY - Object->GetpCurTile()->GetScale().fY / 2)
			{
				Object->SetbFalling(false);
				Object->SetfGravity(3.6f);
			}
		}
		//Falling 이 아닐때
		else
		{
			Object->SetfTime(0.f);
		}
	}
}

//커런트 타일 지정
void CColliderMgr::GetCurObjectTile(vector<CObject*> _ObjectGroup)
{
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	//오브젝트 순회
	for (CObject* Object : _ObjectGroup)
	{
		//타일순회
		for (CObject* Tile : Tiles)
		{
			//조건 1.  x의 값이 상자 범위 안에 있을 때 커런트 라인백터 추가
			if (CheckXRange(Tile, Object))
			{
				Object->SetvCurTileList(Tile);
			}
		}
		//타일백터에 1개 이상의 값이 들어오면 제일 가까운 커런트타일로 지정
		if ((*Object).GetvCurTileList()->size() > 1)
		{
			SetClosestTile(Object, *Object->GetvCurTileList());
			(*Object).GetvCurTileList()->clear();
		}
		//타일백터에 1개의 값이 있으면 커런트타일로 지정
		else if ((*Object).GetvCurTileList()->size() == 1)
		{
			Object->SetpCurTile((*Object).GetvCurTileList()->front());
			(*Object).GetvCurTileList()->clear();
		}
	}
}

//Falling 이 켜지는 조건
void CColliderMgr::GravityCondition(vector<CObject*> _ObjectGroup, vector<CObject*> _Tiles)
{
	//여기서는 Falling 을 키고 끄는게 목표
	
	//조건 2.  플레이어의 y값이 발판의 y값보다 작을 때(더 높음) >> 기준을 플레이어 발 좌표기준
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
	//조건 5.  현재 밟고있는 박스의 오른쪽이나 왼쪽에 박스가 없을 때 && 현재 밟고있는 박스 끝에 갔을 때
	auto& TileList = _Tiles;

	for (auto& Object : _ObjectGroup)
	{
		for (auto it = TileList.begin(); it != prev(TileList.end()); ++it)
		{
			auto& Tile = *it;

			//현재 밟고 있는 타일
			if (Object->GetpCurTile() == *it)
			{
				//다음 타일
				auto nextIt = next(it);
				if (nextIt != TileList.end())
				{
					//현재 타일이 다음타일의 y값보다 작을 때
					auto& nextTile = *nextIt;
					if (Object->GetpCurTile()->GetPos().fY < nextTile->GetPos().fY)
					{
						//오브젝트의 x 값이 지금타일의 x보다 크면 낙하
						if (Object->GetPos().fX >= Object->GetpCurTile()->GetPos().fX + Object->GetpCurTile()->GetScale().fX / 2)
						{
							Object->SetbFalling(true);
						}
					}

				}
				//다음 타일이 없을 때
				bool isGreater = true; // 모든 타일보다 큰지 여부
				for (const auto& tile : TileList)
				{
					if (Object->GetpCurTile()->GetPos().fX < tile->GetPos().fX)
					{
						isGreater = false;
						break; // 조건이 만족하지 않으면 중단
					}
				}

				if (isGreater)
				{
					//오브젝트의 x 값이 지금타일의 x보다 크면 낙하
					if (Object->GetPos().fX - Object->GetScale().fX/2 > Object->GetpCurTile()->GetPos().fX + Object->GetpCurTile()->GetScale().fX / 2)
					{
						Object->SetbFalling(true);
					}
				}

				//이전 타일
				if (it != TileList.begin())
				{
					auto prevIt = prev(it);
					auto& prevTile = *prevIt;
					//현재 타일이 다음타일의 y값보다 작을 때
					if (Object->GetpCurTile()->GetPos().fY < prevTile->GetPos().fY)
					{
						//오브젝트의 x 값이 이전타일보다 크면 낙하
						if (Object->GetPos().fX <= Object->GetpCurTile()->GetPos().fX - Object->GetpCurTile()->GetScale().fX / 2)
						{
							Object->SetbFalling(true);
						}
					}
				}
				//이전 타일이 없을 때
				else if (it == TileList.begin())
				{
					//오브젝트의 x 값이 이전타일보다 크면 낙하
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
	//여기서는 Falling 을 키고 끄는게 목표

	//조건 2.  플레이어의 y값이 발판의 y값보다 작을 때(더 높음) >> 기준을 플레이어 발 좌표기준

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
	//조건 5.  현재 밟고있는 박스의 오른쪽이나 왼쪽에 박스가 없을 때 && 현재 밟고있는 박스 끝에 갔을 때
	auto& TileList = _Tiles;

	for (auto it = TileList.begin(); it != prev(TileList.end()); ++it)
	{
		auto& Tile = *it;

		//현재 밟고 있는 타일
		if (_ObjectGroup->GetpCurTile() == *it)
		{
			//다음 타일
			auto nextIt = next(it);
			if (nextIt != TileList.end())
			{
				//현재 타일이 다음타일의 y값보다 작을 때
				auto& nextTile = *nextIt;
				if (_ObjectGroup->GetpCurTile()->GetPos().fY < nextTile->GetPos().fY)
				{
					//오브젝트의 x 값이 지금타일의 x보다 크면 낙하
					if (_ObjectGroup->GetPos().fX >= _ObjectGroup->GetpCurTile()->GetPos().fX + _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
					{
						_ObjectGroup->SetbFalling(true);
					}
				}

			}
			//다음 타일이 없을 때
			bool isGreater = true; // 모든 타일보다 큰지 여부
			for (const auto& tile : TileList)
			{
				if (_ObjectGroup->GetpCurTile()->GetPos().fX < tile->GetPos().fX)
				{
					isGreater = false;
					break; // 조건이 만족하지 않으면 중단
				}
			}

			if (isGreater)
			{
				//오브젝트의 x 값이 지금타일의 x보다 크면 낙하
				if (_ObjectGroup->GetPos().fX - _ObjectGroup->GetScale().fX / 2 > _ObjectGroup->GetpCurTile()->GetPos().fX + _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
				{
					_ObjectGroup->SetbFalling(true);
				}
			}

			//이전 타일
			if (it != TileList.begin())
			{
				auto prevIt = prev(it);
				auto& prevTile = *prevIt;
				//현재 타일이 다음타일의 y값보다 작을 때
				if (_ObjectGroup->GetpCurTile()->GetPos().fY < prevTile->GetPos().fY)
				{
					//오브젝트의 x 값이 이전타일보다 크면 낙하
					if (_ObjectGroup->GetPos().fX <= _ObjectGroup->GetpCurTile()->GetPos().fX - _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
					{
						_ObjectGroup->SetbFalling(true);
					}
				}
			}
			//이전 타일이 없을 때
			else if (it == TileList.begin())
			{
				//오브젝트의 x 값이 이전타일보다 크면 낙하
				if (_ObjectGroup->GetPos().fX <= _ObjectGroup->GetpCurTile()->GetPos().fX - _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
				{
					_ObjectGroup->SetbFalling(true);
				}
			}
		}
	}
}

//점프 구현
void CColliderMgr::PlayerJump(vector<CObject*> _ObjectGroup)
{
	for (auto& object : _ObjectGroup)
	{
		//점프 true
		if (object->GetbJump())
		{
			object->SetbJumpSwitch(true);

			m_fTime += 0.6f;
			
			object->SetfCurve((object->GetfJumpSpeed() * sinf(90.f) * m_fTime) - (object->GetfGravity() * m_fTime * m_fTime) * 0.5f);
			object->MinusfCurve();

			float maxHeight = CalculateVelocityY(object->GetfJumpSpeed(), object->GetfGravity(), m_fTime);
			// 점프 최대값 도달
			if (maxHeight <= -10)
			{
					m_bJumpLimit = true;
			}

			//하강중
			if (m_bJumpLimit)
			{
				object->SetfCurve(0.f);
				object->SetbJump(false);
				object->SeteState(STATE::FALLING);
				m_bJumpLimit = false;
			}
			
			if (object->GeteState() != STATE::FALLING)
			{
				object->AddPos(tVec2{ 0, dynamic_cast<CPlayer*>(object)->GetfCurve() });
			}

			//점프중이고 발판보다 플레이어 위치가 낮을 때
			if (object->GetPos().fY + object->GetScale().fY / 2 >= object->GetpCurTile()->GetPos().fY - object->GetpCurTile()->GetScale().fY / 2)
			{
				object->SetbJumpSwitch(false);
				object->SetbJump(false);
				object->SetfCurve(0.f);
				object->SetfGravity(3.6f);
				object->SeteState(STATE::IDLE);
			}
		}
		//더블점프 true
		if (object->GetbDoubleJump())
		{
			m_fTime += 0.6f;

			object->SetfDoubleCurve((object->GetfJumpSpeed() * sinf(90.f) * m_fTime) - (object->GetfGravity() * m_fTime * m_fTime) * 0.5f);
			object->MinusfDoubleCurve();

			float maxHeight = CalculateVelocityY(object->GetfJumpSpeed(), object->GetfGravity(), m_fTime);
			// 점프 최대값 도달
			if (maxHeight <= -10)
			{
				m_bJumpLimit = true;
			}

			//하강중
			if (m_bJumpLimit)
			{
				object->SetfCurve(0.f);
				object->SetbJump(false);
				object->SetbDoubleJump(false);
				m_bJumpLimit = false;
			}
			object->AddPos(tVec2{ 0, object->GetfDoubleCurve() });
			
			//점프중이고 발판보다 플레이어 위치가 낮을 때
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
		//점프 true
		if (object->GetbJump())
		{
			object->AddfTime(0.4f);

			object->SetfCurve((object->GetfJumpSpeed() * sinf(90.f) * object->GetfTime()) - (object->GetfGravity() * object->GetfTime() * object->GetfTime()) * 0.5f);
			object->MinusfCurve();

			if (object->GetfCurve() < 0)
			{
				object->AddfGravity(0.5f);
			}//점프 상승중
			else if (object->GetfCurve() > -100)
			{
				object->SetfCurve(MAX_GRAVITY);
			}// 점프 하강중

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

//가로범위 안에 있는지 확인하는 함수
bool CColliderMgr::CheckXRange(CObject* tile, CObject* object)
{
	float left = tile->GetPos().fX - tile->GetScale().fX / 2;
	float right = tile->GetPos().fX + tile->GetScale().fX / 2;
	float objectX = object->GetPos().fX;

	return (left <= objectX && objectX <= right);
}

//발판 구분 함수
bool CColliderMgr::TileCompare(CObject* _Player,CObject* _CurrentTile, CObject* _NextTile)
{
	//플레이어보다 더 가까우면 트루로 해야함
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

//가장 가까운 발판 찾기
void CColliderMgr::SetClosestTile(CObject* object, vector<CObject*> Tiles)
{
	CObject* CurTile = nullptr;

	for (CObject* TileList : Tiles)
	{
		//플레이어와의 거리가 더 가까워야함
		if (CurTile == nullptr || TileCompare(object, CurTile, TileList))
		{
			if(object->GetPos().fY < TileList->GetPos().fY)
				CurTile = TileList; // 가장 가까운 타일 갱신
		}
	}

	if (CurTile != nullptr)
	{
		object->SetpCurTile(CurTile); // 최종 발판 설정
	}
}


float CColliderMgr::CalculateVelocityY(float jumpSpeed, float gravity, float time) 
{
	return jumpSpeed - (gravity * time); // Y축 속도 계산
}

//아직 미사용 함수들
void CColliderMgr::Initialize() {}
void CColliderMgr::Release(){}

