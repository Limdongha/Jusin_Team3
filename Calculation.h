#pragma once

// - - - - - - - �� ���� �� - - - - - - -//

//�浹ü�� x, y ���� �־��ָ� �� ��ü������ �Ÿ��� �˷��ִ� �Լ�
float CalDia(float fX1, float fX2, float fY1, float fY2);

//�� ��ü�� �غ�, ���̸� �����
float CalBase(float _PlayerXpos, float _MouseXpos);
float CalHeight(float _PlayerYpos, float _MouseYpos);

//�� ��ü�� ��Ÿ���� �����(��	��,����)
float CalTheta(float _base, float _height);

// �� ������ ���ΰ��� ������ �̿��� ���
float CalVecTheta(D3DXVECTOR3 A, D3DXVECTOR3 B);

//������ ������ y �� ����
float CalStraightY(float StraightY, float _LPointX, float _LPointY, float _RPointX, float _RPointY, float PosX);

//������ ����
float CalParabola(float m_fJumpSpeed, float m_fTime);







//������ �̿��� �̵�����
//m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
//m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


//���Լ��� ġ����� ����(abs ������)
//fRadian = acosf(fWidth / fDiagonal);
//if (m_pTarget->Get_Info().fY > m_tInfo.fY)
//	fRadian = (2.f * PI) - fRadian;
//m_fAngle = fRadian * (180.f / PI);