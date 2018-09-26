//======================================================================
//	プレイヤーに向かうアクション[moveToPlayer.h]
//
//======================================================================
#ifndef _MOVETOPLAYER_H_
#define _MOVETOPLAYER_H_

#include "actionBase.h"
#include "root.h"

class CActionMoveToPlayer : public CActionBase
{
public:
	CActionMoveToPlayer(CCharacter* chara) :CActionBase(chara) {}
	~CActionMoveToPlayer() {}

	void Init(CCharacter* target, float speed);
	void Update();
	static CActionMoveToPlayer* Create(CCharacter* chara, CCharacter* target, float speed);
private:
	float m_Speed;
	CCharacter* m_Target;
};

#endif // !_MOVETOPLAYER_H_
