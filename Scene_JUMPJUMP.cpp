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
#include "DH_CGameOver.h"
#include "DH_CScore.h"
#include "SoundMgr.h"


CScene_JUMPJUMP::CScene_JUMPJUMP() : m_iMaxHeight(0), m_iTileY(0), m_bExcuseOne(false), m_iTileNum(0), m_pBG2(nullptr)
, m_iBG1(0), m_iBG2(-600), m_pBG3(nullptr), m_ScrollThreshold(100), m_LastTileScrollY(0), m_bExcuseTwo(false), m_bExcuseThree(false)
, m_bGameOver(false)
{
	m_pBG = CResMgr::GetInst()->LoadTexture
	(L"Jump", L"./\\Content\\Textures\\BG\\JumpJump2.bmp");
	
	m_pBG2 = CResMgr::GetInst()->LoadTexture
	(L"Jump2", L"./\\Content\\Textures\\BG\\JumpJump2.bmp");
	
	m_pBG3 = CResMgr::GetInst()->LoadTexture
	(L"Ground", L"./\\Content\\Textures\\BG\\JumpJumpGround.bmp");
	
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
	CScrollMgr::Get_Instance()->Set_ScrollX(0);
	CScrollMgr::Get_Instance()->Set_ScrollY(0);
	m_iBG1 = 0;
	m_iBG2 = -600;

	g_fVolume = 0;
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
	CSoundMgr::GetInst()->PlayBGM(L"JumpBGSound.wav", g_fVolume);

	m_bChangeScene = false;
	m_fFade = 1.0f;

	DH_CPlayer* pPlayer = new DH_CPlayer;
	pPlayer->SetPos(tVec2{ 400, 300 });
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);
	
	DH_CScore* pScore = new DH_CScore;
	pScore->SetName(L"Score");
	pScore->SetPos(tVec2{ WINCX / 2 - 100 , +20 });
	pScore->SetScale(tVec2{ 390, 97 });
	Create_Object(pScore, eObjectType::UI);

#pragma region ù ���۹���

	for (int i = 0; i < 5; ++i)
	{
		DH_CTile* pTile = new DH_CTile;
		pTile->SetPos(tVec2{ 0 + float(250 * i), 500 });
		pTile->SetScale(tVec2{ 300,50 });
		pTile->Initialize();
		Create_Object(pTile, eObjectType::TILE);
	}

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
	//���� ������ Ŀ����
	if (g_fVolume <= 1.f)
	{
		g_fVolume += 0.15f;
		CSoundMgr::GetInst()->SetChannelVolume(SOUND_BGM, g_fVolume);
	}

#pragma region �� ��ȯ

	//�� ��ȯ
	FadeInOut();

	if (KEY_TAP(KEY::ENTER))
	{
		m_bChangeScene = true;
		CMainGame::GetInst()->SetiEventBox(0);
	}

	if (KEY_TAP(KEY::R))
	{

		Change_Scene(eSceneType::SCENE_JUMPJUMP);

	}

	if (m_bChangeScene && m_fFade == 1.f)
	{
		Change_Scene(eSceneType::SCENE_ROUNDROUND);
	}


	//������ ����
	if (KEY_TAP(KEY::ESC))
	{
		DeleteAll();
		exit(123);
	}

#pragma endregion

	auto& player = GetvSceneObj()[(int)eObjectType::PLAYER];
	if (player.size() > 0)
	{
		auto& JPlayer = GetvSceneObj()[(int)eObjectType::PLAYER].front();
		if (JPlayer->GetPos().fY + SCROLLY < 650)
		{
			CheckAndCreateTiles();
			UpdateMaxHeight();

			CScene::Update();
		}
		else
		{
			if (!m_bExcuseTwo)
			{
				CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
				CSoundMgr::GetInst()->PlaySound(L"Fail.wav", SOUND_EFFECT, g_fVolume);

				DH_CGameOver* pGameOver = new DH_CGameOver;
				pGameOver->SetName(L"GameOver");
				pGameOver->SetPos(tVec2{ WINCX / 2 - 175, WINCY / 2 - 150 });
				pGameOver->SetScale(tVec2{ 500, 500 });
				Create_Object(pGameOver, eObjectType::UI);

				player.front()->SetbArrive(false);

				m_bExcuseTwo = true;
			}
		}
	}
}

void CScene_JUMPJUMP::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);

	int BG1 = m_iBG1 + (int)SCROLLY; // ù ��° ��� ��ġ
	int BG2 = m_iBG2 + (int)SCROLLY; // �� ��° ��� ��ġ

	// ù ��° ����� ȭ�� �Ʒ��� ������� ���� �̵�
	if (BG1 >= 600) {
		m_iBG1 = m_iBG2 - 600; // �� ��° ��� �ٷ� ���� �̵�
	}

	// �� ��° ����� ȭ�� �Ʒ��� ������� ���� �̵�
	if (BG2 >= 600) {
		m_iBG2 = m_iBG1 - 600; // ù ��° ��� �ٷ� ���� �̵�
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
	
	GdiTransparentBlt(
		g_memDC,
		0,
		0 + (int)SCROLLY,
		800,
		600,
		m_pBG3->GetDC(),
		0,
		0,
		800,
		600,
		RGB(255,0,255)
	);

	
	CScene::Render();
	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);
}


void CScene_JUMPJUMP::CreateRandomTile()
{
	for (int i = 1; i < 5; ++i)
	{	
		
		// ������ x ��ǥ�� y ��ǥ ���
		int randomX = rand() % 600;
		int randomY = rand() % 150;

		auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();
		auto Tiles = GetvSceneObj()[(ULONG)eObjectType::TILE];

		int playerY = int(pPlayer->GetPos().fY); // �÷��̾��� ���� y ��ǥ
		int tileY = playerY - 800 + randomY * i;

		// ���� Ÿ�ϰ� ���ο� Ÿ�� Y ��ġ ��
		for (auto& existingTile : Tiles)
		{
			//Ÿ�� ���� vs ������ Ÿ�� Y �� ��
			if (abs(existingTile->GetPos().fY - tileY) < 200)
			{
				// ���� Ÿ�ϰ� Y ������ 150 �̸��̸�
				m_bExcuseOne = true;
				break;
			}
			else
			{
				//���� ������ ���� ����ġ on
				m_bExcuseOne = false;
			}
		}

		// ����ġ ON �� ����
		if (!m_bExcuseOne)
		{			
			DH_CTile* pTile = new DH_CTile;

			// ��ġ�� ũ�� ����
			pTile->SetPos(tVec2{ (float)randomX, (float)tileY });
			pTile->SetScale(tVec2{ 200, 50 });

			if (!m_bExcuseThree)
			{
				pTile->SetbUniqTile(true);
				m_bExcuseThree = true;
			}

			// �ʱ�ȭ �� ���� ����Ʈ�� �߰�
			pTile->Initialize();
			Create_Object(pTile, eObjectType::TILE); // ������Ʈ �߰�

			m_iTileY = (int)pTile->GetPos().fY;

			//����ġ off
			m_bExcuseOne = true;
		}
	}
	m_bExcuseThree = false;
}

void CScene_JUMPJUMP::CheckAndCreateTiles()
{
	m_tCurTime = steady_clock::now();

	if (duration_cast<milliseconds>(m_tCurTime - m_tStartTime) < milliseconds(500))
	{
		// ��ٿ� ���̸� Ÿ�� �������� ����
	}
	else
	{
		// �ְ� ���̺��� ���� �Ÿ� ���� Ÿ�� ����
		auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();
		// �ְ� ���̺��� ���� ���� �� ����
		if (pPlayer->GetPos().fY < m_iMaxHeight)
		{
			CreateRandomTile();
			m_tStartTime = m_tCurTime;
		}
		// ���� ������ ��ũ�� ��ġ���� ���� �Ÿ� �ö���� ��� Ÿ�� ����
		if (SCROLLY > m_LastTileScrollY + m_ScrollThreshold)
		{
			CreateRandomTile();
			m_LastTileScrollY = SCROLLY; // ������ ������ ��ũ�� �� ������Ʈ
			m_tStartTime = m_tCurTime; // ��Ÿ�� �ʱ�ȭ
		}
	}
}

void CScene_JUMPJUMP::UpdateMaxHeight()
{
	auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();
	int playerY = (int)pPlayer->GetPos().fY;
	if (playerY < m_iMaxHeight)
	{
		m_iMaxHeight = playerY; // �ְ� ���̸� ����
	}
}


void CScene_JUMPJUMP::DeleteTile()
{
	// Ÿ�� ������Ʈ �� ����
	auto& Tile = GetvSceneObj()[(ULONG)eObjectType::TILE];
	for (auto it = Tile.begin(); it != Tile.end(); )
	{
		// ȭ�� �Ʒ��� ��� Ÿ�� ����
		if ((*it)->GetPos().fY + SCROLLY > 650.f)
		{
			delete* it;
			it = Tile.erase(it);
		}
		else {
			++it;
		}
	}
}