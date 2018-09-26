//======================================================================
//	アクション基底クラス　（2018/07/18）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "texture.h"
#include "billboard.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "field.h"
#include "action.h"

CActionBase *CActionBase::m_ActionBase[ACTION_MAX] = { NULL };

CActionBase::CActionBase(CCharacter* chara)
{
	m_Character = chara;

	for (int i = 0; i < ACTION_MAX; i++)
	{
		if (m_ActionBase[i] == NULL)
		{
			m_ActionBase[i] = this;
			break;
		}
	}
}

void CActionBase::ReleaseAll()
{
	for (int i = 0; i < ACTION_MAX; i++)
	{
		if (m_ActionBase[i] != NULL)
		{
			m_ActionBase[i]->Release();
		}
	}
}

void CActionBase::Release()
{
	for (int i = 0; i < ACTION_MAX; i++)
	{
		if (m_ActionBase[i] == this)
		{
			m_ActionBase[i] = NULL;
			delete this;
			break;
		}
	}
}
