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
	//렌더 레이어
	for (size_t i = 0; i < (int)RENDERID::RENDER_END; ++i)
	{
		for (auto& pObj : m_vecRenderList[i])
			pObj->Render();

		m_vecRenderList[i].clear();
	}

	//오브젝트 타입 순회
	for (int i = 0; i < (ULONG)eObjectType::END; ++i)
	{
		vector<CObject*>::iterator iter = m_vecSceneObject[i].begin();
		//씬의 오브젝트 순회하면서 데드처리
		for (; iter != m_vecSceneObject[i].end();)
		{
			if ((*iter)->GetbArrive())
			{
				++iter;
			}
			//죽었으니 이레이즈
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
		delete ScenePlayer.front(); // 메모리 해제
		ScenePlayer.front() = nullptr;

		// nullptr 제거
		ScenePlayer.erase(remove(ScenePlayer.begin(), ScenePlayer.end(), nullptr),ScenePlayer.end());
	}
}

void CScene::DeleteUI()
{
	//UI 소멸하기
	auto& UI = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::UI];
	if (UI.size() > 0)
	{
		vector<CObject*>::iterator iter = UI.begin();
		//씬의 오브젝트 순회하면서 데드처리
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


// 해당씬의 모든 오브젝트를 업데이트
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
//해당씬의 모든 오브젝트 레이트업데이트
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

// 매개변수가 있는 함수
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
	//체인지가 꺼져있을 때 (페이드 인) >> 0되는게 목적
	if (!m_bChangeScene)
	{
		//0 보다 같거나 내려가면
		if (0.2f >= m_fFade)
			m_fFade = 0.f;
		else if (0.2f < m_fFade)
			m_fFade -= 0.2f;
	}

	//1보다 클때 , 체인지가 켜지면? (페이드 아웃) >> 1되는게 목적
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
	0.0f, 0.0f, 0.0f, alpha, 0.0f, // Alpha (투명도)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // W
	};

	// ImageAttributes 객체 생성
	ImageAttributes imgAttributes;
	imgAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	Color transparentColorMin(255, 0, 255); // 흰색에 가까운 최소 값
	Color transparentColorMax(255, 0, 255); // 완전한 흰색
	imgAttributes.SetColorKey(transparentColorMin, transparentColorMax, ColorAdjustTypeBitmap);

	// 소스 이미지에서 자를 영역 정의 (x, y, width, height)
	Rect sourceRect(0, 0, 1280, 720);


	Rect destRect(0, 0, 1280, 720);

	// 혼합 모드 및 품질 설정
	graphics.SetCompositingMode(CompositingModeSourceOver); // 알파 블렌딩 활성화
	graphics.SetCompositingQuality(CompositingQualityHighQuality);

	graphics.SetSmoothingMode(SmoothingModeNone);                   // 안티앨리어싱 제거
	graphics.SetTextRenderingHint(TextRenderingHintSystemDefault);  // 기본 텍스트 렌더링

	//보간 금지설정
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	// 자른 이미지를 알파 블렌딩으로 화면에 그리기
	graphics.DrawImage(_pImage, destRect, sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height, UnitPixel, &imgAttributes);
}

void CScene::SpawnPlayer(float _XPos, float YPos)
{
	//플레이어소환
	CPlayer* pPlayer = new CPlayer;
	pPlayer->SetScale(tVec2{ 40 , 40 });
	pPlayer->SetPos(tVec2{ _XPos, YPos });
	pPlayer->SetbControl(true);
	pPlayer->Initialize();
	Create_Object(pPlayer, eObjectType::PLAYER);
}




//void CScene::SpawnKniteNamed(float _XPos, float _YPos)
//{
//	//기사 소환
//	CKniteNamed* pKniteNamed = new CKniteNamed;
//	pKniteNamed->SetScale(tVec2{ 100,130 });
//	pKniteNamed->SetPos(tVec2{ _XPos, _YPos });
//	pKniteNamed->Initialize();
//	Create_Object(pKniteNamed, eObjectType::MONSTER);
//}
//
//void CScene::SpawnArcher(float _XPos, float _YPos)
//{
//	//궁수 소환
//	CArcher* pArcher = new CArcher;
//	pArcher->SetScale(tVec2{ 50,80 });
//	pArcher->SetPos(tVec2{ _XPos, _YPos });
//	pArcher->Initialize();
//	Create_Object(pArcher, eObjectType::MONSTER);
//}
//
//void CScene::SpawnNormalTree(float _XPos, float _YPos)
//{
//	//기본 나무 소환
//	CNormalTree* pNormalTree = new CNormalTree;
//	pNormalTree->SetScale(tVec2{ 60,100 });
//	pNormalTree->SetPos(tVec2{ _XPos, _YPos });
//	pNormalTree->Initialize();
//	Create_Object(pNormalTree, eObjectType::MONSTER);
//}
//
//void CScene::SpawnDealTree(float _XPos, float _YPos)
//{
//	//딜 나무 소환
//	CDealTree* pDealTree = new CDealTree;
//	pDealTree->SetScale(tVec2{ 60, 100 });
//	pDealTree->SetPos(tVec2{ _XPos, _YPos });
//	pDealTree->Initialize();
//	Create_Object(pDealTree, eObjectType::MONSTER);
//}
//
//void CScene::SpawnCherryTree(float _XPos, float _YPos)
//{
//	//딜 나무 소환
//	CCherryTree* pCherryTree = new CCherryTree;
//	pCherryTree->SetScale(tVec2{ 60,100 });
//	pCherryTree->SetPos(tVec2{ _XPos, _YPos });
//	pCherryTree->Initialize();
//	Create_Object(pCherryTree, eObjectType::MONSTER);
//}


