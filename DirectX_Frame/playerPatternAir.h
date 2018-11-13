//======================================================================
//	プレイヤー空中パターンヘッダ[playerPatternAir.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_AIR_H_
#define _PLAYER_PATTERN_AIR_H_

#include "playerPatternBase.h"

class CPlayerPatternAir : public CPlayerPatternBase
{
public:
	CPlayerPatternAir(D3DXVECTOR2 vec) { m_MoveVec = vec; }
	~CPlayerPatternAir() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override{}
	void Update(CPlayer* player)override;
private:
	D3DXVECTOR2 m_MoveVec;
};

#endif // !_PLAYER_PATTERN_AIR_H_

