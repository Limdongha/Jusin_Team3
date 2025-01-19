#include "pch.h"
#include "Scene.h"
#include "Define.h"
#include "Textures.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "PngManager.h"
#include "Effect.h"
#include "EffectMgr.h"
#include "Player.h"
#include "SoundMgr.h"


CScene::CScene() : m_pBG(nullptr), m_tFrame(FRAME{}), m_pBlack(nullptr), m_fFade(0.f), m_bBackToStart(false),
m_bChangeScene(false),m_bExcuteOne(false), m_bExcuteTwo(false), m_bDontCopy(false)
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\Black.png", L"Black");

	m_pBlack = CPngManager::GetInst()->Find_Png(L"Black");
}
CScene::~CScene()
{
}


void CScene::Render()
{
	//���� ���̾�
	for (size_t i = 0; i < (int)RENDERID::RENDER_END; ++i)
	{
		for (auto& pObj : m_vecRenderList[i])
			pObj->Render();

		m_vecRenderList[i].clear();
	}

	//������Ʈ Ÿ�� ��ȸ
	for (int i = 0; i < (ULONG)eObjectType::END; ++i)
	{
		vector<CObject*>::iterator iter = m_vecSceneObject[i].begin();
		//���� ������Ʈ ��ȸ�ϸ鼭 ����ó��
		for (; iter != m_vecSceneObject[i].end();)
		{
			if ((*iter)->GetbArrive())
			{
				++iter;
			}
			//�׾����� �̷�����
			else
			{
				delete *iter;
				iter = m_vecSceneObject[i].erase(iter);
			}
		}
	}
}

void CScene::AddvObject(CObject* _obj, eObjectType _type)
{
	m_vecSceneObject[(ULONG)_type].push_back(_obj);
}

void CScene::DeleteGroup(eObjectType _eTarget)
{
	for (size_t i = 0; i < m_vecSceneObject[(UINT)_eTarget].size(); ++i)
	{
		delete m_vecSceneObject[(UINT)_eTarget][i];
		m_vecSceneObject[(UINT)_eTarget][i] = nullptr;
	}
	m_vecSceneObject[(UINT)_eTarget].clear();
}



void CScene::DeletePlayerGroup()
{
	auto& ScenePlayer = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(UINT)eObjectType::PLAYER];


	if (!ScenePlayer.empty())
	{
		delete ScenePlayer.front(); // �޸� ����
		ScenePlayer.front() = nullptr;

		// nullptr ����
		ScenePlayer.erase(remove(ScenePlayer.begin(), ScenePlayer.end(), nullptr),ScenePlayer.end());
	}
}

void CScene::DeleteUI()
{
	//UI �Ҹ��ϱ�
	auto& UI = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::UI];
	if (UI.size() > 0)
	{
		vector<CObject*>::iterator iter = UI.begin();
		//���� ������Ʈ ��ȸ�ϸ鼭 ����ó��
		for (; iter != UI.end();)
		{
			delete *iter;
			iter = UI.erase(iter);
		}
	}
}

void CScene::DeleteAll()
{
	for (size_t i = 0; i < (UINT)eObjectType::END; ++i)
	{
		DeleteGroup(eObjectType(i));
	}
}


// �ش���� ��� ������Ʈ�� ������Ʈ
void CScene::Update()
{
	for (int i = 0; i < (ULONG)eObjectType::END; ++i)
	{
		for (int j = 0; j < GetvSceneObj()[i].size(); ++j)
		{
			if (GetvSceneObj()[i][j]->GetbArrive())
			{
				GetvSceneObj()[i][j]->Update();
			}
		}
	}
}
//�ش���� ��� ������Ʈ ����Ʈ������Ʈ
void CScene::LateUpdate()
{
	for (int i = 0; i < (ULONG)eObjectType::END; ++i)
	{
		for (int j = 0; j < GetvSceneObj()[i].size(); ++j)
		{
			GetvSceneObj()[i][j]->LateUpdate();

			RENDERID	eID = GetvSceneObj()[i][j]->Get_GroupID();
			m_vecRenderList[(int)eID].push_back(GetvSceneObj()[i][j]);
		}
	}
}

void CScene::Move_Frame()
{
	Move_Frame(m_tFrame);
}

// �Ű������� �ִ� �Լ�
void CScene::Move_Frame(FRAME& frame)
{
	if (frame.dwTime + frame.dwSpeed < GetTickCount64())
	{
		++frame.iFrameStart;

		if (frame.iFrameStart >= frame.iFrameEnd)
			frame.iFrameStart = 0;

		frame.dwTime = GetTickCount64();
	}
}

void CScene::FadeInOut()
{
	//ü������ �������� �� (���̵� ��) >> 0�Ǵ°� ����
	if (!m_bChangeScene)
	{
		//0 ���� ���ų� ��������
		if (0.2f >= m_fFade)
			m_fFade = 0.f;
		else if (0.2f < m_fFade)
			m_fFade -= 0.2f;
	}

	//1���� Ŭ�� , ü������ ������? (���̵� �ƿ�) >> 1�Ǵ°� ����
	else if (m_bChangeScene)
	{
		if (0.8f <= m_fFade)
			m_fFade = 1.f;
		else if(0.8f > m_fFade)
			m_fFade += 0.2f;
	}
}

void CScene::AlphaBlend(Image* _pImage, float _alpha)
{
	Gdiplus::Graphics graphics(g_memDC);
	float alpha = _alpha;

	ColorMatrix colorMatrix = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Red
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Green
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Blue
	0.0f, 0.0f, 0.0f, alpha, 0.0f, // Alpha (����)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // W
	};

	// ImageAttributes ��ü ����
	ImageAttributes imgAttributes;
	imgAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	Color transparentColorMin(255, 0, 255); // ����� ����� �ּ� ��
	Color transparentColorMax(255, 0, 255); // ������ ���
	imgAttributes.SetColorKey(transparentColorMin, transparentColorMax, ColorAdjustTypeBitmap);

	// �ҽ� �̹������� �ڸ� ���� ���� (x, y, width, height)
	Rect sourceRect(0, 0, 1280, 720);


	Rect destRect(0, 0, 1280, 720);

	// ȥ�� ��� �� ǰ�� ����
	graphics.SetCompositingMode(CompositingModeSourceOver); // ���� ���� Ȱ��ȭ
	graphics.SetCompositingQuality(CompositingQualityHighQuality);

	graphics.SetSmoothingMode(SmoothingModeNone);                   // ��Ƽ�ٸ���� ����
	graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);  // �⺻ �ؽ�Ʈ ������

	//���� ��������
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// �ڸ� �̹����� ���� �������� ȭ�鿡 �׸���
	graphics.DrawImage(_pImage, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}

void CScene::SpawnPlayer(float _XPos, float YPos)
{
	//�÷��̾��ȯ
	CPlayer* pPlayer = new CPlayer;
	pPlayer->SetScale(tVec2{ 40 , 40 });
	pPlayer->SetPos(tVec2{ _XPos, YPos });
	pPlayer->SetbControl(true);
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);
}




//void CScene::SpawnKniteNamed(float _XPos, float _YPos)
//{
//	//��� ��ȯ
//	CKniteNamed* pKniteNamed = new CKniteNamed;
//	pKniteNamed->SetScale(tVec2{ 100,130 });
//	pKniteNamed->SetPos(tVec2{ _XPos, _YPos });
//	pKniteNamed->Initialize();
//	Create_Object(pKniteNamed, eObjectType::MONSTER);
//}
//
//void CScene::SpawnArcher(float _XPos, float _YPos)
//{
//	//�ü� ��ȯ
//	CArcher* pArcher = new CArcher;
//	pArcher->SetScale(tVec2{ 50,80 });
//	pArcher->SetPos(tVec2{ _XPos, _YPos });
//	pArcher->Initialize();
//	Create_Object(pArcher, eObjectType::MONSTER);
//}
//
//void CScene::SpawnNormalTree(float _XPos, float _YPos)
//{
//	//�⺻ ���� ��ȯ
//	CNormalTree* pNormalTree = new CNormalTree;
//	pNormalTree->SetScale(tVec2{ 60,100 });
//	pNormalTree->SetPos(tVec2{ _XPos, _YPos });
//	pNormalTree->Initialize();
//	Create_Object(pNormalTree, eObjectType::MONSTER);
//}
//
//void CScene::SpawnDealTree(float _XPos, float _YPos)
//{
//	//�� ���� ��ȯ
//	CDealTree* pDealTree = new CDealTree;
//	pDealTree->SetScale(tVec2{ 60, 100 });
//	pDealTree->SetPos(tVec2{ _XPos, _YPos });
//	pDealTree->Initialize();
//	Create_Object(pDealTree, eObjectType::MONSTER);
//}
//
//void CScene::SpawnCherryTree(float _XPos, float _YPos)
//{
//	//�� ���� ��ȯ
//	CCherryTree* pCherryTree = new CCherryTree;
//	pCherryTree->SetScale(tVec2{ 60,100 });
//	pCherryTree->SetPos(tVec2{ _XPos, _YPos });
//	pCherryTree->Initialize();
//	Create_Object(pCherryTree, eObjectType::MONSTER);
//}


