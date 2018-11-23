//======================================================================
//	プレイヤー通常時パターンヘッダ[playerPatternNormal.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_NORMAL_H_
#define _PLAYER_PATTERN_NORMAL_H_

#include "playerPatternBase.h"

class CPlayerPatternNormal : public CPlayerPatternBase
{
public:
	CPlayerPatternNormal() {}
	~CPlayerPatternNormal() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override{}
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_NORMAL_H_
