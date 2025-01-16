#pragma once
#include "Object.h"

class CUI;
class CMonster;

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void		Enter() PURE;
	virtual void		Exit() PURE;
	virtual void		Update();
	virtual void		LateUpdate();
	virtual void		Render();


public:
	void		AddvObject(CObject* _obj, eObjectType _type);
	void		DeleteGroup(eObjectType _eTarget);
	void		DeletePlayerGroup();
	void		DeleteUI();
	void		DeleteAll();


	void			SetName(const wstring _strName) { m_strName = _strName; }
	const wstring	GetName() { return m_strName; }

public:
	vector<CObject*>* GetvSceneObj() { return m_vecSceneObject; }
	vector<CObject*>* GetvRenderObj() { return m_vecRenderList; }

	vector<CObject*>& GetUIGroup()	{ return m_vecSceneObject[(UINT)eObjectType::UI]; }
	CTextures*		  GetpBG()		{ return m_pBG; }

public:
	void		Move_Frame();
	void		Move_Frame(FRAME& frame);
	void		FadeInOut();

public:
	void		SpawnPlayer(float _XPos, float YPos);
	template<typename T>
	void		SpawnMonster(float _XPos, float _YPos, float _iSacleX, float _iSacleY)
	{
		//몬스터 소환
		CObject* pMonster = new T;
		pMonster->SetScale(tVec2{ _iSacleX, _iSacleY });
		pMonster->SetPos(tVec2{ _XPos, _YPos });
		pMonster->Initialize();
		Create_Object(pMonster, eObjectType::MONSTER);
	}

public:
	void		AlphaBlend(Image* _pImage, float _alpha);


public:
	void		SetbBackToStart(bool _BackStart) { m_bBackToStart = _BackStart; }
	bool		GetbBackToStart() { return m_bBackToStart; }

private:
	vector<CObject*>	m_vecSceneObject[(ULONG)eObjectType::END];
	vector<CObject*>	m_vecRenderList[(int)RENDERID::RENDER_END];
	wstring				m_strName;

protected:
	FRAME				m_tFrame;
	CTextures*			m_pBG;
	Image*				m_pBlack;
	float				m_fFade;
	bool				m_bChangeScene;
	bool				m_bExcuteOne;
	bool				m_bExcuteTwo;
	bool				m_bDontCopy;
	bool				m_bBackToStart;

};