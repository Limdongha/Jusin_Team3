#pragma once

#include "Object.h"


class CJumpPad : public CObject
{
public:
	CJumpPad();
	~CJumpPad();

public:
	virtual void	Render() override;
	virtual void	Update() override;
	virtual void	Initialize() override;
	virtual void	LateUpdate() override;
	
	
	

private:


	
};
