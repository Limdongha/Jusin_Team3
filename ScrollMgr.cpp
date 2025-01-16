#include "pch.h"
#include "ScrollMgr.h"
#include "Object.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;


//플레이어 오프셋에서 조절중입니다.
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
    if (m_bShake01) // UI 흔들리는 연출
    {
        m_iShakeTime01--;
        if (m_iShakeTime01 <= 0)
        {
            m_bShake01 = false;
            m_iShakeTime01 = 10;
            m_fScrollX = Get_ScrollX(); // 스크롤을 정확히 초기 위치로 복원
            m_fScrollY = Get_ScrollY();
            m_fShakeTimeElapsed = 1.0f; // 흔들림 시간 초기화
            return;
        }

        // 현재 스크롤 위치 가져오기
        float CurrentScrollX = Get_ScrollX();
        float CurrentScrollY = Get_ScrollY();

        float attenuation = float(m_iShakeTime01) / 60.0f; // 초기 흔들림 강도 기준
        if (attenuation < 0.3f) 
            attenuation = 0.3f; // 최소 감쇠 값 설정 (너무 느려지지 않도록)

        // 흔들림 주기 15 = 초반 , 10 = 후반
        float frequencyMultiplier = 15.0f - (10.0f * attenuation);

        // 흔들림 강도 (20.f)
        float offsetX = attenuation * 10.0f * sinf(m_fShakeTimeElapsed * frequencyMultiplier); // X축 흔들림
        float offsetY = attenuation * 10.0f * sinf(m_fShakeTimeElapsed * (frequencyMultiplier + 1.0f)); // Y축 흔들림
        // 흔들림 시간 증가
        m_fShakeTimeElapsed += 0.3f; // 시간 흐름에 따라 사인파 진행

        // 흔들림 적용
        m_fScrollX = CurrentScrollX + offsetX;
        m_fScrollY = CurrentScrollY + offsetY;
    }
}
void CScrollMgr::Scroll_Shake02()
{
    if (m_bShake02) // UI 흔들리는 연출
    {
        m_iShakeTime02--;
        if (m_iShakeTime02 <= 0)
        {
            m_bShake02 = false;
            m_iShakeTime02 = 30;
            m_fScrollX = Get_ScrollX(); // 스크롤을 정확히 초기 위치로 복원
            m_fScrollY = Get_ScrollY();
            m_fShakeTimeElapsed = 1.0f; // 흔들림 시간 초기화
            return;
        }

        // 현재 스크롤 위치 가져오기
        float CurrentScrollX = Get_ScrollX();
        float CurrentScrollY = Get_ScrollY();

        float attenuation = float(m_iShakeTime02) / 60.0f; // 초기 흔들림 강도 기준 (60을 초기값으로 가정)
        if (attenuation < 0.3f)
            attenuation = 0.3f; // 최소 감쇠 값 설정 (너무 느려지지 않도록)

        // 흔들림 주기 15 = 초반 , 10 = 후반
        float frequencyMultiplier = 15.0f - (10.0f * attenuation);

        // 흔들림 강도 (20.f)
        float offsetX = attenuation * 20.0f * sinf(m_fShakeTimeElapsed * frequencyMultiplier); // X축 흔들림
        float offsetY = attenuation * 20.0f * sinf(m_fShakeTimeElapsed * (frequencyMultiplier + 1.0f)); // Y축 흔들림
        // 흔들림 시간 증가
        m_fShakeTimeElapsed += 0.3f; // 시간 흐름에 따라 사인파 진행

        // 흔들림 적용
        m_fScrollX = CurrentScrollX + offsetX;
        m_fScrollY = CurrentScrollY + offsetY;
    }
}
void CScrollMgr::Scroll_Shake03()
{

    if (m_bShake03) // UI 흔들리는 연출
    {
        m_iShakeTime03--;
        if (m_iShakeTime03 <= 0)
        {
            m_bShake03 = false;
            m_iShakeTime03 = 10;
            m_fScrollX = Get_ScrollX(); // 스크롤을 정확히 초기 위치로 복원
            m_fScrollY = Get_ScrollY();
            m_fShakeTimeElapsed = 1.0f; // 흔들림 시간 초기화
            return;
        }

        // 현재 스크롤 위치 가져오기
        float CurrentScrollX = Get_ScrollX();
        float CurrentScrollY = Get_ScrollY();

        float attenuation = float(m_iShakeTime03) / 60.0f; // 초기 흔들림 강도 기준 (60을 초기값으로 가정)
        if (attenuation < 0.3f)
            attenuation = 0.3f; // 최소 감쇠 값 설정 (너무 느려지지 않도록)

        // 흔들림 주기 15 = 초반 , 10 = 후반
        float frequencyMultiplier = 15.0f - (10.0f * attenuation);

        // 흔들림 강도 (20.f)
        float offsetX = attenuation * 30.0f * sinf(m_fShakeTimeElapsed * frequencyMultiplier); // X축 흔들림
        float offsetY = attenuation * 30.0f * sinf(m_fShakeTimeElapsed * (frequencyMultiplier + 1.0f)); // Y축 흔들림
        // 흔들림 시간 증가
        m_fShakeTimeElapsed += 0.3f; // 시간 흐름에 따라 사인파 진행

        // 흔들림 적용
        m_fScrollX = CurrentScrollX + offsetX;
        m_fScrollY = CurrentScrollY + offsetY;
    }

}


void CScrollMgr::Scroll_MoveX(float _fX)
{
    m_fScrollX += _fX;
}
