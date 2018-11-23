//======================================================================
//	武器クラス [weapon.cpp]　（2018/9/27）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene2D.h"
#include "collision.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "player.h"
#include "weapon.h"
#include "debug.h"

CWeapon *CWeapon::m_Weapons[WEAPON_MAX] = { NULL };

void CWeapon::Release()
{
	for (int i = 0; i < WEAPON_MAX; i++)
	{
		if (m_Weapons[i] == this)
		{
			Uninit();
			m_Weapons[i] = NULL;
			delete this;
			break;
		}
	}
}

void CWeapon::UpdateAll()
{
	for (int i = 0; i < WEAPON_MAX; i++)
	{
		if (m_Weapons[i] != NULL)
		{
			m_Weapons[i]->Update();
			m_Weapons[i]->RecoilUpdate();
		}
	}
}

void CWeapon::ReleaseAll()
{
	for (int i = 0; i < WEAPON_MAX; i++)
	{
		if (m_Weapons[i] != NULL)
		{
			m_Weapons[i]->Release();
			m_Weapons[i] = NULL;
		}
	}
}

void CWeapon::SetActive(bool active) 
{
	m_Model->SetVisible(active);
	//m_Crosshair->SetVisible(active);
	m_BulletDebug->SetVisible(active);
	m_isActive = active;
}
