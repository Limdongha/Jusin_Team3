#include "pch.h"
#include "Scene_ROUNDROUND.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "PngManager.h"
#include "MainGame.h"
#include "SoundMgr.h"
#include "KL_CPlayer.h"
#include "CJumpPad.h"
#include "ColliderMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"

CScene_ROUNDROUND::CScene_ROUNDROUND()
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Black.png", L"FadeBlack");

	m_pBlack = CPngManager::GetInst()->Find_Png(L"FadeBlack");
	m_tStartTime = steady_clock::now();
}

CScene_ROUNDROUND::~CScene_ROUNDROUND()
{
}


void CScene_ROUNDROUND::Enter()
{
	m_bChangeScene = false;
	m_fFade = 1.0f;


	for (int i = 0; i < 5; ++i)
	{
		CObject* pJumpPad = new CJumpPad;
		pJumpPad->Initialize();
		
		if (i % 2 == 0)
		{
			pJumpPad->SetPosVector(100 + 175 * i, 450);
			pJumpPad->SetRotateSpeed(2.f);
		}
			
		else
		{
			pJumpPad->SetPosVector(100 + 175 * i, 350);
			pJumpPad->SetRotateSpeed(-2.f);
		}
			

		Create_Object(pJumpPad, eObjectType::TILE);
	}

	CObject* pPlayer = new KL_CPlayer;
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);

}

void CScene_ROUNDROUND::Exit()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

void CScene_ROUNDROUND::Update()
{
	//음악 서서히 커지기
	if (g_fVolume <= 1.f)
	{
		g_fVolume += 0.15f;
		CSoundMgr::GetInst()->SetChannelVolume(SOUND_BGM, g_fVolume);
	}

#pragma region 씬 전환

	//씬 전환
	FadeInOut();

	if (KEY_TAP(KEY::ENTER))
	{
		m_bChangeScene = true;
		CMainGame::GetInst()->SetiEventBox(0);
	}

	if (m_bChangeScene && m_fFade == 1.f)
	{
		Exit();
		PostQuitMessage(0);
		//Change_Scene(eSceneType::SCENE_START);
	}


	CScene::Update();

	CColliderMgr::GetInst()->KL_CollisionCircle(GetvSceneObj()[(ULONG)eObjectType::PLAYER].front(), GetvSceneObj()[(ULONG)eObjectType::TILE]);

	if (nullptr == CSceneMgr::GetInst()->GetPlayer())
	{
		CObject* pPlayer = new KL_CPlayer;
		pPlayer->Initialize();
		Create_Object(pPlayer, eObjectType::PLAYER);
		return;
	}

	//윈도우 종료
	if (KEY_TAP(KEY::ESC))
	{
		Exit();
		PostQuitMessage(0);
	}

	CScene::LateUpdate();

#pragma endregion

}

void CScene_ROUNDROUND::Render()
{

	CScrollMgr::Get_Instance()->Scroll_Lock(3000, 720);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	Rectangle(g_memDC, iScrollX, 0, 800 , 720);

	CScene::Render();

	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);

	
}


