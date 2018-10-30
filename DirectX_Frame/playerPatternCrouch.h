//======================================================================
//	プレイヤー歩行パターンヘッダ[playerPatternCrouch.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_CROUCH_H_
#define _PLAYER_PATTERN_CROUCH_H_

#include "playerPatternBase.h"

class CPlayerPatternCrouch : public CPlayerPatternBase
{
public:
	CPlayerPatternCrouch() {}
	~CPlayerPatternCrouch() {}

	void Init(CPlayer* player);
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_CROUCH_H_

