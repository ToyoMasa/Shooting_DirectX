//======================================================================
//	�v���C���[�󒆃p�^�[���w�b�_[playerPatternAir.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_AIR_H_
#define _PLAYER_PATTERN_AIR_H_

#include "playerPatternBase.h"

class CPlayerPatternAir : public CPlayerPatternBase
{
public:
	CPlayerPatternAir() {}
	~CPlayerPatternAir() {}

	void Init(CPlayer* player);
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_AIR_H_

