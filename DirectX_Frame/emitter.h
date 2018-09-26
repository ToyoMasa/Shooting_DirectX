//======================================================================
//	エミッターヘッダ[emitter.h]
//
//======================================================================
#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "billboard.h"

class CParticleEmitter
{
public:
	CParticleEmitter()
	{
		m_LifeSpan = 0;
		m_SpawnSpan = 0;
		m_SpawnCount = 0;
		m_FrameCount = 0;
		m_MaxParticle = 0;
		m_ParticleSize = 1.0f;
		m_SizeChangeValue = 0.0f;
		m_Loop = false;
		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_VelocityRangeStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_VelocityRanegeEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_VelocityAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	~CParticleEmitter() {}

	void Init(
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
		bool loop);
	void Uninit();
	void Update();

private:

	int m_TextureID;
	int m_LifeSpan;
	int m_SpawnSpan;
	int m_SpawnCount;
	int m_FrameCount;
	int m_MaxParticle;
	float m_ParticleSize;
	float m_SizeChangeValue;
	bool m_Loop;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_VelocityRangeStart;
	D3DXVECTOR3 m_VelocityRanegeEnd;
	D3DXVECTOR3 m_VelocityAcceleration;
};

#endif // !_EMITTER_H_
