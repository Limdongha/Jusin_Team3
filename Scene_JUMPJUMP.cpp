#include "pch.h"
#include "Scene_JUMPJUMP.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "PngManager.h"
#include "MainGame.h"
#include "SoundMgr.h"

CScene_JUMPJUMP::CScene_JUMPJUMP()
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Black.png", L"FadeBlack");

	m_pBlack = CPngManager::GetInst()->Find_Png(L"FadeBlack");
	m_tStartTime = steady_clock::now();
}

CScene_JUMPJUMP::~CScene_JUMPJUMP()
{
}


void CScene_JUMPJUMP::Enter()
{
	m_bChangeScene = false;
	m_fFade = 1.0f;
}

void CScene_JUMPJUMP::Exit()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

void CScene_JUMPJUMP::Update()
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
		Change_Scene(eSceneType::SCENE_ROUNDROUND);
	}


	//윈도우 종료
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

#pragma endregion

}

void CScene_JUMPJUMP::Render()
{

	Rectangle(g_memDC, 0, 0, 1280, 720);

	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);
}


