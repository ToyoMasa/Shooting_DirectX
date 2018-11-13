//======================================================================
//	プレイヤーダッシュパターンヘッダ[playerPatternDash.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_DASH_H_
#define _PLAYER_PATTERN_DASH_H_

#include "playerPatternBase.h"

class CPlayerPatternDash : public CPlayerPatternBase
{
public:
	CPlayerPatternDash() {}
	~CPlayerPatternDash() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override;
	void Update(CPlayer* player)override;
};

#endif // !_PLAYER_PATTERN_DASH_H_
