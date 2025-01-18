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


CScene_RUNRUN::CScene_RUNRUN()
{
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

	HW_CPlayer* pPlayer = new HW_CPlayer;
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);

	HW_CLineMgr::Get_Instance()->Initialize();

}

void CScene_RUNRUN::Exit()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

void CScene_RUNRUN::Update()
{
	//���� ������ Ŀ����
	if (g_fVolume <= 1.f)
	{
		g_fVolume += 0.15f;
		CSoundMgr::GetInst()->SetChannelVolume(SOUND_BGM, g_fVolume);
	}

	CScene::Update();
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
		Change_Scene(eSceneType::SCENE_JUMPJUMP);
	}


	//������ ����
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

#pragma endregion

}

void CScene_RUNRUN::Render()
{

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Rectangle(g_memDC, 0, 0, 1280, 720);

	HW_CLineMgr::Get_Instance()->Render();
	CScene::Render();

	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);
}


