//======================================================================
//	パーティクルエミッター　（2018/07/02）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene3D.h"
#include "texture.h"
#include "billboard.h"
#include "particle.h"
#include "emitter.h"

CParticleEmitter *CParticleEmitter::m_Emitters[EMITTER_MAX] = { NULL };

void CParticleEmitter::Init(
	int texId,
	int lifeSpan,
	int spawnSpan,
	int numParticle,
	float size,
	float sizechange,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocityStart,
	D3DXVECTOR3 velocityEnd,
	D3DXVECTOR3 accelerate,
	bool loop)
{
	m_TextureID = texId;
	m_LifeSpan = lifeSpan;
	m_SpawnSpan = spawnSpan;
	m_MaxParticle = numParticle;
	m_ParticleSize = size;
	m_SizeChangeValue = sizechange;
	m_Pos = pos;
	m_VelocityRangeStart = velocityStart;
	m_VelocityRanegeEnd = velocityEnd;
	m_VelocityAcceleration = accelerate;
	m_Loop = loop;
}

void CParticleEmitter::Uninit()
{

}

void CParticleEmitter::Release()
{
	for (int i = 0; i < EMITTER_MAX; i++)
	{
		if (m_Emitters[i] == this)
		{
			Uninit();
			m_Emitters[i] = NULL;
			delete this;
			break;
		}
	}
}

void CParticleEmitter::Update()
{
	if (m_FrameCount % m_SpawnSpan == 0)
	{
		CParticle* particle = CParticle::Create(m_TextureID, m_LifeSpan, m_ParticleSize, m_Pos);
		particle->SetSizeChageValue(m_SizeChangeValue);

		D3DXVECTOR3 velocity = m_VelocityRanegeEnd - m_VelocityRangeStart;
		if ((int)(velocity.x * 1000) == 0)
		{
			velocity.x = 0.0f;
		}
		else
		{
			velocity.x = m_VelocityRangeStart.x + (rand() % (int)(velocity.x * 1000)) / 1000.0f;
		}
		if ((int)(velocity.y * 1000) == 0)
		{
			velocity.y = 0.0f;
		}
		else
		{
			velocity.y = m_VelocityRangeStart.y + (rand() % (int)(velocity.y * 1000)) / 1000.0f;
		}
		if ((int)(velocity.z * 1000) == 0)
		{
			velocity.z = 0.0f;
		}
		else
		{
			velocity.z = m_VelocityRangeStart.z + (rand() % (int)(velocity.z * 1000)) / 1000.0f;
		}

		particle->SetVelocity(velocity);
		particle->SetVelocityAcceleration(m_VelocityAcceleration);

		m_SpawnCount++;
	}

	if (!m_Loop)
	{
		if (m_SpawnCount >= m_MaxParticle)
		{
			// 自身を解放
			Release();
			return;
		}
	}
	else
	{
		m_SpawnCount = 0;
	}

	m_FrameCount++;
}

void CParticleEmitter::UpdateAll()
{
	for (int i = 0; i < EMITTER_MAX; i++)
	{
		if (m_Emitters[i] != NULL)
		{
			m_Emitters[i]->Update();
		}
	}
}

void CParticleEmitter::ReleaseAll()
{
	for (int i = 0; i < EMITTER_MAX; i++)
	{
		if (m_Emitters[i] != NULL)
		{
			m_Emitters[i]->Release();
		}
	}
}

CParticleEmitter* CParticleEmitter::Create(
	int texId,
	int lifeSpan,
	int spawnSpan,
	int numParticle,
	float size,
	float sizechange,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocityStart,
	D3DXVECTOR3 velocityEnd,
	D3DXVECTOR3 accelerate,
	bool loop)
{
	CParticleEmitter* emitter = new CParticleEmitter();
	emitter->Init(
		texId,
		lifeSpan,
		spawnSpan,
		numParticle,
		size,
		sizechange,
		pos,
		velocityStart,
		velocityEnd,
		accelerate,
		loop
	);

	return emitter;
}
