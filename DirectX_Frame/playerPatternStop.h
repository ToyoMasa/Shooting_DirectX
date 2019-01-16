//======================================================================
//	プレイヤーストップパターンヘッダ[playerPatternStop.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_STOP_H_
#define _PLAYER_PATTERN_STOP_H_

#include "playerPatternBase.h"

class CPlayerPatternStop : public CPlayerPatternBase
{
public:
	CPlayerPatternStop() {}
	~CPlayerPatternStop() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override {}
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_STOP_H_

