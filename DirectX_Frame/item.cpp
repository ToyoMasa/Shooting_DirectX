//======================================================================
//	アイテム　（2018/11/23）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "item.h"

CItem *CItem::m_Items[ITEM_MAX] = { NULL };

CItem::CItem()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_Items[i] == NULL)
		{
			m_Items[i] = this;

			break;
		}
	}
}

void CItem::UpdateAll()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_Items[i])
		{
			m_Items[i]->Update();
		}
	}
}

void CItem::Release()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_Items[i] == this)
		{
			Uninit();
			m_Items[i] = NULL;
			delete this;
			break;
		}
	}
}

void CItem::ReleaseAll()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (m_Items[i])
		{
			m_Items[i]->Release();
		}
	}
}
