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
	D3DXVECTOR3 Accelerate,
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
	m_VelocityAcceleration = Accelerate;
	m_Loop = loop;
}

void CParticleEmitter::Uninit()
{

}

void CParticleEmitter::Update()
{
	if (m_FrameCount % m_SpawnSpan == 0)
	{
		CParticle* particle = CParticle::Create(m_TextureID, m_LifeSpan, m_ParticleSize, m_Pos);
		particle->SetSizeChageValue(m_SizeChangeValue);

		D3DXVECTOR3 velocity = m_VelocityRanegeEnd - m_VelocityRangeStart;
		velocity.x = m_VelocityRangeStart.x + (rand() % (int)(velocity.x * 1000)) / 1000.0f;
		velocity.y = m_VelocityRangeStart.y + (rand() % (int)(velocity.y * 1000)) / 1000.0f;
		velocity.z = m_VelocityRangeStart.z + (rand() % (int)(velocity.z * 1000)) / 1000.0f;

		particle->SetVelocity(velocity);
		particle->SetVelocityAcceleration(m_VelocityAcceleration);

		m_SpawnCount++;
	}

	if (!m_Loop)
	{
		if (m_SpawnCount >= m_MaxParticle)
		{
			// 自身を解放
		}
	}
	else
	{
		m_SpawnCount = 0;
	}

	m_FrameCount++;
}
