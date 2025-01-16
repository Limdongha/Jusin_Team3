#include "pch.h"
#include "Calculation.h"
#include "Define.h"

// - - - - - - - ↓ 계산식 ↓ - - - - - - -//

//충돌체의 x, y 값을 넣으면 빗변을 알려주는 함수
float CalDia(float fX1, float fX2, float fY1, float fY2)
{
	float dx = fX2 - fX1;
	float dy = fY2 - fY1;
	return sqrt(dx * dx + dy * dy);
}

//두 물체의 밑변, 높이를 계산함
float CalBase(float _PlayerXpos, float _MouseXpos)
{
	return abs(_PlayerXpos - _MouseXpos);
}
float CalHeight(float _PlayerYpos, float _MouseYpos)
{
	return abs(_PlayerYpos - _MouseYpos);
}

//두 물체의 세타각을 계산함(밑	변,높이)
float CalTheta(float _base, float _height)
{
	float theta = atan2(_height, _base); // 라디안 단위로 반환
	theta = theta * 180.0f / PI;    // 라디안을 도(degree)로 변환

	return theta;
}


//직선의 방정식 y 값 도출
float CalStraightY(float StraightY, float _LPointX, float _LPointY, float _RPointX, float _RPointY, float PosX)
{
	//StraightY, PosX = 구하고자하는 좌표 , _LPointX, _LPointY = 왼쪽 포인트, _RPointX, _RPointY = 오른쪽 포인트

	StraightY = 
		((_RPointY - _LPointY) / (_RPointX - _LPointX)) * (PosX - _LPointX) + _LPointY;
	return StraightY;
}

//포물선 공식
float CalParabola(float m_fJumpSpeed, float m_fTime)
{
	float Curve;
	Curve = (m_fJumpSpeed * sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;

	return Curve;
}
