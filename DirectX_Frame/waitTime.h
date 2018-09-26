//======================================================================
//	éwíËéûä‘ï™ÇªÇÃèÍë“ã@[waitTime.h]
//
//======================================================================
#ifndef _WAITTIME_H_
#define _WAITTIME_H_

#include "actionBase.h"
#include "root.h"

class CActionWaitTime : public CActionBase
{
public:
	CActionWaitTime(CCharacter* chara) :CActionBase(chara) {}
	~CActionWaitTime() {}

	void Init(int waitFrame, CActionBase* nextAction);
	void Update();
	static CActionWaitTime* Create(CCharacter* chara, int waitFrame, CActionBase* nextAction);
private:
	int m_WaitCount;
	CActionBase* m_NextAction;
};

#endif // !_WAITTIME_H_
