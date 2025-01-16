#include "pch.h"
#include <thread>
#include <string>
#include "Calculation.h"
#include "Define.h"
#include "Player.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "MouseMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "ColliderMgr.h"
#include "ScrollMgr.h"
#include "PngManager.h"
#include "EffectMgr.h"
#include "Effect.h"
#include "EffectFactory.h"
#include "MainGame.h"
#include "SoundMgr.h"
#include "UIMgr.h"
#include "UI.h"
#include "PanelUI.h"
#include "BtnUI.h"


CPlayer::CPlayer() : m_fSpeed(10.f), m_fDistance(20.f), m_tPoint(POINT{}), m_ePlayerState(PLAYERSTATE::HEAD),
m_bAttack1(false), m_bAttack2(false), m_bJumpAttack(false), m_PlayerInfo(1), m_iGold(1432), m_bSwitch(false),
m_bPlayerDie(false), m_pPlayerLeft(nullptr), m_pPlayerRight(nullptr), m_bAttackBox(true), m_isInvincible(false),
m_invincibleTime(0.0f), m_invincibleDuration(3.0f), m_bControl(true),m_StartWalkTime(0), m_bWalk(false)
{
	SetiHP(100);
	SetName(L"Player");

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 150;
	m_tFrame.dwTime = GetTickCount64();

}

CPlayer::~CPlayer()
{
}

//플레이어 그리기
void CPlayer::Render()
{
	if(!m_bPlayerDie)
	{
		int iOffset = -100;
		
		GdiTransparentBlt(
			g_memDC,
			int(GetPos().fX + iOffset + 10 + SCROLLX),
			int(GetPos().fY + iOffset + SCROLLY),
			180,
			180,
			m_pCurTex->GetDC(),
			0 + (180 * m_tFrame.iFrameStart),
			0 + (180 * m_tFrame.iMotion),
			180,
			180,
			RGB(255, 0, 255)
		);

#pragma region 개발모드 출력

		if (CMainGame::GetInst()->GetiEventBox() == 1)
		{
			SetBkMode(g_memDC, TRANSPARENT); // 배경을 투명하게 설정
			SetTextColor(g_memDC, RGB(255, 0, 140)); // 텍스트 색상

			wchar_t scoreText01[50];
			swprintf(scoreText01, 50, L"Attack: %s", m_bAttack1 ? L"true" : L"false");

			wchar_t scoreText02[50];
			swprintf(scoreText02, 50, L"Falling: %s", GetbFalling() ? L"true" : L"false");

			wchar_t scoreText03[50];
			swprintf(scoreText03, 50, L"Jump: %s", GetbJump() ? L"true" : L"false");

			wchar_t scoreText04[50];
			swprintf(scoreText04, 50, L"Player X : %f", GetPos().fX);

			wchar_t scoreText05[50];
			swprintf(scoreText05, 50, L"Player Y : %f", GetPos().fY);

			wchar_t scoreText06[50];
			swprintf(scoreText06, 50, L"Player STATE: %s", StateToString(m_eCurState));

			wchar_t scoreText07[50];
			swprintf(scoreText07, 50, L"Player Direction: %s", GetiDirection() > 0 ? L"Right" : L"Left");

			TextOut(g_memDC,
				WINCX / 2 + 200,
				80,
				scoreText01,
				int(wcslen(scoreText01)));

			TextOut(g_memDC,
				WINCX / 2 + 200,
				100,
				scoreText02,
				int(wcslen(scoreText02)));

			TextOut(g_memDC,
				WINCX / 2 + 200,
				120,
				scoreText03,
				int(wcslen(scoreText03)));

			TextOut(g_memDC,
				WINCX / 2 + 200,
				140,
				scoreText04,
				int(wcslen(scoreText04)));

			TextOut(g_memDC,
				WINCX / 2 + 200,
				160,
				scoreText05,
				int(wcslen(scoreText05)));

			TextOut(g_memDC,
				WINCX / 2 + 200,
				180,
				scoreText06,
				int(wcslen(scoreText06)));

			TextOut(g_memDC,
				WINCX / 2 + 200,
				200,
				scoreText07,
				int(wcslen(scoreText07)));
		}
#pragma endregion

	}

#pragma region 밟은 타일체크

	if (CMainGame::GetInst()->GetiEventBox() == 1)
	{
		int TestX = int(GetpCurTile()->GetPos().fX - GetpCurTile()->GetScale().fX / 2);
		int TestY = int(GetpCurTile()->GetPos().fY - GetpCurTile()->GetScale().fY / 2);


		HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);

		Rectangle(
			g_memDC,
			int(TestX + SCROLLX),
			int(TestY - 5 + SCROLLY),
			int(TestX + TILECX + SCROLLX),
			int(TestY + 5 + SCROLLY));

		SelectObject(g_memDC, oldPen);
		SelectObject(g_memDC, oldBrush);

		DeleteObject(newBrush);
		DeleteObject(newPen);
	}
#pragma endregion

}


//플레이어 업데이트
void CPlayer::Update()
{
	Offset();
	Change_Motion();
	invincibility();


	//발자국 소리
	if (m_eCurState == STATE::WALK)
	{
		ULONG64 m_CurrentWalkTime = GetTickCount64();

		if (!m_bWalk)
		{
			m_StartWalkTime = GetTickCount64();
			m_bWalk = true;
		}
		if (m_CurrentWalkTime - m_StartWalkTime > 600)
		{
			CSoundMgr::GetInst()->StopSound(SOUND_WALK);
			CSoundMgr::GetInst()->PlaySound(L"Skul_Step.wav", SOUND_WALK, 0.3f); 
			m_StartWalkTime = m_CurrentWalkTime;
		}
	}
}

void CPlayer::Initialize()
{
#pragma region 이미지로드

	//플레이어
	m_pPlayerLeft = CResMgr::GetInst()->LoadTexture
	(L"PlayerHeadLeft", L"./\\Content\\Textures\\Player\\NormalSkul\\Skul_Left.bmp");
	m_pPlayerRight = CResMgr::GetInst()->LoadTexture
	(L"PlayerHeadRight", L"./\\Content\\Textures\\Player\\NormalSkul\\Skul_Right.bmp");

	m_pLeftTex = m_pPlayerLeft;
	m_pRightTex = m_pPlayerRight;

	m_pCurTex = m_pRightTex;

#pragma endregion

	m_eCurState = STATE::IDLE;
	m_ePreState = STATE::IDLE;

	m_tFrame.dwTime = GetTickCount64();

	m_eRender = RENDERID::RENDER_PLAYER;

}

void CPlayer::LateUpdate()
{
	//키 설정
	if (m_bControl)
	{
		KeyInput();
	}
	else if (!m_bControl)
	{
		m_eCurState = STATE::IDLE;
	}

	//무브 프레임
	CObject::Move_Frame();

}

//키 입력
void CPlayer::KeyInput()
{
	if (KEY_HOLD(KEY::LEFT))
	{
		m_pCurTex = m_pLeftTex;
		m_pCurPNG = m_pLeftPNG;
		SetiDirection(-1);

		m_eCurState = STATE::WALK;

		AddPos(tVec2{ -m_fSpeed,0 });
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		m_pCurTex = m_pRightTex;
		m_pCurPNG = m_pRightPNG;
		SetiDirection(1);

		m_eCurState = STATE::WALK;

		AddPos(tVec2{ m_fSpeed,0 });
	}
	if (KEY_TAP(KEY::C))
	{
		m_eCurState = STATE::JUMP;

		if (GetbJumpSwitch() && !GetbDoubleJump())
		{
			// 2단 점프 구현 //
			CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
			CSoundMgr::GetInst()->PlaySound(L"Ghoul_Jump2.wav", SOUND_EFFECT, g_fVolume);

			CColliderMgr::GetInst()->SetfTime(0.f);
			CEffectFactory::CreateEffect
			(GetPos().fX, GetPos().fY + 30, 200, 200, eEffect::EFFECT, eEffectName::DOUBLEJUMP, false, this);
			SetfGravity(3.6f);
			SetbDoubleJump(true);
			SetbJump(false);
		}
		else
		{
			// 1단 점프 구현 //
			CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
			CSoundMgr::GetInst()->PlaySound(L"Default_Jump.wav", SOUND_EFFECT, g_fVolume);

			SetbJump(true);
		}
	}
	
	if (!GetbFalling() && !GetbJump())
	{
		m_eCurState = STATE::IDLE;
	}

#pragma endregion
}

//플레이어 모션
void CPlayer::Change_Motion()
{
	//모션인포메이션
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 6;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::ATTACK2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 7;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::DASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::JUMPATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 8;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::SKILL1:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 9;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::SKILL2:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::CHANGE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 10;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE::FALLING:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 5;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

//플레이어 스크롤
void CPlayer::Offset()
{
	int		iOffSetminX = 200;
	int		iOffSetmaxX = 400;
	int		iOffSetminY = 350;
	int		iOffSetmaxY = 500;

	if (!CScrollMgr::Get_Instance()->GetbShake01())
	{
		int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

		if (iOffSetminX > GetPos().fX + iScrollX)
			CScrollMgr::Get_Instance()->Add_ScrollX(m_fSpeed);

		if (iOffSetmaxX < GetPos().fX + iScrollX)
			CScrollMgr::Get_Instance()->Add_ScrollX(-m_fSpeed);

		int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		if (iOffSetminY > GetPos().fY + iScrollY)
			CScrollMgr::Get_Instance()->Add_ScrollY(m_fSpeed);

		if (iOffSetmaxY < GetPos().fY + iScrollY)
			CScrollMgr::Get_Instance()->Add_ScrollY(-m_fSpeed);
	}
}

//상태를 텍스트로 알려주는 함수
const wchar_t* CPlayer::StateToString(STATE state)
{
	switch (state)
	{
	case STATE::IDLE:		return L"IDLE";
	case STATE::WALK:		return L"WALK";
	case STATE::FALLING:	return L"FALLING";
	case STATE::JUMP:		return L"JUMP";
	case STATE::JUMPATTACK: return L"JUMPATTACK";
	case STATE::ATTACK:		return L"ATTACK";
	case STATE::ATTACK2:	return L"ATTACK2";
	case STATE::ATTACK3:	return L"ATTACK3";
	case STATE::SKILL1:		return L"SKILL1";
	case STATE::SKILL2:		return L"SKILL2";
	case STATE::CHANGE:		return L"CHANGE";
	case STATE::HIT:		return L"HIT";
	case STATE::DEAD:		return L"DEAD";
	case STATE::DASH:		return L"DASH";

	default:                  return L"UNKNOWN";
	}
}

//플레이어 무적시간
void CPlayer::invincibility()
{
	if (m_isInvincible)
	{
		m_invincibleTime -= 0.1f;
		if (m_invincibleTime <= 0.0f)
		{
			m_isInvincible = false; // 무적 해제
			m_invincibleTime = 0.0f;
		}
	}
}
void CPlayer::ActivateInvincibility()
{
	m_isInvincible = true;
	m_invincibleTime = m_invincibleDuration;
}
