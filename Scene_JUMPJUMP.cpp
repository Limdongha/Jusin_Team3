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


#pragma region ù ���۹���

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

	if (m_bChangeScene && m_fFade == 1.f)
	{
		Change_Scene(eSceneType::SCENE_ROUNDROUND);
	}


	//������ ����
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
			else if (tileY > playerY - 150)
			{
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

			// �ʱ�ȭ �� ���� ����Ʈ�� �߰�
			pTile->Initialize();
			Create_Object(pTile, eObjectType::TILE); // ������Ʈ �߰�

			m_iTileY = (int)pTile->GetPos().fY;

			//����ġ off
			m_bExcuseOne = true;
		}
	}
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