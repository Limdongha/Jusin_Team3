#include "pch.h"
#include "DH_CSuperJump.h"
#include "PngManager.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "DH_CPlayer.h"
#include "KeyMgr.h"

DH_CSuperJump::DH_CSuperJump() : m_fAlngle(0), m_fTargetAlngle(0), m_fDot(0)
{
	CPngManager::GetInst()->Insert_Png
	(L"./\\Content\\Textures\\UNIQ\\Base.png", L"Base");

	m_pLeftPNG = CPngManager::GetInst()->Find_Png(L"Base");

	m_eRender = RENDERID::RENDER_UI;


	//�÷��̾� ���� �ʱ�ȭ
	m_vPointInfo.vLook = { 0.f , -1.f, 0.f };
	m_vPointInfo.vDir = { 0.f , 0.f , 0.f };

	m_vTargetInfo.vLook = { 0.f , -1.f, 0.f };
	m_vTargetInfo.vDir = { 0.f , 0.f , 0.f };

	GETPLAYER
	m_vPoint[0] = { 0.f, 0.f, 0.f };
	m_vPoint[1] = { +5, -25, 0.f };
	m_vPoint[2] = { 0.f, -55, 0.f };
	m_vPoint[3] = { -5, -25, 0.f };

	m_vTargetPoint = { 0.f , -60.f, 0.f };

	for (int i = 0; i < 4; ++i)
		m_vPointOrigin[i] = m_vPoint[i];

	m_vTargetOriginPoint = m_vTargetPoint;
}

DH_CSuperJump::~DH_CSuperJump()
{
}

void DH_CSuperJump::Render()
{
	AlphaBlend(m_pLeftPNG, 150, 150, 0, 0, 0, 0, 1.f);

	HPEN newPen = CreatePen(PS_SOLID, 5, RGB(0, 191, 255));
	HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);

	Ellipse(g_memDC,
		(int)m_vTargetPoint.x - 10,
		(int)m_vTargetPoint.y - 10,
		(int)m_vTargetPoint.x + 10,
		(int)m_vTargetPoint.y + 10
	);

	SelectObject(g_memDC, oldPen);
	SelectObject(g_memDC, oldBrush);
	DeleteObject(newBrush);
	DeleteObject(newPen);

	DrawPolygonCustom(m_vPoint, 4, RGB(0, 255, 255));


	SetBkMode(g_memDC, TRANSPARENT); // ����� �����ϰ� ����
	SetTextColor(g_memDC, RGB(255, 0, 140)); // �ؽ�Ʈ ����

	wchar_t scoreText01[50];
	swprintf(scoreText01, 50, L"Ÿ������ Look ����X : %f", m_vPointInfo.vLook.x);

	TextOut(g_memDC,
		WINCX / 2 + 100,
		240,
		scoreText01,
		int(wcslen(scoreText01)));

	wchar_t scoreText02[50];
	swprintf(scoreText02, 50, L"Ÿ������ Look ����Y : %f", m_vPointInfo.vLook.y);

	TextOut(g_memDC,
		WINCX / 2 + 100,
		260,
		scoreText02,
		int(wcslen(scoreText02)));

	wchar_t scoreText03[50];
	swprintf(scoreText03, 50, L"���� : %f", -1.f * m_fDot);

	TextOut(g_memDC,
		WINCX / 2 + 100,
		280,
		scoreText03,
		int(wcslen(scoreText03)));
}

void DH_CSuperJump::Update()
{
	GETPLAYER;
	SetPos(tVec2{ Player.GetPos().fX + SCROLLX,Player.GetPos().fY + SCROLLY });
	m_vPointInfo.vPos = { Player.GetPos().fX + SCROLLX, Player.GetPos().fY + SCROLLY, 0.f };
	m_vTargetInfo.vPos = { Player.GetPos().fX + SCROLLX, Player.GetPos().fY + SCROLLY, 0.f };

	//ȭ��ǥ ���
	D3DXMATRIX matScale, matRotate, matTransform;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationZ(&matRotate, m_fAlngle);
	D3DXMatrixTranslation(&matTransform, m_vPointInfo.vPos.x, m_vPointInfo.vPos.y, 0.f);

	m_vPointInfo.matWorld = matScale * matRotate * matTransform;

	for (int i = 0; i < 4; ++i)
	{
		m_vPoint[i] = m_vPointOrigin[i];
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &m_vPointInfo.matWorld);
	}

	m_vPointInfo.vLook = { 0.f, -1.f, 0.f };
	D3DXVec3TransformNormal(&m_vPointInfo.vLook, &m_vPointInfo.vLook, &m_vPointInfo.matWorld);


	//Ÿ�� ���
	D3DXMATRIX TargetmatRotate, TargetmatTransform;

	D3DXMatrixRotationZ(&TargetmatRotate, m_fTargetAlngle);
	D3DXMatrixTranslation(&TargetmatTransform, m_vTargetInfo.vPos.x, m_vTargetInfo.vPos.y, 0.f);

	m_vTargetInfo.matWorld = TargetmatRotate * TargetmatTransform;

	m_vTargetPoint = m_vTargetOriginPoint;
	D3DXVec3TransformCoord(&m_vTargetPoint, &m_vTargetPoint, &m_vTargetInfo.matWorld);

	//ȭ��ǥ Ÿ������ ���ϴ� ����
	D3DXVECTOR3 ToTargetVector = m_vPoint[0] - m_vTargetPoint;
	D3DXVec3Normalize(&m_vPointInfo.vLook, &m_vPointInfo.vLook);
	D3DXVec3Normalize(&ToTargetVector, &ToTargetVector);

	m_fDot = D3DXVec3Dot(&ToTargetVector , &m_vPointInfo.vLook);

	m_fAlngle += D3DXToRadian(5.f);
	m_fTargetAlngle -= D3DXToRadian(10.f);

	CUI::Update();
}

void DH_CSuperJump::AlphaBlend
(Image* _pImage, int _SizeX, int _SizeY, int _iOffsetX, int _iOffsetY, int _CropY, int _Crop, float _alpha)
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
	Rect sourceRect(0, _CropY, _SizeX, _SizeY - _Crop);

	GETPLAYER;
	
	Rect destRect = {
		int(GetPos().fX - GetScale().fX / 2) - _iOffsetX,
		int(GetPos().fY - GetScale().fY / 2) - _iOffsetY + _CropY,
		_SizeX,
		_SizeY - _Crop };


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