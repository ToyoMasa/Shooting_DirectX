//======================================================================
//	パーティクルヘッダ[particle.h]
//
//======================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "billboard.h"

static const int PARTICLE_MAX = 10000;

class CParticle
{
public:
	CParticle()
	{
		for (int i = 0; i < PARTICLE_MAX; i++)
		{
			if (m_Particles[i] == NULL)
			{
				m_Particles[i] = this;

				m_Particles[i]->m_LifeSpan = 0;
				m_Particles[i]->m_Size = 0.0f;
				m_Particles[i]->m_SizeChangeValue = 0.0f;
				m_Particles[i]->m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_Particles[i]->m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_Particles[i]->m_VelocityAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_Particles[i]->m_Billboard = NULL;

				break;
			}
		}
	}
	~CParticle(){}

	void Init(
		int texId,
		int lifeSpan,
		float size,
		D3DXVECTOR3 pos);

	void Init(
		int texId,
		int lifeSpan,
		float size,
		D3DXVECTOR3 pos,
		D3DCOLOR color);

	void Uninit();
	void Update();

	static CParticle* Create(
		int texId,
		int lifeSpan,
		float size,
		D3DXVECTOR3 pos);
	static CParticle* Create(
		int texId,
		int lifeSpan,
		float size,
		D3DXVECTOR3 pos,
		D3DCOLOR color);

	void Release();
	
	static void ReleaseAll();

	static void UpdateAll();

	void SetSizeChageValue(float value) { m_SizeChangeValue = value; }
	void SetVelocity(D3DXVECTOR3 value) { m_Velocity = value; }
	void SetVelocityAcceleration(D3DXVECTOR3 value) { m_VelocityAcceleration = value; }

private:
	int m_TextureID;
	int m_LifeSpan;
	float m_Size;
	float m_SizeChangeValue;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Velocity;
	D3DXVECTOR3 m_VelocityAcceleration;
	CBillBoard* m_Billboard;
	static CParticle* m_Particles[PARTICLE_MAX];
};

#endif // !_PARTICLE_H_

