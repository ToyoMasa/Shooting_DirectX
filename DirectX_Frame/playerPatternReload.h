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
	void Uninit()override {}
	void Update()override;

	virtual void Move(D3DXVECTOR2 move)override;
	virtual void Rotate(D3DXVECTOR2 rot)override;
private:
	int	m_CountFrame;
};

#endif // !_PLAYER_PATTERN_RELOAD_H_
