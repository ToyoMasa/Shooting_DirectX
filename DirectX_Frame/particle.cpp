//======================================================================
//	パーティクル　（2018/07/02）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene3D.h"
#include "texture.h"
#include "billboard.h"
#include "particle.h"

//======================================================================
//	静的メンバ変数の初期化
//======================================================================
CParticle* CParticle::m_Particles[PARTICLE_MAX] = { NULL };

void CParticle::Init(
	int texId,
	int lifeSpan,
	float size,
	D3DXVECTOR3 pos)
{
	m_TextureID = texId;
	m_LifeSpan = lifeSpan;
	m_Size = size;
	m_Pos = pos;

	m_Billboard = CBillBoard::Create(texId);
	m_Billboard->Set(m_TextureID ,m_Pos, m_Size, NORMAL);
}

void CParticle::Uninit()
{
	if (m_Billboard != NULL)
	{
		m_Billboard->Release();
	}
}

void CParticle::Update()
{
	m_LifeSpan--;
	if (m_LifeSpan <= 0)
	{
		Release();
		return;
	}

	m_Velocity += m_VelocityAcceleration;
	m_Pos += m_Velocity;

	m_Size += m_SizeChangeValue;

	m_Billboard->Set(m_TextureID, m_Pos, m_Size, NORMAL);
}

void CParticle::UpdateAll()
{
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (CParticle::m_Particles[i] != NULL)
		{
			CParticle::m_Particles[i]->Update();
		}
	}
}

CParticle* CParticle::Create(
	int texId,
	int lifeSpan,
	float size,
	D3DXVECTOR3 pos)
{
	CParticle* particle = new CParticle();
	particle->Init(texId, lifeSpan, size, pos);

	return particle;
}

void CParticle::Release()
{
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (CParticle::m_Particles[i] == this)
		{
			Uninit();
			delete CParticle::m_Particles[i];
			CParticle::m_Particles[i] = NULL;
			break;
		}
	}
}

void CParticle::ReleaseAll()
{
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		if (CParticle::m_Particles[i] != NULL)
		{
			CParticle::m_Particles[i]->Release();
		}
	}
}