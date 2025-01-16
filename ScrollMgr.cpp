#include "pch.h"
#include "ScrollMgr.h"
#include "Object.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;


//�÷��̾� �����¿��� �������Դϴ�.
CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f), m_bShake01(0), m_iTime(0), m_fShakeTimeElapsed(1.f),
m_bShake02(0), m_bShake03(0), m_iShakeTime01(10) , m_iShakeTime02(30), m_iShakeTime03(60)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Update()
{
    Scroll_Shake01();
    Scroll_Shake02();
    Scroll_Shake03();
}

void CScrollMgr::Scroll_Lock(float _LockX, float _LockY)
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (WINCX - _LockX > m_fScrollX)
		m_fScrollX = WINCX - _LockX;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCY - _LockY > m_fScrollY)
		m_fScrollY = WINCY - _LockY;
}

void CScrollMgr::Scroll_Shake01()
{
    if (m_bShake01) // UI ��鸮�� ����
    {
        m_iShakeTime01--;
        if (m_iShakeTime01 <= 0)
        {
            m_bShake01 = false;
            m_iShakeTime01 = 10;
            m_fScrollX = Get_ScrollX(); // ��ũ���� ��Ȯ�� �ʱ� ��ġ�� ����
            m_fScrollY = Get_ScrollY();
            m_fShakeTimeElapsed = 1.0f; // ��鸲 �ð� �ʱ�ȭ
            return;
        }

        // ���� ��ũ�� ��ġ ��������
        float CurrentScrollX = Get_ScrollX();
        float CurrentScrollY = Get_ScrollY();

        float attenuation = float(m_iShakeTime01) / 60.0f; // �ʱ� ��鸲 ���� ����
        if (attenuation < 0.3f) 
            attenuation = 0.3f; // �ּ� ���� �� ���� (�ʹ� �������� �ʵ���)

        // ��鸲 �ֱ� 15 = �ʹ� , 10 = �Ĺ�
        float frequencyMultiplier = 15.0f - (10.0f * attenuation);

        // ��鸲 ���� (20.f)
        float offsetX = attenuation * 10.0f * sinf(m_fShakeTimeElapsed * frequencyMultiplier); // X�� ��鸲
        float offsetY = attenuation * 10.0f * sinf(m_fShakeTimeElapsed * (frequencyMultiplier + 1.0f)); // Y�� ��鸲
        // ��鸲 �ð� ����
        m_fShakeTimeElapsed += 0.3f; // �ð� �帧�� ���� ������ ����

        // ��鸲 ����
        m_fScrollX = CurrentScrollX + offsetX;
        m_fScrollY = CurrentScrollY + offsetY;
    }
}
void CScrollMgr::Scroll_Shake02()
{
    if (m_bShake02) // UI ��鸮�� ����
    {
        m_iShakeTime02--;
        if (m_iShakeTime02 <= 0)
        {
            m_bShake02 = false;
            m_iShakeTime02 = 30;
            m_fScrollX = Get_ScrollX(); // ��ũ���� ��Ȯ�� �ʱ� ��ġ�� ����
            m_fScrollY = Get_ScrollY();
            m_fShakeTimeElapsed = 1.0f; // ��鸲 �ð� �ʱ�ȭ
            return;
        }

        // ���� ��ũ�� ��ġ ��������
        float CurrentScrollX = Get_ScrollX();
        float CurrentScrollY = Get_ScrollY();

        float attenuation = float(m_iShakeTime02) / 60.0f; // �ʱ� ��鸲 ���� ���� (60�� �ʱⰪ���� ����)
        if (attenuation < 0.3f)
            attenuation = 0.3f; // �ּ� ���� �� ���� (�ʹ� �������� �ʵ���)

        // ��鸲 �ֱ� 15 = �ʹ� , 10 = �Ĺ�
        float frequencyMultiplier = 15.0f - (10.0f * attenuation);

        // ��鸲 ���� (20.f)
        float offsetX = attenuation * 20.0f * sinf(m_fShakeTimeElapsed * frequencyMultiplier); // X�� ��鸲
        float offsetY = attenuation * 20.0f * sinf(m_fShakeTimeElapsed * (frequencyMultiplier + 1.0f)); // Y�� ��鸲
        // ��鸲 �ð� ����
        m_fShakeTimeElapsed += 0.3f; // �ð� �帧�� ���� ������ ����

        // ��鸲 ����
        m_fScrollX = CurrentScrollX + offsetX;
        m_fScrollY = CurrentScrollY + offsetY;
    }
}
void CScrollMgr::Scroll_Shake03()
{

    if (m_bShake03) // UI ��鸮�� ����
    {
        m_iShakeTime03--;
        if (m_iShakeTime03 <= 0)
        {
            m_bShake03 = false;
            m_iShakeTime03 = 10;
            m_fScrollX = Get_ScrollX(); // ��ũ���� ��Ȯ�� �ʱ� ��ġ�� ����
            m_fScrollY = Get_ScrollY();
            m_fShakeTimeElapsed = 1.0f; // ��鸲 �ð� �ʱ�ȭ
            return;
        }

        // ���� ��ũ�� ��ġ ��������
        float CurrentScrollX = Get_ScrollX();
        float CurrentScrollY = Get_ScrollY();

        float attenuation = float(m_iShakeTime03) / 60.0f; // �ʱ� ��鸲 ���� ���� (60�� �ʱⰪ���� ����)
        if (attenuation < 0.3f)
            attenuation = 0.3f; // �ּ� ���� �� ���� (�ʹ� �������� �ʵ���)

        // ��鸲 �ֱ� 15 = �ʹ� , 10 = �Ĺ�
        float frequencyMultiplier = 15.0f - (10.0f * attenuation);

        // ��鸲 ���� (20.f)
        float offsetX = attenuation * 30.0f * sinf(m_fShakeTimeElapsed * frequencyMultiplier); // X�� ��鸲
        float offsetY = attenuation * 30.0f * sinf(m_fShakeTimeElapsed * (frequencyMultiplier + 1.0f)); // Y�� ��鸲
        // ��鸲 �ð� ����
        m_fShakeTimeElapsed += 0.3f; // �ð� �帧�� ���� ������ ����

        // ��鸲 ����
        m_fScrollX = CurrentScrollX + offsetX;
        m_fScrollY = CurrentScrollY + offsetY;
    }

}


void CScrollMgr::Scroll_MoveX(float _fX)
{
    m_fScrollX += _fX;
}
