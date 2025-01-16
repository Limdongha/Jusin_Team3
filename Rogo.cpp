#include "pch.h"
#include "Rogo.h"
#include "KeyMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "PngManager.h"
#include "MainGame.h"
#include "SoundMgr.h"

CScene_Rogo::CScene_Rogo() : m_Rogo(nullptr), m_Text(nullptr), m_fText(0.f)
{
	m_Rogo = CResMgr::GetInst()->LoadTexture
	(L"Rogo", L"./\\Content\\Textures\\BG\\ROGO.bmp");

	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Black.png", L"FadeBlack");

	m_pBlack = CPngManager::GetInst()->Find_Png(L"FadeBlack");
	m_tStartTime = steady_clock::now();
}

CScene_Rogo::~CScene_Rogo()
{
}


void CScene_Rogo::Enter()
{
	CSoundMgr::GetInst()->PlayBGM(L"Chapter5_Boss_Dead.wav", g_fVolume);
	m_bChangeScene = false;
	m_fFade = 1.0f;
}

void CScene_Rogo::Exit()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

void CScene_Rogo::Update()
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
		Change_Scene(eSceneType::SCENE_START);
	}


	//�������� ����
	if (KEY_TAP(KEY::SPACE))
	{
		CMainGame::GetInst()->SetiEventBox(1);
		Change_Scene(eSceneType::SCENE_TOOL);
	}


	//������ ����
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

#pragma endregion

}

void CScene_Rogo::Render()
{
	BitBlt(
		g_memDC,
		0, 0,
		1280, 720,
		m_Rogo->GetDC(),
		0, 0,
		SRCCOPY
	);
	
	if(0 < m_fFade)
		AlphaBlend(m_pBlack , m_fFade);
}


