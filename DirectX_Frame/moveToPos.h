//======================================================================
//	Žw’è’n“_‚ÖˆÚ“®[moveToPos.h]
//
//======================================================================
#ifndef _MOVETOPOS_H_
#define _MOVETOPOS_H_

#include "actionBase.h"
#include "root.h"

class CActionMoveToPos : public CActionBase
{
public:
	CActionMoveToPos(CCharacter* chara) :CActionBase(chara) {}
	~CActionMoveToPos() {}

	void Init(int rootId, float speed);
	void Update();
	static CActionMoveToPos* Create(CCharacter* chara, int rootId, float speed);
private:
	float m_Speed;
	int m_RootID;
	D3DXVECTOR3 m_TargetPos;
};

#endif // !_MOVETOPOS_H_
