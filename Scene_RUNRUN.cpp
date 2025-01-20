#include "pch.h"
#include "Scene_RUNRUN.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "PngManager.h"
#include "MainGame.h"
#include "SoundMgr.h"
#include "HW_CPlayer.h"
#include "HW_CLineMgr.h"
#include "ScrollMgr.h"
#include "HW_CMonster.h"
#include "ColliderMgr.h"
#include "SceneMgr.h"

CScene_RUNRUN::CScene_RUNRUN() : m_pBG2(nullptr), m_iBG1(0), m_iBG2(800)
{
	m_pBG = CResMgr::GetInst()->LoadTexture
	(L"Run", L"./\\Content\\Textures\\BG\\JumpJump.bmp");

	m_pBG2 = CResMgr::GetInst()->LoadTexture
	(L"Run2", L"./\\Content\\Textures\\BG\\JumpJump.bmp");

	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Black.png", L"FadeBlack");

	m_pBlack = CPngManager::GetInst()->Find_Png(L"FadeBlack");
	m_tStartTime = steady_clock::now();
}

CScene_RUNRUN::~CScene_RUNRUN()
{
	HW_CLineMgr::Get_Instance()->Release();
}


void CScene_RUNRUN::Enter()
{
	m_bChangeScene = false;
	m_fFade = 1.0f;

	HW_CLineMgr::Get_Instance()->Initialize();

	for (int i = 0; i < 10; ++i)
	{
		HW_CMonster* pMonster = new HW_CMonster;
		pMonster->Initialize();
		pMonster->SetPosVector(100 * i + 400, 420.f);
		Create_Object(pMonster, eObjectType::MONSTER);
	
	}
	
	HW_CPlayer* pPlayer = new HW_CPlayer;
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);

}

void CScene_RUNRUN::Exit()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

void CScene_RUNRUN::Update()
{
	//음악 서서히 커지기
	if (g_fVolume <= 1.f)
	{
		g_fVolume += 0.15f;
		CSoundMgr::GetInst()->SetChannelVolume(SOUND_BGM, g_fVolume);
	}

	CScene::Update();

	
	if(0 != GetvSceneObj()[(ULONG)eObjectType::MONSTER].size())
		CColliderMgr::GetInst()->HW_CollisionCircle(GetvSceneObj()[(ULONG)eObjectType::PLAYER].front(), GetvSceneObj()[(ULONG)eObjectType::MONSTER].front());


	if (nullptr == CSceneMgr::GetInst()->GetPlayer())
	{
		//CObject* pPlayer = new HW_CPlayer;
		//pPlayer->Initialize();
		//Create_Object(pPlayer, eObjectType::PLAYER);
		//return;
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
		Change_Scene(eSceneType::SCENE_JUMPJUMP);
	}

	//윈도우 종료
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

	#pragma endregion
}

void CScene_RUNRUN::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);
	int BG1 = m_iBG1 + (int)SCROLLX; // 첫 번째 배경 위치
	int BG2 = m_iBG2 + (int)SCROLLX; // 두 번째 배경 위치

	// 첫 번째 배경이 화면 왼쪽으로 사라지면 오른쪽 이동
	if (BG1 <= -800) 
	{
		m_iBG1 = m_iBG2 + 800; // 두 번째 배경 바로 오른쪽으로 이동
	}

	// 두 번째 배경이 화면 왼쪽으로 사라지면 오른쪽 이동
	if (BG2 <= -800) 
	{
		m_iBG2 = m_iBG1 + 800; // 첫 번째 배경 바로 오른쪽으로 이동

	}
	BitBlt(
		g_memDC,
		m_iBG1 + (int)SCROLLX,
		0,
		800,
		600,
		m_pBG->GetDC(),
		0,
		0,
		SRCCOPY
	);
	BitBlt(
		g_memDC,
		m_iBG2 + (int)SCROLLX,
		0,
		800,
		600,
		m_pBG2->GetDC(),
		0,
		0,
		SRCCOPY
	);


	HW_CLineMgr::Get_Instance()->Render();


	CScene::Render();

	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);
}


