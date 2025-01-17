#include "pch.h"
#include "Scene_JUMPJUMP.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "PngManager.h"
#include "MainGame.h"
#include "SoundMgr.h"
#include "DH_CPlayer.h"
#include "DH_CTile.h"
#include "ScrollMgr.h"

CScene_JUMPJUMP::CScene_JUMPJUMP() : m_iMaxHeight(0), m_iTileY(0), m_bExcuseOne(false), m_iTileNum(0), m_pBG2(nullptr)
, m_iBG1(0), m_iBG2(-600)
{
	m_pBG = CResMgr::GetInst()->LoadTexture
	(L"Jump", L"./\\Content\\Textures\\BG\\JumpJump.bmp");

	m_pBG2 = CResMgr::GetInst()->LoadTexture
	(L"Jump2", L"./\\Content\\Textures\\BG\\JumpJump.bmp");

	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Black.png", L"FadeBlack");

	m_pBlack = CPngManager::GetInst()->Find_Png(L"FadeBlack");
	m_tStartTime = steady_clock::now();

}

CScene_JUMPJUMP::~CScene_JUMPJUMP()
{
	Exit();
}


void CScene_JUMPJUMP::Enter()
{
	m_bChangeScene = false;
	m_fFade = 1.0f;

	DH_CPlayer* pPlayer = new DH_CPlayer;
	pPlayer->SetPos(tVec2{ 400, 300 });
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);


#pragma region 첫 시작발판

	DH_CTile* pTile = new DH_CTile;
	pTile->SetPos(tVec2{ 400, 500 });
	pTile->SetScale(tVec2{ 700,50 });
	pTile->Initialize();
	Create_Object(pTile, eObjectType::TILE);

	DH_CTile* pTile2 = new DH_CTile;
	pTile2->SetPos(tVec2{ 500, 300 });
	pTile2->SetScale(tVec2{ 200,50 });
	pTile2->Initialize();
	Create_Object(pTile2, eObjectType::TILE);

	DH_CTile* pTile3 = new DH_CTile;
	pTile3->SetPos(tVec2{ 200, 100 });
	pTile3->SetScale(tVec2{ 200,50 });
	pTile3->Initialize();
	Create_Object(pTile3, eObjectType::TILE);

	DH_CTile* pTile4 = new DH_CTile;
	pTile4->SetPos(tVec2{ 350, -100 });
	pTile4->SetScale(tVec2{ 200,50 });
	pTile4->Initialize();
	Create_Object(pTile4, eObjectType::TILE);

#pragma endregion

	
}

void CScene_JUMPJUMP::Exit()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

void CScene_JUMPJUMP::Update()
{
	CheckAndCreateTiles();
	UpdateMaxHeight();

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
		Change_Scene(eSceneType::SCENE_ROUNDROUND);
	}


	//윈도우 종료
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

#pragma endregion

	CScene::Update();
}

void CScene_JUMPJUMP::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);

	int BG1 = m_iBG1 + (int)SCROLLY; // 첫 번째 배경 위치
	int BG2 = m_iBG2 + (int)SCROLLY; // 두 번째 배경 위치

	// 첫 번째 배경이 화면 아래로 사라지면 위로 이동
	if (BG1 >= 600) {
		m_iBG1 = m_iBG2 - 600; // 두 번째 배경 바로 위로 이동
	}

	// 두 번째 배경이 화면 아래로 사라지면 위로 이동
	if (BG2 >= 600) {
		m_iBG2 = m_iBG1 - 600; // 첫 번째 배경 바로 위로 이동
	}


	BitBlt(
		g_memDC,
		0,
		m_iBG1 + (int)SCROLLY,
		800,
		600,
		m_pBG->GetDC(),
		0,
		0,
		SRCCOPY
	);
	BitBlt(
		g_memDC,
		0,
		m_iBG2 + (int)SCROLLY,
		800,
		600,
		m_pBG2->GetDC(),
		0,
		0,
		SRCCOPY
	);

	CScene::Render();
	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);
}


void CScene_JUMPJUMP::CreateRandomTile()
{
	for (int i = 1; i < 5; ++i)
	{	
		
		// 랜덤한 x 좌표와 y 좌표 계산
		int randomX = rand() % 600;
		int randomY = rand() % 150;

		auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();
		auto Tiles = GetvSceneObj()[(ULONG)eObjectType::TILE];

		int playerY = int(pPlayer->GetPos().fY); // 플레이어의 현재 y 좌표
		int tileY = playerY - 800 + randomY * i;

		// 기존 타일과 새로운 타일 Y 위치 비교
		for (auto& existingTile : Tiles)
		{
			//타일 전부 vs 생성할 타일 Y 값 비교
			if (abs(existingTile->GetPos().fY - tileY) < 200)
			{
				// 기존 타일과 Y 간격이 150 미만이면
				m_bExcuseOne = true;
				break;
			}
			else if (tileY > playerY - 150)
			{
				m_bExcuseOne = true;
				break; 
			}
			else
			{
				//조건 만족시 생성 스위치 on
				m_bExcuseOne = false;
			}
		}

		// 스위치 ON 시 생성
		if (!m_bExcuseOne)
		{
			DH_CTile* pTile = new DH_CTile;

			// 위치와 크기 설정
			pTile->SetPos(tVec2{ (float)randomX, (float)tileY });
			pTile->SetScale(tVec2{ 200, 50 });

			// 초기화 및 관리 리스트에 추가
			pTile->Initialize();
			Create_Object(pTile, eObjectType::TILE); // 오브젝트 추가

			m_iTileY = (int)pTile->GetPos().fY;

			//스위치 off
			m_bExcuseOne = true;
		}
	}
}

void CScene_JUMPJUMP::CheckAndCreateTiles()
{
	m_tCurTime = steady_clock::now();

	if (duration_cast<milliseconds>(m_tCurTime - m_tStartTime) < milliseconds(500))
	{
		// 쿨다운 중이면 타일 생성하지 않음
	}
	else
	{
		// 최고 높이보다 일정 거리 위로 타일 생성
		auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();
		// 최고 높이보다 위에 있을 때 생성
		if (pPlayer->GetPos().fY < m_iMaxHeight)
		{
			CreateRandomTile();
			m_tStartTime = m_tCurTime;
		}
	}
}

void CScene_JUMPJUMP::UpdateMaxHeight()
{
	auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();
	int playerY = (int)pPlayer->GetPos().fY;
	if (playerY < m_iMaxHeight)
	{
		m_iMaxHeight = playerY; // 최고 높이를 갱신
	}
}


void CScene_JUMPJUMP::DeleteTile()
{
	// 타일 업데이트 및 삭제
	auto& Tile = GetvSceneObj()[(ULONG)eObjectType::TILE];
	for (auto it = Tile.begin(); it != Tile.end(); )
	{
		// 화면 아래로 벗어난 타일 삭제
		if ((*it)->GetPos().fY > m_iMaxHeight + 400)
		{
			delete* it;
			it = Tile.erase(it);
		}
		else {
			++it;
		}
	}
}