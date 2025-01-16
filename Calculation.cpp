#include "pch.h"
#include "Calculation.h"
#include "Define.h"

// - - - - - - - �� ���� �� - - - - - - -//

//�浹ü�� x, y ���� ������ ������ �˷��ִ� �Լ�
float CalDia(float fX1, float fX2, float fY1, float fY2)
{
	float dx = fX2 - fX1;
	float dy = fY2 - fY1;
	return sqrt(dx * dx + dy * dy);
}

//�� ��ü�� �غ�, ���̸� �����
float CalBase(float _PlayerXpos, float _MouseXpos)
{
	return abs(_PlayerXpos - _MouseXpos);
}
float CalHeight(float _PlayerYpos, float _MouseYpos)
{
	return abs(_PlayerYpos - _MouseYpos);
}

//�� ��ü�� ��Ÿ���� �����(��	��,����)
float CalTheta(float _base, float _height)
{
	float theta = atan2(_height, _base); // ���� ������ ��ȯ
	theta = theta * 180.0f / PI;    // ������ ��(degree)�� ��ȯ

	return theta;
}


//������ ������ y �� ����
float CalStraightY(float StraightY, float _LPointX, float _LPointY, float _RPointX, float _RPointY, float PosX)
{
	//StraightY, PosX = ���ϰ����ϴ� ��ǥ , _LPointX, _LPointY = ���� ����Ʈ, _RPointX, _RPointY = ������ ����Ʈ

	StraightY = 
		((_RPointY - _LPointY) / (_RPointX - _LPointX)) * (PosX - _LPointX) + _LPointY;
	return StraightY;
}

//������ ����
float CalParabola(float m_fJumpSpeed, float m_fTime)
{
	float Curve;
	Curve = (m_fJumpSpeed * sinf(90.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;

	return Curve;
}
