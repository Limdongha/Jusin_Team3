#include "pch.h"
#include "DH_CPlayer.h"
#include "KeyMgr.h"
#include "ColliderMgr.h"
#include "SceneMgr.h"
#include "MainGame.h"
#include "ScrollMgr.h"
#include "DH_CGage.h"
#include "DH_CTile.h"
#include "DH_CSuperJump.h"
#include "ResMgr.h"
#include "Textures.h"
#include "SoundMgr.h"


DH_CPlayer::DH_CPlayer() : m_fAlngle(0), m_DHfSpeed(10), m_fPosinAlngle(0), m_bAngleIncreasing(false)
, m_fAngleLimit(D3DXToRadian(30.0f)), m_fCurrentTime(0), m_fOscillationSpeed(5.f), m_bCharge(false), m_DHfX(0),
m_bExcuseOne(false), m_pSuperJump(nullptr), m_DHFast(0)
{
	ZeroMemory(&m_vPlayerInfo, sizeof(INFO));
	ZeroMemory(&m_vPosinInfo, sizeof(INFO));
	ZeroMemory(&m_vPlayerPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vPlayerOriginPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vPosinPoint, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vPosinOriginPoint, sizeof(D3DXVECTOR3));


	SetfJumpSpeed(0.f);
	m_iPointNum = 7;
	m_fRadius = 50.0f; // 5������ ������
	m_fAngleStep = 2 * D3DX_PI / m_iPointNum; // �� ������ �� ���� (����)
	m_fCenterX = m_vPlayerInfo.vPos.x; // �߽� X ��ǥ
	m_fCenterY = m_vPlayerInfo.vPos.y; // �߽� Y ��ǥ

	m_pCurTex = CResMgr::GetInst()->LoadTexture
	(L"JumpPlayer", L"./\\Content\\Textures\\Player\\Jump.bmp");
}

DH_CPlayer::~DH_CPlayer()
{
	Release();
}


// ������ �� 
// 1. ��� ������Ʈ�� �������� �׸� �Ŀ� �ű��.
// 2. �÷��̾� �̵��� ������ǥ���� ����Ѵ�.
// 3. �浹ó���� ������ǥ���� ����Ѵ�.

void DH_CPlayer::Initialize()
{
	//�÷��̾� ���� �ʱ�ȭ
	m_vPlayerInfo.vLook = { 0.f , -1.f, 0.f };
	m_vPlayerInfo.vDir = { 0.f , 0.f , 0.f };
	//���� ���� �ʱ�ȭ
	m_vPosinInfo.vPos = { 0.f , - 70.f , 0.f };
	m_vPosinInfo.vLook = { 0.f , -1.f, 0.f };
	m_vPosinInfo.vDir = { 0.f , 0.f , 0.f };


	for (int i = 0; i < m_iPointNum; ++i)
	{
		float angle = m_fAngleStep * i; // �� �������� ����
		m_vPlayerPoint[i] =
		{
			m_fCenterX + m_fRadius * cos(angle),
			m_fCenterY + m_fRadius * sin(angle),
			0.0f // Z���� ������� ����
		};
	}
	for (int i = 0; i < m_iPointNum; ++i)
		m_vPlayerOriginPoint[i] = m_vPlayerPoint[i];


	m_vPosinPoint[0] = { m_vPosinInfo.vPos.x , m_vPosinInfo.vPos.y - 15 , 0.f };
	m_vPosinPoint[1] = { m_vPosinInfo.vPos.x - 10, m_vPosinInfo.vPos.y , 0.f };
	m_vPosinPoint[2] = { m_vPosinInfo.vPos.x + 10, m_vPosinInfo.vPos.y , 0.f };

	for (int i = 0; i < 3; ++i)
		m_vPosinOriginPoint[i] = m_vPosinPoint[i];
}

void DH_CPlayer::Update()
{
	//�÷��̾� ȸ�� >> ������ǥ���� ȸ��
	//�÷��̾� ���� >> ���ÿ��� ������, ������ǥ�� ��������.
	m_vPlayerInfo.vPos = { GetPos().fX + SCROLLX, GetPos().fY + SCROLLY, 0.f };
	m_vPosinInfo.vPos  = { GetPos().fX + SCROLLX, GetPos().fY + SCROLLY, 0.f };

	Offset();
	Physics();
	Change_Motion();
	UpdateAngle();
	SuperJump();

	//������ �븻�������� x �̵�
	JumpNormalVector();

	D3DXMATRIX matScale, matRotate, matTransform;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotate, m_fAlngle);
	D3DXMatrixTranslation(&matTransform, m_vPlayerInfo.vPos.x, m_vPlayerInfo.vPos.y, 0.f);

	m_vPlayerInfo.matWorld = matScale * matRotate * matTransform;

	for (int i = 0; i < m_iPointNum; ++i)
	{
		m_vPlayerPoint[i] = m_vPlayerOriginPoint[i];
		D3DXVec3TransformCoord(&m_vPlayerPoint[i], &m_vPlayerPoint[i], &m_vPlayerInfo.matWorld);
	}


	//// ���� /////

	D3DXMATRIX PosinmatScale, PosinmatRotate, PosinmatTransform;

	D3DXMatrixScaling(&PosinmatScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&PosinmatRotate, m_fPosinAlngle);
	D3DXMatrixTranslation(&PosinmatTransform, m_vPosinInfo.vPos.x, m_vPosinInfo.vPos.y, 0.f);

	m_vPosinInfo.matWorld = PosinmatScale * PosinmatRotate * PosinmatTransform;

	for (int i = 0; i < 3; ++i)
	{
		m_vPosinPoint[i] = m_vPosinOriginPoint[i];
		D3DXVec3TransformCoord(&m_vPosinPoint[i], &m_vPosinPoint[i], &m_vPosinInfo.matWorld);
	}
}



void DH_CPlayer::Render()
{	
	DrawPolygonCustom(m_vPlayerPoint, m_iPointNum, RGB(210,180,140));
	DrawPolygonCustom(m_vPosinPoint, 3 , RGB(139, 69, 19));

	for (int i = 0; i < m_iPointNum; ++i)
	{
		Ellipse(g_memDC,
			int(m_vPlayerPoint[i].x - 3.f),
			int(m_vPlayerPoint[i].y - 3.f),
			int(m_vPlayerPoint[i].x + 3.f),
			int(m_vPlayerPoint[i].y + 3.f));
	}

	GdiTransparentBlt(
		g_memDC,
		int(m_vPlayerInfo.vPos.x - GetScale().fX / 2),
		int(m_vPlayerInfo.vPos.y - GetScale().fY / 2),
		100,
		100,
		m_pCurTex->GetDC(),
		0,
		0,
		450,
		450,
		RGB(255, 0, 255)
	);

#pragma region ���߸�� ���

	if (CMainGame::GetInst()->GetiEventBox() == 0)
	{
		SetBkMode(g_memDC, TRANSPARENT); // ����� �����ϰ� ����
		SetTextColor(g_memDC, RGB(255, 0, 140)); // �ؽ�Ʈ ����

		wchar_t scoreText01[50];
		swprintf(scoreText01, 50, L"Attack: %s", m_bAttack1 ? L"true" : L"false");

		wchar_t scoreText02[50];
		swprintf(scoreText02, 50, L"Falling: %s", GetbFalling() ? L"true" : L"false");

		wchar_t scoreText03[50];
		swprintf(scoreText03, 50, L"Jump: %s", GetbJump() ? L"true" : L"false");

		wchar_t scoreText04[50];
		swprintf(scoreText04, 50, L"Player X : %f", GetPos().fX);

		wchar_t scoreText05[50];
		swprintf(scoreText05, 50, L"Player Y : %f", GetPos().fY);

		wchar_t scoreText06[50];
		swprintf(scoreText06, 50, L"Player STATE: %s", StateToString(m_eCurState));

		wchar_t scoreText07[50];
		swprintf(scoreText07, 50, L"Player Direction: %s", GetiDirection() > 0 ? L"Right" : L"Left");

		wchar_t scoreText08[50];
		swprintf(scoreText08, 50, L"Player JumpSpeed: %f", GetfJumpSpeed());

		TextOut(g_memDC,
			WINCX / 2 + 200,
			80,
			scoreText01,
			int(wcslen(scoreText01)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			100,
			scoreText02,
			int(wcslen(scoreText02)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			120,
			scoreText03,
			int(wcslen(scoreText03)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			140,
			scoreText04,
			int(wcslen(scoreText04)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			160,
			scoreText05,
			int(wcslen(scoreText05)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			180,
			scoreText06,
			int(wcslen(scoreText06)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			200,
			scoreText07,
			int(wcslen(scoreText07)));

		TextOut(g_memDC,
			WINCX / 2 + 200,
			220,
			scoreText08,
			int(wcslen(scoreText08)));
	}
#pragma endregion


}

void DH_CPlayer::LateUpdate()
{
	if (GetbJumpSwitch() && m_fPosinAlngle < 0)
		m_fAlngle -= D3DXToRadian(30.f);
	if (GetbJumpSwitch() && m_fPosinAlngle > 0)
		m_fAlngle += D3DXToRadian(30.f);
	if (!GetbJumpSwitch() && m_DHFast == 0)
	{
		if (m_fPosinAlngle > 0)
			m_fAlngle += D3DXToRadian(1.f);
		if (m_fPosinAlngle < 0)
			m_fAlngle -= D3DXToRadian(1.f);
	}


	//Ű ����
	if (m_bControl && !GetbJumpSwitch())
	{
		KeyInput();
	}
	else if (!m_bControl)
	{
		m_eCurState = STATE::IDLE;
	}
	
	//���� ������
	CObject::Move_Frame();

	m_eRender = RENDERID::RENDER_PLAYER;
}

void DH_CPlayer::KeyInput()
{
	
	if (m_eCurState == STATE::IDLE)
	{
		if (KEY_TAP(KEY::SPACE))
		{
			CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
			CSoundMgr::GetInst()->PlaySound(L"Charge.wav", SOUND_EFFECT, g_fVolume);

			DH_CGage* pGage = new DH_CGage;
			pGage->SetName(L"Gage");
			pGage->SetPos(GetPos());
			pGage->SetScale(tVec2{ 500, 500 });
			Create_Object(pGage, eObjectType::UI);

			m_bCharge = true;
			SetfJumpSpeed(0.f);
		}
		if (KEY_HOLD(KEY::SPACE))
		{
			m_DHFast += 0.5;
			m_fAlngle += D3DXToRadian(m_DHFast);

			AddfJumpSpeed(0.5f);
			//�ִ밪 ����
			if (GetfJumpSpeed() > 25.f)
				SetfJumpSpeed(25.f);
		}
		if (KEY_AWAY(KEY::SPACE))
		{
			CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
			CSoundMgr::GetInst()->PlaySound(L"JumpSound.wav", SOUND_EFFECT, g_fVolume);

			auto& UIGroup = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();
			vector<CObject*>::iterator iter = UIGroup.begin();
			for (; iter != UIGroup.end();)
			{
				if ((*iter)->GetName() == L"Gage")
				{
					delete (*iter);
					iter = UIGroup.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			m_DHFast = 0;
			m_bCharge = false;
			m_eCurState = STATE::JUMP;
			SetbJump(true);
		}
	}

	if (!GetbFalling() && !GetbJump())
	{
		m_eCurState = STATE::IDLE;
	}

}


void DH_CPlayer::UpdateAngle() 
{
	if (!m_bCharge && !GetbJumpSwitch())
	{
		// �ð� ������Ʈ
		m_fCurrentTime += 0.02f;

		// ���� ��� (�ֱ������� ��ȭ)
		m_fPosinAlngle = m_fAngleLimit * sin(m_fCurrentTime * m_fOscillationSpeed);
	}
}


void DH_CPlayer::Change_Motion()
{
}

void DH_CPlayer::Offset()
{
	int		iOffSetminY = 350;
	int		iOffSetmaxY = 400;

	if (!CScrollMgr::Get_Instance()->GetbShake01())
	{
		int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		if (iOffSetminY > GetPos().fY + iScrollY)
			CScrollMgr::Get_Instance()->Add_ScrollY(m_fSpeed);

		//if (iOffSetmaxY < GetPos().fY + iScrollY)
			//CScrollMgr::Get_Instance()->Add_ScrollY(-m_fSpeed);
	}
}

void DH_CPlayer::Physics()
{
	auto& player = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::PLAYER];
	auto& Tiles = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILE];

	//�÷��̾� ����
	if (player.size() > 0 && Tiles.size() > 0)
	{
		//���� Ÿ�� ã��
		CColliderMgr::GetInst()->GetCurObjectTile(player);

		//��ֹ� �浹
		if(CColliderMgr::GetInst()->GetbCollisionCooldown())
			CColliderMgr::GetInst()->CollisionEx(player, Tiles);

		//�߷��ۿ� ����
		CColliderMgr::GetInst()->GravityCondition(player, Tiles);

		//���� �Լ�
		CColliderMgr::GetInst()->PlayerJump(player);

		//�߷��ۿ� (Falling �϶���)
		CColliderMgr::GetInst()->PlayerGravityEx(player);
	}
}

void DH_CPlayer::JumpNormalVector()
{
	if (GetbJumpSwitch())
	{
		m_DHfX = m_fPosinAlngle;

		// �ʱ� �ӵ��� ���� ���
		float jumpSpeed = GetfJumpSpeed(); // �ʱ� �ӵ�
		float gravity = GetfGravity();     // �߷�
		float angle = D3DXToRadian(m_fPosinAlngle);

		// x�� �̵� �Ÿ� ���
		float velocityX = jumpSpeed * cosf(angle); // �ʱ� x�� �ӵ�

		AddPos(tVec2{ velocityX * m_DHfX, 0.f });

	}
}

void DH_CPlayer::SuperJump()
{
	if (dynamic_cast<DH_CTile*>(GetpCurTile())->GetbUniqTile() && m_eCurState == STATE::IDLE)
	{
		m_bControl = false;

		if (!m_bExcuseOne)
		{
			CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
			CSoundMgr::GetInst()->PlaySound(L"Charging.wav", SOUND_EFFECT, g_fVolume);

			//������ ����
			m_pSuperJump = new DH_CSuperJump;
			m_pSuperJump->SetName(L"SuperJump");
			m_pSuperJump->SetPos(GetPos());
			m_pSuperJump->SetScale(tVec2{ 150, 150 });
			Create_Object(m_pSuperJump, eObjectType::UI);

			m_bExcuseOne = true;
		}

		if (KEY_TAP(KEY::SPACE))
		{
			float fDot = m_pSuperJump->GetfDot() * -1;
			if (0.8f < fDot && fDot < 1.2f)
			{
				CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
				CSoundMgr::GetInst()->PlaySound(L"ChargeJump0.wav", SOUND_EFFECT, g_fVolume);

				//��������
				m_bControl = true;

				SetfJumpSpeed(30.f);
				m_bCharge = false;
				m_eCurState = STATE::JUMP;
				SetbJump(true);
				m_bExcuseOne = false;
			}
			else
			{
				CSoundMgr::GetInst()->StopSound(SOUND_EFFECT);
				CSoundMgr::GetInst()->PlaySound(L"ChargeJump1.wav", SOUND_EFFECT, g_fVolume);
				//�׳�����
				m_bControl = true;

				SetfJumpSpeed(15.f);
				m_bCharge = false;
				m_eCurState = STATE::JUMP;
				SetbJump(true);
				m_bExcuseOne = false;
			}

			//���� ����
			auto& UIGroup = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();
			vector<CObject*>::iterator iter = UIGroup.begin();
			for (; iter != UIGroup.end();)
			{
				if ((*iter)->GetName() == L"SuperJump")
				{
					delete (*iter);
					(*iter) = nullptr;
					iter = UIGroup.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}
}
