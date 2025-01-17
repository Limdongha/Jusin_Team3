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

	CObject* pPlayer = new KL_CPlayer;
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);

	for (int i = 0; i < 5; ++i)
	{
		CObject* pJumpPad = new CJumpPad;
		pJumpPad->Initialize();
		pJumpPad->SetPosVector(100 + 150 * i, 300);
		if (i % 2 == 0)
			pJumpPad->SetRotateSpeed(2.f);
		else
			pJumpPad->SetRotateSpeed(-2.f);

		Create_Object(pJumpPad, eObjectType::TILE);
	}

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

	//������ ����
	if (KEY_TAP(KEY::ESC))
	{
		Exit();
		PostQuitMessage(0);
	}

#pragma endregion

}

void CScene_ROUNDROUND::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);

	CScene::Render();

	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);

	
}


