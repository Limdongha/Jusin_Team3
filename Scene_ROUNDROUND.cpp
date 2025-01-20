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
#include "DH_CGameOver.h"
#include "DH_CScore.h"


CScene_ROUNDROUND::CScene_ROUNDROUND() : m_pBG2(nullptr), m_iBG1(0), m_iBG2(1024), m_iCountCreatePad(6), m_bSpawn(false)
{
	m_pBG = CResMgr::GetInst()->LoadTexture
	(L"Round", L"./\\Content\\Textures\\BG\\Round.bmp");

	m_pBG2 = CResMgr::GetInst()->LoadTexture
	(L"Round2", L"./\\Content\\Textures\\BG\\Round.bmp");



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

	CScrollMgr::Get_Instance()->Set_ScrollX(0);
	CScrollMgr::Get_Instance()->Set_ScrollY(0);

	for (int i = 0; i < 7; ++i)
	{
		CObject* pJumpPad = new CJumpPad;
		pJumpPad->Initialize();
		
		if (i % 2 == 0)
		{
			pJumpPad->SetPosVector(float(100 + 175 * i), 450.f);
			pJumpPad->SetRotateSpeed(2.f);
		}
			
		else
		{
			pJumpPad->SetPosVector(float(100 + 175 * i), 350.f);
			pJumpPad->SetRotateSpeed(-2.f);
		}
			

		Create_Object(pJumpPad, eObjectType::TILE);
	}

	CObject* pPlayer = new KL_CPlayer;
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);

	DH_CScore* pScore = new DH_CScore;
	pScore->SetName(L"Score");
	pScore->SetPos(tVec2{ WINCX / 2 - 100 , +20 });
	pScore->SetScale(tVec2{ 390, 97 });
	Create_Object(pScore, eObjectType::UI);

	g_fVolume = 0;
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
	CSoundMgr::GetInst()->PlayBGM(L"RoundBGM.mp3", g_fVolume);
}

void CScene_ROUNDROUND::Exit()
{
	DeleteAll();
	GetvRenderObj()->clear();
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

	//윈도우 종료
	if (KEY_TAP(KEY::ESC))
	{
		Exit();

		exit(123);
	}



	//씬 전환
	FadeInOut();

	if (KEY_TAP(KEY::ENTER))
	{
		m_bChangeScene = true;
		CMainGame::GetInst()->SetiEventBox(0);
	}

	if (KEY_TAP(KEY::R))
	{
		
		Change_Scene(eSceneType::SCENE_ROUNDROUND);
		
	}


	if (m_bChangeScene && m_fFade == 1.f)
	{
		Exit();
		
		exit(123);
		return;
		//Change_Scene(eSceneType::SCENE_START);
	}

	CScene::Update();
	
	if (!CSceneMgr::GetInst()->GetPlayer()->GetbDie())
	{

		CColliderMgr::GetInst()->KL_CollisionCircle(GetvSceneObj()[(ULONG)eObjectType::PLAYER].front(), GetvSceneObj()[(ULONG)eObjectType::TILE]);
		
		CreateJumpPad();
		DeleteJumpPad();
	}
	else
	{
		if (!m_bSpawn)
		{
			DH_CGameOver* pGameOver = new DH_CGameOver;
			pGameOver->SetName(L"GameOver");
			pGameOver->SetPos(tVec2{ WINCX / 2 - 175, WINCY / 2 - 150 });
			pGameOver->SetScale(tVec2{ 500, 500 });
			Create_Object(pGameOver, eObjectType::UI);
			m_bSpawn = true;

			CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
			CSoundMgr::GetInst()->PlaySound(L"RoundFall.mp3", SOUND_EFFECT, 1.f);
			
		}

	}
	
	

	CScene::LateUpdate();


#pragma endregion

}

void CScene_ROUNDROUND::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);

	int BG1 = m_iBG1 + (int)SCROLLX; // 첫 번째 배경 위치
	int BG2 = m_iBG2 + (int)SCROLLX; // 두 번째 배경 위치

	// 첫 번째 배경이 화면 왼쪽으로 사라지면 오른쪽 이동
	if (BG1 <= -1024) {
		m_iBG1 = m_iBG2 + 1024; // 두 번째 배경 바로 오른쪽으로 이동
	}

	// 두 번째 배경이 화면 왼쪽으로 사라지면 오른쪽 이동
	if (BG2 <= -1024) {
		m_iBG2 = m_iBG1 + 1024; // 첫 번째 배경 바로 오른쪽으로 이동
	}


	BitBlt(
		g_memDC,
		m_iBG1 + (int)SCROLLX,
		0,
		1024,
		1024,
		m_pBG->GetDC(),
		0,
		0,
		SRCCOPY
	);
	BitBlt(
		g_memDC,
		m_iBG2 + (int)SCROLLX,
		0,
		1024,
		1024,
		m_pBG2->GetDC(),
		0,
		0,
		SRCCOPY
	);

	CScene::Render();
	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);

	
}

void CScene_ROUNDROUND::CreateJumpPad()
{
	
	if (nullptr == CSceneMgr::GetInst()->GetPlayer())
	{
		return;
	}
		auto& pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();

	auto& Tile = GetvSceneObj()[(ULONG)eObjectType::TILE];


	if (pPlayer->GetInfo().vPos.x + 300 > Tile.back()->GetInfo().vPos.x)
	{
		++m_iCountCreatePad;
		CObject* pJumpPad = new CJumpPad;
		pJumpPad->Initialize();

		if (m_iCountCreatePad % 2 == 0)
		{
			pJumpPad->SetPosVector(100 + 175 * m_iCountCreatePad, 450);
			pJumpPad->SetRotateSpeed(2.f * m_iCountCreatePad * 0.2f);
		}

		else
		{
			pJumpPad->SetPosVector(100 + 175 * m_iCountCreatePad, 350);
			pJumpPad->SetRotateSpeed(-2.f * m_iCountCreatePad * 0.2f);
		}


		Create_Object(pJumpPad, eObjectType::TILE);
	}
}

void CScene_ROUNDROUND::DeleteJumpPad()
{
	if (nullptr == CSceneMgr::GetInst()->GetPlayer())
	{
		return;
	}

	auto& pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();

	auto& Tile = GetvSceneObj()[(ULONG)eObjectType::TILE];
	for (auto it = Tile.begin(); it != Tile.end(); ++it)
	{
		//  플레이어 왼쪽에 있는 애들 일정 거리 넘어가면 계속 삭제
		if ((*it)->GetInfo().vPos.x + 300 < pPlayer->GetInfo().vPos.x)
		{
			(*it)->SetbArrive(false);
		}

	}
	
}


