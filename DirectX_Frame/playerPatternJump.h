//======================================================================
//	プレイヤージャンプパターンヘッダ[playerPatternJump.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_JUMP_H_
#define _PLAYER_PATTERN_JUMP_H_

#include "playerPatternBase.h"

class CPlayerPatternJump : public CPlayerPatternBase
{
public:
	CPlayerPatternJump() {}
	~CPlayerPatternJump() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override{}
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_JUMP_H_

