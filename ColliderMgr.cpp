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
	//������� ������Ʈ �׷�
	auto& player = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::PLAYER];
	auto& monsters = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::MONSTER];
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	auto& PlayerAttackBox = CEffectMgr::GetInst()->GetVecEffect()[(int)eEffect::PLAYERATTACK];
	auto& MonsterAttackBox = CEffectMgr::GetInst()->GetVecEffect()[(int)eEffect::MONSTERATTACK];

}


//��ƮEX �о��
bool CColliderMgr::Check_Rect(CObject* _player, CObject* _mob, float* _fX, float* _fY)
{
	//�Ű����� : �� ������Ʈ�� �ݴ�� ������ ����
	
	// �� ��ü���� �غ�, ����
	float fX = abs(_player->GetPos().fX - _mob->GetPos().fX);
	float fY = abs(_player->GetPos().fY - _mob->GetPos().fY);
	// �� ��ü�� �������� ���� ���� ��
	float fScaleX = (_player->GetScale().fX + _mob->GetScale().fX) * 0.5f;
	float fScaleY = (_player->GetScale().fY + _mob->GetScale().fY) * 0.5f;

	// �� ��ü�� ������ ��
	if (fScaleX > fX && fScaleY > fY)
	{
		// �о�� ���� ����
		*_fX = fScaleX - fX;
		*_fY = fScaleY - fY;
		return true;
	}
	return false;
}
void CColliderMgr::CollisionEx(vector<CObject*> _Dst, vector<CObject*> _Src)
{
	//�� �׷��� ��ȸ (_Dst �� �з����� �׷�)
	float	fX(0.f), fY(0.f);

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			//��ġ���� Ȯ��
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				// �� �� �浹
				if (fX > fY)
				{
					// �� �浹
					if (Dst->GetPos().fY + Dst->GetScale().fY / 2 >= Src->GetPos().fY - Src->GetScale().fY / 2 &&
						Dst->GetPos().fY + Dst->GetScale().fY / 2 < Src->GetPos().fY)
					{
						Dst->SetPos(tVec2{
							Dst->GetPos().fX,
							Src->GetPos().fY - (Src->GetScale().fY / 2) - (Dst->GetScale().fY / 2)
							});
						return;
					}
					// �� �浹
					else
					{
						Dst->AddPos(tVec2{ 0, +fY });
					}
				}

				// �� �� �浹
				else
				{
					// �� �浹
					if (Dst->GetPos().fX < Src->GetPos().fX)
					{
						Dst->AddPos(tVec2{ -fX, 0 });
					}
					// �� �浹
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

	float fRadius = 70.f; // ������ ��

	float fDiagonal = D3DXVec3Length(&temp); // �� ���� �Ÿ�

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


//���浹
bool CColliderMgr::CollisionCircle(CObject* lhs, CObject* rhs, float _ScrollX, float _ScrollY)
{
	float radiusObj = lhs->GetScale().fX * 0.5f;
	float radiusObj1 = rhs->GetScale().fY * 0.5f;

	float collisionDis = radiusObj + radiusObj1; // �� ������Ʈ�� ������ ���� ��(�浹 ���� �Ÿ�)

	float x1 = lhs->GetPos().fX + _ScrollX, x2 = rhs->GetPos().fX + _ScrollX;
	float y1 = lhs->GetPos().fY + _ScrollY, y2 = rhs->GetPos().fY + _ScrollY;

	// �� ������Ʈ�� ���� ���� ��ǥ �Ÿ� ���
	float dx = y2 - y1;
	float dy = x2 - x1;
	float squareDeltaDis = dx * dx + dy * dy;

	return (collisionDis * collisionDis) >= squareDeltaDis;
}

//�߷����� �Լ�
void CColliderMgr::PlayerGravityEx(vector<CObject*> _ObjectGroup)
{
	const float MAX_GRAVITY = 25.f; // �ִ� �߷� ���ӵ� �� ����

	for (CObject* Object : _ObjectGroup)
	{
		// Falling �� ����
		if (Object->GetbFalling() && !Object->GetbJump() && !Object->GetbDoubleJump())
		{
			if (dynamic_cast<CPlayer*>(Object)->GetbJumpAttack())
			{
				Object->SeteState(STATE::JUMPATTACK);
			}
			
			m_fTime += 0.2f;

			// �߷� ���
			Object->SetfCure2((9.8f * m_fTime * m_fTime) * 0.5f);

			// �߷� ���� ����
			if (Object->GetfCurve2() > MAX_GRAVITY)
				Object->SetfCure2(MAX_GRAVITY);

			//Falling ����
			if (!dynamic_cast<CPlayer*>(Object)->GetbJumpAttack())
			{
				Object->SeteState(STATE::FALLING);
			}

			// �߷� ����
			Object->AddPos(tVec2{ 0, Object->GetfCurve2() });
		}

		//�����ϴ°͵� �ƴϰ� �������� �͵� �ƴ� ��
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
	const float MAX_GRAVITY = 25.f; // �ִ� �߷� ���ӵ� �� ����

	for (CObject* Object : _ObjectGroup)
	{
		// Falling �� ����
		if (Object->GetbFalling())
		{
			Object->AddfTime(0.4f);
			
			// �߷� ���
			Object->SetfCure2((9.8f * Object->GetfTime() * Object->GetfTime()) * 0.5f);
			
			// �߷� ���� ����
			if (Object->GetfCurve2() > MAX_GRAVITY)
				Object->SetfCure2(MAX_GRAVITY);

			// �߷� ����
			Object->AddPos(tVec2{ 0, Object->GetfCurve2()});

			if (Object->GetPos().fY + Object->GetScale().fY / 2 >= Object->GetpCurTile()->GetPos().fY - Object->GetpCurTile()->GetScale().fY / 2)
			{
				Object->SetbFalling(false);
				Object->SetfGravity(3.6f);
			}
		}
		//Falling �� �ƴҶ�
		else
		{
			Object->SetfTime(0.f);
		}
	}
}

//Ŀ��Ʈ Ÿ�� ����
void CColliderMgr::GetCurObjectTile(vector<CObject*> _ObjectGroup)
{
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	//������Ʈ ��ȸ
	for (CObject* Object : _ObjectGroup)
	{
		//Ÿ�ϼ�ȸ
		for (CObject* Tile : Tiles)
		{
			//���� 1.  x�� ���� ���� ���� �ȿ� ���� �� Ŀ��Ʈ ���ι��� �߰�
			if (CheckXRange(Tile, Object))
			{
				Object->SetvCurTileList(Tile);
			}
		}
		//Ÿ�Ϲ��Ϳ� 1�� �̻��� ���� ������ ���� ����� Ŀ��ƮŸ�Ϸ� ����
		if ((*Object).GetvCurTileList()->size() > 1)
		{
			SetClosestTile(Object, *Object->GetvCurTileList());
			(*Object).GetvCurTileList()->clear();
		}
		//Ÿ�Ϲ��Ϳ� 1���� ���� ������ Ŀ��ƮŸ�Ϸ� ����
		else if ((*Object).GetvCurTileList()->size() == 1)
		{
			Object->SetpCurTile((*Object).GetvCurTileList()->front());
			(*Object).GetvCurTileList()->clear();
		}
	}
}

//Falling �� ������ ����
void CColliderMgr::GravityCondition(vector<CObject*> _ObjectGroup, vector<CObject*> _Tiles)
{
	//���⼭�� Falling �� Ű�� ���°� ��ǥ
	
	//���� 2.  �÷��̾��� y���� ������ y������ ���� ��(�� ����) >> ������ �÷��̾� �� ��ǥ����
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
	//���� 5.  ���� ����ִ� �ڽ��� �������̳� ���ʿ� �ڽ��� ���� �� && ���� ����ִ� �ڽ� ���� ���� ��
	auto& TileList = _Tiles;

	for (auto& Object : _ObjectGroup)
	{
		for (auto it = TileList.begin(); it != prev(TileList.end()); ++it)
		{
			auto& Tile = *it;

			//���� ��� �ִ� Ÿ��
			if (Object->GetpCurTile() == *it)
			{
				//���� Ÿ��
				auto nextIt = next(it);
				if (nextIt != TileList.end())
				{
					//���� Ÿ���� ����Ÿ���� y������ ���� ��
					auto& nextTile = *nextIt;
					if (Object->GetpCurTile()->GetPos().fY < nextTile->GetPos().fY)
					{
						//������Ʈ�� x ���� ����Ÿ���� x���� ũ�� ����
						if (Object->GetPos().fX >= Object->GetpCurTile()->GetPos().fX + Object->GetpCurTile()->GetScale().fX / 2)
						{
							Object->SetbFalling(true);
						}
					}

				}
				//���� Ÿ���� ���� ��
				bool isGreater = true; // ��� Ÿ�Ϻ��� ū�� ����
				for (const auto& tile : TileList)
				{
					if (Object->GetpCurTile()->GetPos().fX < tile->GetPos().fX)
					{
						isGreater = false;
						break; // ������ �������� ������ �ߴ�
					}
				}

				if (isGreater)
				{
					//������Ʈ�� x ���� ����Ÿ���� x���� ũ�� ����
					if (Object->GetPos().fX - Object->GetScale().fX/2 > Object->GetpCurTile()->GetPos().fX + Object->GetpCurTile()->GetScale().fX / 2)
					{
						Object->SetbFalling(true);
					}
				}

				//���� Ÿ��
				if (it != TileList.begin())
				{
					auto prevIt = prev(it);
					auto& prevTile = *prevIt;
					//���� Ÿ���� ����Ÿ���� y������ ���� ��
					if (Object->GetpCurTile()->GetPos().fY < prevTile->GetPos().fY)
					{
						//������Ʈ�� x ���� ����Ÿ�Ϻ��� ũ�� ����
						if (Object->GetPos().fX <= Object->GetpCurTile()->GetPos().fX - Object->GetpCurTile()->GetScale().fX / 2)
						{
							Object->SetbFalling(true);
						}
					}
				}
				//���� Ÿ���� ���� ��
				else if (it == TileList.begin())
				{
					//������Ʈ�� x ���� ����Ÿ�Ϻ��� ũ�� ����
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
	//���⼭�� Falling �� Ű�� ���°� ��ǥ

	//���� 2.  �÷��̾��� y���� ������ y������ ���� ��(�� ����) >> ������ �÷��̾� �� ��ǥ����

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
	//���� 5.  ���� ����ִ� �ڽ��� �������̳� ���ʿ� �ڽ��� ���� �� && ���� ����ִ� �ڽ� ���� ���� ��
	auto& TileList = _Tiles;

	for (auto it = TileList.begin(); it != prev(TileList.end()); ++it)
	{
		auto& Tile = *it;

		//���� ��� �ִ� Ÿ��
		if (_ObjectGroup->GetpCurTile() == *it)
		{
			//���� Ÿ��
			auto nextIt = next(it);
			if (nextIt != TileList.end())
			{
				//���� Ÿ���� ����Ÿ���� y������ ���� ��
				auto& nextTile = *nextIt;
				if (_ObjectGroup->GetpCurTile()->GetPos().fY < nextTile->GetPos().fY)
				{
					//������Ʈ�� x ���� ����Ÿ���� x���� ũ�� ����
					if (_ObjectGroup->GetPos().fX >= _ObjectGroup->GetpCurTile()->GetPos().fX + _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
					{
						_ObjectGroup->SetbFalling(true);
					}
				}

			}
			//���� Ÿ���� ���� ��
			bool isGreater = true; // ��� Ÿ�Ϻ��� ū�� ����
			for (const auto& tile : TileList)
			{
				if (_ObjectGroup->GetpCurTile()->GetPos().fX < tile->GetPos().fX)
				{
					isGreater = false;
					break; // ������ �������� ������ �ߴ�
				}
			}

			if (isGreater)
			{
				//������Ʈ�� x ���� ����Ÿ���� x���� ũ�� ����
				if (_ObjectGroup->GetPos().fX - _ObjectGroup->GetScale().fX / 2 > _ObjectGroup->GetpCurTile()->GetPos().fX + _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
				{
					_ObjectGroup->SetbFalling(true);
				}
			}

			//���� Ÿ��
			if (it != TileList.begin())
			{
				auto prevIt = prev(it);
				auto& prevTile = *prevIt;
				//���� Ÿ���� ����Ÿ���� y������ ���� ��
				if (_ObjectGroup->GetpCurTile()->GetPos().fY < prevTile->GetPos().fY)
				{
					//������Ʈ�� x ���� ����Ÿ�Ϻ��� ũ�� ����
					if (_ObjectGroup->GetPos().fX <= _ObjectGroup->GetpCurTile()->GetPos().fX - _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
					{
						_ObjectGroup->SetbFalling(true);
					}
				}
			}
			//���� Ÿ���� ���� ��
			else if (it == TileList.begin())
			{
				//������Ʈ�� x ���� ����Ÿ�Ϻ��� ũ�� ����
				if (_ObjectGroup->GetPos().fX <= _ObjectGroup->GetpCurTile()->GetPos().fX - _ObjectGroup->GetpCurTile()->GetScale().fX / 2)
				{
					_ObjectGroup->SetbFalling(true);
				}
			}
		}
	}
}

//���� ����
void CColliderMgr::PlayerJump(vector<CObject*> _ObjectGroup)
{
	for (auto& object : _ObjectGroup)
	{
		//���� true
		if (object->GetbJump())
		{
			object->SetbJumpSwitch(true);

			m_fTime += 0.6f;
			
			object->SetfCurve((object->GetfJumpSpeed() * sinf(90.f) * m_fTime) - (object->GetfGravity() * m_fTime * m_fTime) * 0.5f);
			object->MinusfCurve();

			float maxHeight = CalculateVelocityY(object->GetfJumpSpeed(), object->GetfGravity(), m_fTime);
			// ���� �ִ밪 ����
			if (maxHeight <= -10)
			{
					m_bJumpLimit = true;
			}

			//�ϰ���
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

			//�������̰� ���Ǻ��� �÷��̾� ��ġ�� ���� ��
			if (object->GetPos().fY + object->GetScale().fY / 2 >= object->GetpCurTile()->GetPos().fY - object->GetpCurTile()->GetScale().fY / 2)
			{
				object->SetbJumpSwitch(false);
				object->SetbJump(false);
				object->SetfCurve(0.f);
				object->SetfGravity(3.6f);
				object->SeteState(STATE::IDLE);
			}
		}
		//�������� true
		if (object->GetbDoubleJump())
		{
			m_fTime += 0.6f;

			object->SetfDoubleCurve((object->GetfJumpSpeed() * sinf(90.f) * m_fTime) - (object->GetfGravity() * m_fTime * m_fTime) * 0.5f);
			object->MinusfDoubleCurve();

			float maxHeight = CalculateVelocityY(object->GetfJumpSpeed(), object->GetfGravity(), m_fTime);
			// ���� �ִ밪 ����
			if (maxHeight <= -10)
			{
				m_bJumpLimit = true;
			}

			//�ϰ���
			if (m_bJumpLimit)
			{
				object->SetfCurve(0.f);
				object->SetbJump(false);
				object->SetbDoubleJump(false);
				m_bJumpLimit = false;
			}
			object->AddPos(tVec2{ 0, object->GetfDoubleCurve() });
			
			//�������̰� ���Ǻ��� �÷��̾� ��ġ�� ���� ��
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
		//���� true
		if (object->GetbJump())
		{
			object->AddfTime(0.4f);

			object->SetfCurve((object->GetfJumpSpeed() * sinf(90.f) * object->GetfTime()) - (object->GetfGravity() * object->GetfTime() * object->GetfTime()) * 0.5f);
			object->MinusfCurve();

			if (object->GetfCurve() < 0)
			{
				object->AddfGravity(0.5f);
			}//���� �����
			else if (object->GetfCurve() > -100)
			{
				object->SetfCurve(MAX_GRAVITY);
			}// ���� �ϰ���

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

//���ι��� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
bool CColliderMgr::CheckXRange(CObject* tile, CObject* object)
{
	float left = tile->GetPos().fX - tile->GetScale().fX / 2;
	float right = tile->GetPos().fX + tile->GetScale().fX / 2;
	float objectX = object->GetPos().fX;

	return (left <= objectX && objectX <= right);
}

//���� ���� �Լ�
bool CColliderMgr::TileCompare(CObject* _Player,CObject* _CurrentTile, CObject* _NextTile)
{
	//�÷��̾�� �� ������ Ʈ��� �ؾ���
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

//���� ����� ���� ã��
void CColliderMgr::SetClosestTile(CObject* object, vector<CObject*> Tiles)
{
	CObject* CurTile = nullptr;

	for (CObject* TileList : Tiles)
	{
		//�÷��̾���� �Ÿ��� �� ���������
		if (CurTile == nullptr || TileCompare(object, CurTile, TileList))
		{
			if(object->GetPos().fY < TileList->GetPos().fY)
				CurTile = TileList; // ���� ����� Ÿ�� ����
		}
	}

	if (CurTile != nullptr)
	{
		object->SetpCurTile(CurTile); // ���� ���� ����
	}
}


float CColliderMgr::CalculateVelocityY(float jumpSpeed, float gravity, float time) 
{
	return jumpSpeed - (gravity * time); // Y�� �ӵ� ���
}

//���� �̻�� �Լ���
void CColliderMgr::Initialize() {}
void CColliderMgr::Release(){}

