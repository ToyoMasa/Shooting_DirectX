//======================================================================
//	�ŏ��̓G�����C�x���g[eventStartEnemy.h]
//
//======================================================================
#ifndef _EVENT_START_ENEMY_H_
#define _EVENT_START_ENEMY_H_

#include "event.h"

//*********************************************************
//	�G�̐������n�߂�
//*********************************************************
class CEventStartEnemy : public CEvent
{
public:
	CEventStartEnemy() {}
	~CEventStartEnemy() {}

	void Init()override;
	void Uninit()override;
	void Update()override;
	static CEventStartEnemy* Create();

private:
	D3DXVECTOR3 m_Pos;
};


#endif !_EVENT_START_ENEMY_H_
