//======================================================================
//	�v���C���[ADS�p�^�[���w�b�_[playerPatternADS.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_ADS_H_
#define _PLAYER_PATTERN_ADS_H_

#include "playerPatternBase.h"

//*********************************************************
//	�e��`������ł��鎞�̏���
//*********************************************************
class CPlayerPatternADS : public CPlayerPatternBase
{
public:
	void Init(CPlayer* player)override;
	void Uninit()override;
	void Update()override;

	virtual void Move(D3DXVECTOR2 move)override;
	virtual void Rotate(D3DXVECTOR2 rot)override;
	virtual void Shoot()override;
	virtual void Reload()override;
private:
	DWORD m_Time;
};

#endif // !_PLAYER_PATTERN_ADS_H_

