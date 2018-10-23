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
#include "game.h"

#define RECOILE_PATTERN_X ((0.1 * m_CountFire * (-350 + rand() % 1000) * 0.001))
#define RECOILE_PATTERN_Y ((0.1 * m_CountFire * (rand() % 1000) * 0.001))

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
	m_Crosshair->SetVisible(active);
	m_BulletDebug->SetVisible(active);
	m_isActive = active;
}

void CWeapon::SetADS(bool ads)
{
	m_Crosshair->SetVisible(ads);
	m_isADS = ads;
}

void CWeapon::ChangeCrosshair(int nextTex)
{
	if (m_Crosshair != NULL)
	{
		m_Crosshair->Release();
		m_Crosshair = CScene2D::Create(nextTex, 32, 32);
	}
}

void CWeapon::Recoil(float recoilX, float recoilY)
{
	if (m_CountFire < 10)
	{
		m_CountFire++;

		CModeGame::GetPlayer()->Rotate(recoilX * RECOILE_PATTERN_X, -recoilY * RECOILE_PATTERN_Y);
		m_TotalRecoilX += recoilX * RECOILE_PATTERN_X;
		m_TotalRecoilY += recoilY * RECOILE_PATTERN_Y;
	}
	else
	{
		CModeGame::GetPlayer()->Rotate(recoilX * RECOILE_PATTERN_X, -recoilY * 0.1f);
		m_TotalRecoilX += recoilX * RECOILE_PATTERN_X;
		m_TotalRecoilY += recoilY * 0.1f;
	}
}

void CWeapon::RecoilUpdate()
{
	if (m_CountFire == 0)
	{
		if (m_RecoilCount > 0)
		{
			CModeGame::GetPlayer()->Rotate(-m_RecoilX, m_RecoilY);
			m_RecoilCount--;
		}
		else
		{
			m_RecoilX = 0.0f;
			m_RecoilY = 0.0f;
			m_TotalRecoilX = 0.0f;
			m_TotalRecoilY = 0.0f;
		}
	}

	ImGui::Begin("FireCount", 0);
	ImGui::Text("%d", m_CountFire);
	ImGui::End();
}

void CWeapon::ReleaseTrigger()
{
	if (m_CountFire < 3)
	{
		m_CountFire = 0;
		m_RecoilCount = 1;
		m_RecoilX = m_TotalRecoilX;
		m_RecoilY = m_TotalRecoilY;
	}
	else
	{
		m_CountFire = 0;
		m_RecoilCount = 5;
		m_RecoilX = m_TotalRecoilX / 5.0f;
		m_RecoilY = m_TotalRecoilY / 5.0f;
	}
}
