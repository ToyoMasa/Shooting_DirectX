//======================================================================
//	ƒ‰ƒ“ƒ_ƒ€‚È’n“_‚ÖˆÚ“®[moveToRandom.h]
//
//======================================================================
#ifndef _MOVETORANDOM_H_
#define _MOVETORANDOM_H_

#include "actionBase.h"
#include "root.h"

class CActionMoveToRandom : public CActionBase
{
public:
	CActionMoveToRandom(CCharacter* chara) :CActionBase(chara) {}
	~CActionMoveToRandom() {}

	void Init(float min, float maxfloat, float speed);
	void Update();
	static CActionMoveToRandom* Create(CCharacter* chara,float min, float max, float speed);
private:
	float m_Speed;
	float m_MinMove;
	float m_MaxMove;
	D3DXVECTOR3 m_TargetPos;
};

#endif // !_MOVETORANDOM_H_
