//======================================================================
//	イベント　（2018/11/20)
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "event.h"

CEvent *CEvent::m_Events[EVENT_NUM_MAX] = { NULL };

CEvent::CEvent()
{
	for (int i = 0; i < EVENT_NUM_MAX; i++)
	{
		if (m_Events[i] == NULL)
		{
			m_Events[i] = this;
			break;
		}
	}
}

void CEvent::Release()
{

	for (int i = 0; i < EVENT_NUM_MAX; i++)
	{
		if (m_Events[i] == this)
		{
			Uninit();
			m_Events[i] = NULL;
			delete this;
			break;
		}
	}
}

void CEvent::UpdateAll()
{
	for (int i = 0; i < EVENT_NUM_MAX; i++)
	{
		if (m_Events[i] != NULL)
		{
			m_Events[i]->Update();
		}
	}
}

void CEvent::ReleaseAll()
{
	for (int i = 0; i < EVENT_NUM_MAX; i++)
	{
		if (m_Events[i] != NULL)
		{
			m_Events[i]->Release();
		}
	}
}
