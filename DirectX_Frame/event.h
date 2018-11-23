//======================================================================
//	ƒCƒxƒ“ƒg[event.h]
//
//======================================================================
#ifndef _EVENT_H_
#define _EVENT_H_

#include "gameobject.h"
#include <d3dx9.h>

class CEvent : public CGameObject
{
public:
	CEvent() {}
	~CEvent() {}

	virtual void Update() = 0;
	virtual void isCollision() = 0;

private:
	D3DXVECTOR3 m_Pos;
};


#endif !_EVENT_H_
