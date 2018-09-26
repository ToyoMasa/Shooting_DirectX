//======================================================================
//	ÇªÇÃèÍë“ã@[wait.h]
//
//======================================================================
#ifndef _WAIT_H_
#define _WAIT_H_

#include "actionBase.h"
#include "root.h"

class CActionWait : public CActionBase
{
public:
	CActionWait(CCharacter* chara) :CActionBase(chara) {}
	~CActionWait() {}

	void Init();
	void Update();
	static CActionWait* Create(CCharacter* chara);
private:
};

#endif // !_WAIT_H_
