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
	CPlayerPatternJump(D3DXVECTOR2 vec) { m_MoveVec = vec; }
	~CPlayerPatternJump() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override{}
	void Update(CPlayer* player)override;
private:
	D3DXVECTOR2 m_MoveVec;
};

#endif // !_PLAYER_PATTERN_JUMP_H_

