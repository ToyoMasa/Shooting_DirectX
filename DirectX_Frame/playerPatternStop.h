//======================================================================
//	�v���C���[�X�g�b�v�p�^�[���w�b�_[playerPatternStop.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_STOP_H_
#define _PLAYER_PATTERN_STOP_H_

#include "playerPatternBase.h"

//*********************************************************
//	�s���s�\���̏���
//*********************************************************
class CPlayerPatternStop : public CPlayerPatternBase
{
public:
	void Init(CPlayer* player)override {}
	void Uninit()override {}
	void Update()override {}
};

#endif // !_PLAYER_PATTERN_STOP_H_

