#pragma once
class CTextures;

class CObject
{
public:
	CObject();
	virtual  ~CObject();
	
public:
	virtual void		Update();
	virtual void		LateUpdate();
	virtual void		Initialize();
	virtual void		Render();
	virtual void		Release();

public:
	// GET �Լ��� //

	tObjInfo*			Get_Info_Pointer()	{ return &m_tObjInfo; }
	tVec2				GetPos()			{ return m_tObjInfo.tPos; }
	tVec2				GetScale()			{ return m_tObjInfo.tScale; }
	bool				GetbArrive()		{ return m_bArrive; }
	bool				GetbFalling()		{ return m_bFalling; }
	const wstring		GetName()			{ return m_strName; }
	CObject*			GetpCurTile()		{ return m_pCurTile; }
	vector<CObject*>*	GetvCurTileList()	{ return &m_vCurTileVector; }
	bool				GetbJump()			{ return m_bJump; }
	bool				GetbDoubleJump()	{ return m_bDoubleJump; }
	float				GetfJumpSpeed()		{ return m_fJumpSpeed; }
	float				GetfTime()			{ return m_fTime; }
	float				GetfGravity()		{ return m_fGravity; }
	int					GetiDirection()		{ return m_iDirection; }
	float				GetiHP()			{ return m_iHP; }
	float				GetfSpeed()			{ return m_fSpeed; }
	float				GetfCusDeltaTime()	{ return m_fCusDeltaTime; }
	float				GetfRealDeltaTime() { return m_fRealDeltaTime; }
	int					GetiDieTime()		{ return m_iDieTime; }
	bool				GetbDie()			{ return m_bDie; }
	float				GetfCurve()			{ return m_fCurve; }
	float				GetfDoubleCurve()	{ return m_fDoubleCurve; }
	float				GetfCurve2()		{ return m_fCurve2; }
	STATE				GeteState()			{ return m_eCurState; }
	eEffectName			GetCurEffect()		{ return m_eCurEffect; }
	bool				GetbJumpSwitch()	{ return m_bJumpSwitch; }
	bool				GetbMoveFrame()		{ return m_bMoveFrame; }
	float				GetfRotateSpeed()	{ return m_fRotateSpeed; }

public:
	
	// SET �Լ��� //
	
	void		SetPos(tVec2 _Set)				{ m_tObjInfo.tPos = _Set; }
	void		SetScale(tVec2 _Set)			{ m_tObjInfo.tScale = _Set; }
	void		SetName(const wstring& _strName){ m_strName = _strName; }
	void		SetbFalling(bool _bFall)		{ m_bFalling = _bFall; }
	void		SetpCurTile(CObject* _Tile)		{ m_pCurTile = _Tile; }
	void		SetvCurTileList (CObject* _Tile){ m_vCurTileVector.push_back(_Tile); }
	void		SetbJump(bool _bjump)			{ m_bJump = _bjump; }
	void		SetbDoubleJump(bool _bDjump)	{ m_bDoubleJump = _bDjump; }
	void		SetfJumpSpeed(float _fJSpeed)	{ m_fJumpSpeed = _fJSpeed; }
	void		SetfTime(float _fTime)			{ m_fTime = _fTime; }
	void		SeteState(STATE _state)			{ m_eCurState = _state; }
	void		SetfGravity(float _fVel)		{ m_fGravity = _fVel; }
	void		SetiDirection(int _iDir)		{ m_iDirection = _iDir; }
	void		SetCurEffect(eEffectName _eType){ m_eCurEffect = _eType; }
	void		SetbArrive(bool _arrive)		{ m_bArrive = _arrive; }
	void		SetiHP(float _iHP)				{ m_iHP = _iHP; }
	void		SetfSpeed(float _fSpeed)		{ m_fSpeed = _fSpeed; }
	void		SetiDieTime(int _DieTime)		{ m_iDieTime = _DieTime; }
	void		SetbDie(bool _Die)				{ m_bDie = _Die; }
	void		SetfCurve(float _Curve)			{ m_fCurve = _Curve; }
	void		SetfDoubleCurve(float _Curve)	{ m_fDoubleCurve = _Curve; }
	void		SetfCure2(float _Curve)			{ m_fCurve2 = _Curve; }
	void		SetbJumpSwitch(bool _Switch)	{ m_bJumpSwitch = _Switch; }
	void		SetbMoveFrame(bool _Frame)		{ m_bMoveFrame = _Frame; }
	void		SetRotateSpeed(float _RotateSpeed) { m_fRotateSpeed = _RotateSpeed; }
	void		SetPosVector(float _X, float _Y) { m_tInfo.vPos = { _X, _Y, 0.f }; }

public:
	
	// Add �Լ��� //
	
	void		AddPos(tVec2 _Set)			{ m_tObjInfo.tPos + _Set; }
	void		AddScale(tVec2 _Set)		{ m_tObjInfo.tScale + _Set; }
	void		AddfGravity(float _fVel)	{ m_fGravity += _fVel; }
	void		AddfJumpSpeed(float _fJump) { m_fJumpSpeed += _fJump; }
	void		AddfTime(float _fTime)		{ m_fTime += _fTime; }
	void		AddiHP(float _iHP)			{ m_iHP += _iHP; }
	void		AddfSpeed(float _fSpeed)	{ m_fSpeed += _fSpeed; }
	void		AddfCurve(float _Curve)		{ m_fCurve += _Curve; }
	void		AddfDoubleCurve(float _Curve){ m_fDoubleCurve += _Curve; }
	void		AddfCurve2(float _Curve)		{ m_fCurve2 += _Curve; }

public:

	// �߷°��� �Լ��� //

	void		MinusfCurve() { m_fCurve *= -1; }
	void		MinusfDoubleCurve() { m_fDoubleCurve *= -1; }
	void		MinusfCurve2() { m_fCurve2 *= -1; }


public:
	
	// ������ �׸��� //
	void		DrawPolygon(int _PointNum, float centerX, float centerY, float radius, COLORREF _Color);
	void		GenerateRegularPolygon(D3DXVECTOR3* points, int numPoints, float centerX, float centerY, float radius);
	// Ŀ���� ������ �׸��� //
	void		DrawPolygonCustom(D3DXVECTOR3* _Point, int _PointNum, COLORREF _Color);

public:

	// ������Ʈ ���� �ݸ��� �����Լ� //

	void		CreateAttackBox
				(float _PosfX, float _PosfY, float _ScaleX, float _ScaleY, eEffect _eType,
				eEffectName _enType, bool _follow, float _fOffsetX, float _fOffsetY, bool _Rect);


public:
	
	// �ִϸ��̼� ���� //

	void		Move_Frame();

public:
	const RENDERID Get_GroupID() const { return m_eRender; }

private:
	//�̺�Ʈ �Ŵ����� ���ؼ��� ��������
	void		SetDead() { m_bArrive = false; }
	friend class CEventMgr;

private:
	//������Ʈ �̸�
	wstring			m_strName;

	//������Ʈ ����
	tObjInfo		m_tObjInfo;
	RECT			m_rRect;

	//���� ���
	bool			m_bDie;
	bool			m_bArrive;
	bool			m_bFalling;
	bool			m_bMoveFrame;
	bool			m_bJump;
	bool			m_bDoubleJump;
	bool			m_bJumpSwitch;
	float			m_fCurve;
	float			m_fDoubleCurve;
	float			m_fCurve2;
	float			m_fJumpSpeed;
	float			m_fTime;
	float			m_fSpeed;
	float			m_fGravity;
	float			m_fCusDeltaTime;
	float			m_fRealDeltaTime;
	float			m_iHP;
	int				m_iDirection;
	int				m_iDieTime;

protected:
	//��Ʈ�� ����
	CTextures*			m_pCurTex;
	CTextures*			m_pLeftTex;
	CTextures*			m_pRightTex;

protected:
	//PNG ����
	Image*			m_pCurPNG;
	Image*			m_pLeftPNG;
	Image*			m_pRightPNG;

protected:
	//�ִϸ��̼�
	CObject*			m_pCurTile;
	vector<CObject*>	m_vCurTileVector;

	//�ִϸ��̼� ����
	STATE				m_eCurState;
	STATE				m_ePreState;

	//����Ʈ�̸�
	eEffectName			m_eCurEffect;
	eEffectName			m_ePreEffect;

	//�ִϸ��̼� ������
	FRAME				m_tFrame;

	//�������̾�
	RENDERID			m_eRender;

protected:
	INFO m_tInfo;
	float m_fRotateSpeed;
	float m_fAngle;

public:
	INFO& GetInfo() { return m_tInfo; }
};

