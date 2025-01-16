#include "pch.h"
#include "Object.h"
#include "Define.h"
#include "Effect.h"
#include "EffectMgr.h"
#include "SceneMgr.h"

CObject::CObject() :
	m_tObjInfo(tObjInfo{ tVec2{100,100},tVec2{100,100} }), m_pCurTex(nullptr),
	m_bArrive(true), m_rRect(RECT{}), m_pLeftTex(nullptr), m_pRightTex(nullptr), m_iDirection(0),
	m_eCurState(STATE::IDLE), m_ePreState(STATE::IDLE), m_tFrame(FRAME{}), m_bFalling(false),
	m_pCurTile(nullptr), m_vCurTileVector(),m_bJump(false), m_fJumpSpeed(16.f), m_fTime(0.f),
	m_pCurPNG(nullptr), m_pLeftPNG(nullptr), m_pRightPNG(nullptr), m_fGravity(3.6f), m_bDoubleJump(false),
	m_ePreEffect(eEffectName::END), m_eCurEffect(eEffectName::END), m_eRender(RENDERID::RENDER_END),
	m_iHP(100), m_fSpeed(3.0f), m_fCusDeltaTime(0.667f), m_fRealDeltaTime(0.01667f), m_iDieTime(4),
	m_bDie(false), m_fCurve(0.f), m_fCurve2(0.f), m_fDoubleCurve(0.f), m_bJumpSwitch(false), m_bMoveFrame(true)
{
}										   

CObject::~CObject()
{
	Release();
}

//오브젝트 그리기
void CObject::Render()
{
}

void CObject::Release()
{
}

void CObject::Move_Frame()
{
	if (m_bMoveFrame)
	{
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

//어택박스 생성
void CObject::CreateAttackBox
(float _PosfX, float _PosfY, float _ScaleX, float _ScaleY, eEffect _eType, eEffectName _enType,
bool _follow, float _fOffsetX, float _fOffsetY, bool _Rect)
{
	CEffect* AttackBox = new CEffect;

	if (GetiDirection() < 0)
		AttackBox->SetPos(tVec2{ _PosfX - _fOffsetX, _PosfY + _fOffsetY });
	if (GetiDirection() > 0)
		AttackBox->SetPos(tVec2{ _PosfX + _fOffsetX, _PosfY + _fOffsetY });

	AttackBox->SetScale(tVec2{ _ScaleX, _ScaleY });
	AttackBox->SeteEffect(_eType);
	AttackBox->SetbFollow(_follow);
	AttackBox->Initialize();
	AttackBox->SetCurEffect(_enType);
	AttackBox->SetbRect(_Rect);
	CEffectMgr::GetInst()->AddVecEffect(AttackBox, _eType);
}

void CObject::LateUpdate() 
{
	Move_Frame();
}




// 아직 미사용 함수들
void CObject::Update(){}
void CObject::Initialize(){}
