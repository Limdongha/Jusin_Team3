#pragma once

// - - - - - - - ↓ 계산식 ↓ - - - - - - -//

//충돌체의 x, y 값을 넣어주면 두 물체사이의 거리를 알려주는 함수
float CalDia(float fX1, float fX2, float fY1, float fY2);

//두 물체의 밑변, 높이를 계산함
float CalBase(float _PlayerXpos, float _MouseXpos);
float CalHeight(float _PlayerYpos, float _MouseYpos);

//두 물체의 세타각을 계산함(밑	변,높이)
float CalTheta(float _base, float _height);

// 두 벡터의 끼인각을 내적을 이용해 계산
float CalVecTheta(D3DXVECTOR3 A, D3DXVECTOR3 B);

//직선의 방정식 y 값 도출
float CalStraightY(float StraightY, float _LPointX, float _LPointY, float _RPointX, float _RPointY, float PosX);

//포물선 공식
float CalParabola(float m_fJumpSpeed, float m_fTime);







//각도를 이용한 이동공식
//m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
//m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


//역함수와 치역계산 공식(abs 쓰지마)
//fRadian = acosf(fWidth / fDiagonal);
//if (m_pTarget->Get_Info().fY > m_tInfo.fY)
//	fRadian = (2.f * PI) - fRadian;
//m_fAngle = fRadian * (180.f / PI);