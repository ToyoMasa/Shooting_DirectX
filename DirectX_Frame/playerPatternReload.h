//======================================================================
//	プレイヤーリロードパターンヘッダ[playerPatternReload.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_RELOAD_H_
#define _PLAYER_PATTERN_RELOAD_H_

#include "playerPatternBase.h"

class CPlayerPatternReload : public CPlayerPatternBase
{
public:
	CPlayerPatternReload() {}
	~CPlayerPatternReload() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override {}
	void Update(CPlayer* player)override;
private:
	int	m_CountFrame;
};

#endif // !_PLAYER_PATTERN_RELOAD_H_
