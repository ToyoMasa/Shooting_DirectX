//======================================================================
//	プレイヤーストップパターンヘッダ[playerPatternStop.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_STOP_H_
#define _PLAYER_PATTERN_STOP_H_

#include "playerPatternBase.h"

//*********************************************************
//	行動不能時の処理
//*********************************************************
class CPlayerPatternStop : public CPlayerPatternBase
{
public:
	void Init(CPlayer* player)override {}
	void Uninit()override {}
	void Update()override {}
};

#endif // !_PLAYER_PATTERN_STOP_H_

