//======================================================================
//	プレイヤーパターンヘッダ[playerPatternBase.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_BASE_H_
#define _PLAYER_PATTERN_BASE_H_

class CPlayerPatternBase
{
public:
	CPlayerPatternBase() {}
	~CPlayerPatternBase() {}

	virtual void Init() = 0;
	virtual void Update(CPlayer* player) = 0;
};

#endif // !_PLAYER_PATTERN_BASE_H_

