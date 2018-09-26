//======================================================================
//	アクションの基底クラス[actionBase.h]
//
//======================================================================
#ifndef _ACTIONBASE_H_
#define _ACTIONBASE_H_

static const int ACTION_MAX = 100;

class CActionBase
{
public:
	CActionBase(CCharacter* chara);
	~CActionBase() {}

	virtual void Init() {};
	virtual void Update() = 0;
	static void ReleaseAll();
	void Release();
protected:
	CCharacter* m_Character;
	static CActionBase* m_ActionBase[ACTION_MAX];
};

#endif // !_ACTIONBASE_H_
