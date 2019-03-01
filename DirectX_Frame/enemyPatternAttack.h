//======================================================================
//	“GUŒ‚ƒpƒ^[ƒ“ƒwƒbƒ_[enemyPatternAttack.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_ATTACK_H_
#define _ENEMY_PATTERN_ATTACK_H_

#include "enemyPatternBase.h"

//*********************************************************************
//	“G‚ªUŒ‚‚µ‚Ä‚¢‚éŠÔ‚Ìˆ—
//*********************************************************************
class CEnemyPatternAttack : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_ATTACK_H_
