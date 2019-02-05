//======================================================================
//	ƒCƒxƒ“ƒg[event.h]
//
//======================================================================
#ifndef _EVENT_H_
#define _EVENT_H_

#include "gameobject.h"
#include <d3dx9.h>

static const int EVENT_NUM_MAX = 20;

class CEvent
{
public:
	CEvent();
	~CEvent() {}

	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	void Release();

	static void UpdateAll();
	static void ReleaseAll();

private:
	static CEvent* m_Events[EVENT_NUM_MAX];

protected:
	D3DXVECTOR3 m_Pos;
};


#endif !_EVENT_H_
