//======================================================================
//	“GUŒ‚(‘–‚è)ƒpƒ^[ƒ“ƒwƒbƒ_[enemyPatternAttackRun.h]
//
//======================================================================
#ifndef _ENEMY_PATTERN_ATTACK_RUN_H_
#define _ENEMY_PATTERN_ATTACK_RUN_H_

#include "enemyPatternBase.h"

//*********************************************************
//	‘–‚Á‚Ä‚¢‚é“G‚ÌUŒ‚’†‚Ìˆ—
//*********************************************************
class CEnemyPatternAttackRun : public CEnemyPatternBase
{
public:
	void Init(CEnemy* enemy)override;
	void Update(CEnemy* enemy)override;
};

#endif // !_ENEMY_PATTERN_ATTACK_RUN_H_
