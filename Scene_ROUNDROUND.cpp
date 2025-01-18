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

CScene_ROUNDROUND::CScene_ROUNDROUND() : m_pBG2(nullptr), m_iBG1(0), m_iBG2(800), m_iCountCreatePad(6)
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

CScene_ROUNDROUND::~CScene_ROUNDROUND()
{
}


void CScene_ROUNDROUND::Enter()
{
	m_bChangeScene = false;
	m_fFade = 1.0f;

	int a;

	for (int i = 0; i < 7; ++i)
	{
		CObject* pJumpPad = new CJumpPad;
		pJumpPad->Initialize();
		
		if (i % 2 == 0)
		{
			pJumpPad->SetPosVector(100 + 175 * i, 375);
			pJumpPad->SetRotateSpeed(2.f);
		}
			
		else
		{
			pJumpPad->SetPosVector(100 + 175 * i, 225);
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

	//������ ����
	if (KEY_TAP(KEY::ESC))
	{
		Exit();
		PostQuitMessage(0);
	}

	CScene::LateUpdate();

	DeleteJumpPad();
	CreateJumpPad();
	

#pragma endregion

}

void CScene_ROUNDROUND::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);

	int BG1 = m_iBG1 + (int)SCROLLX; // ù ��° ��� ��ġ
	int BG2 = m_iBG2 + (int)SCROLLX; // �� ��° ��� ��ġ

	// ù ��° ����� ȭ�� �������� ������� ������ �̵�
	if (BG1 <= -800) {
		m_iBG1 = m_iBG2 + 800; // �� ��° ��� �ٷ� ���������� �̵�
	}

	// �� ��° ����� ȭ�� �������� ������� ������ �̵�
	if (BG2 <= -800) {
		m_iBG2 = m_iBG1 + 800; // ù ��° ��� �ٷ� ���������� �̵�
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

	CScene::Render();
	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);

	
}

void CScene_ROUNDROUND::CreateJumpPad()
{
	auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();

	auto& Tile = GetvSceneObj()[(ULONG)eObjectType::TILE];

	if (pPlayer->GetInfo().vPos.x + 300 > Tile.back()->GetInfo().vPos.x)
	{
		++m_iCountCreatePad;
		CObject* pJumpPad = new CJumpPad;
		pJumpPad->Initialize();

		if (m_iCountCreatePad % 2 == 0)
		{
			pJumpPad->SetPosVector(100 + 175 * m_iCountCreatePad, 375);
			pJumpPad->SetRotateSpeed(2.f * m_iCountCreatePad * 0.2f);
		}

		else
		{
			pJumpPad->SetPosVector(100 + 175 * m_iCountCreatePad, 225);
			pJumpPad->SetRotateSpeed(-2.f * m_iCountCreatePad * 0.2f);
		}


		Create_Object(pJumpPad, eObjectType::TILE);
	}
}

void CScene_ROUNDROUND::DeleteJumpPad()
{
	auto pPlayer = GetvSceneObj()[(ULONG)eObjectType::PLAYER].front();

	auto& Tile = GetvSceneObj()[(ULONG)eObjectType::TILE];
	for (auto it = Tile.begin(); it != Tile.end(); ++it )
	{
		//  �÷��̾� ���ʿ� �ִ� �ֵ� ���� �Ÿ� �Ѿ�� ��� ����
		if ((*it)->GetInfo().vPos.x + 300 < pPlayer->GetInfo().vPos.x )
		{
			(*it)->SetbArrive(false);
		}
		
	}
}


