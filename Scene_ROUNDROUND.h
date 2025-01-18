#pragma once
#include "Scene.h"
class CScene_ROUNDROUND : public CScene
{
public:
	CScene_ROUNDROUND();
	virtual ~CScene_ROUNDROUND();

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
	virtual void		Update() override;
	virtual void		Render() override;

	void				CreateJumpPad();
	void				DeleteJumpPad();


private:
	CTextures* m_pBG2;
	int		m_iBG1;
	int		m_iBG2;
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;

	int		m_iCountCreatePad; // 패드 생성 수 에 따라서 패드 위치 결정
};

