//======================================================================
//	プレイヤーアイドルパターンヘッダ[playerPatternIdle.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_IDLE_H_
#define _PLAYER_PATTERN_IDLE_H_

#include "playerPatternBase.h"

class CPlayerPatternIdle : public CPlayerPatternBase
{
public:
	CPlayerPatternIdle() {}
	~CPlayerPatternIdle() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override{}
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_IDLE_H_
