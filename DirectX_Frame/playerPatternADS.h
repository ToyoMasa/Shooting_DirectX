//======================================================================
//	プレイヤーADSパターンヘッダ[playerPatternADS.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_ADS_H_
#define _PLAYER_PATTERN_ADS_H_

#include "playerPatternBase.h"

class CPlayerPatternADS : public CPlayerPatternBase
{
public:
	CPlayerPatternADS() {}
	~CPlayerPatternADS() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override;
	void Update(CPlayer* player)override;
private:
	DWORD m_Time;
};

#endif // !_PLAYER_PATTERN_ADS_H_

