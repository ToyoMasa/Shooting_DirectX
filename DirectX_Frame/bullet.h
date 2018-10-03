#ifndef _BULLET_H_
#define _BULLET_H_

#include "billboard.h"
#include "debug.h"

static const int BULLET_MAX = 1000;

class CBullet
{
public:
	CBullet::CBullet()
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (m_Bullets[i] == NULL)
			{
				m_Bullets[i] = this;
				break;
			}
		}

		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Forward =	D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Speed = 1.0f;
		m_Move = 0.0f;
		m_Range = 1.0f;
		m_Damage = 10;
		m_Billboard = NULL;
	}
	~CBullet(){}

	void Init(D3DXVECTOR3 spawnPos, D3DXVECTOR3 vec, float speed, float range, int damage);
	void Uninit();
	void Update();
	void Draw();
	void Release();
	static void UpdateAll();
	static void ReleaseAll();
	static CBullet* Create(D3DXVECTOR3 spawnPos, D3DXVECTOR3 vec, float speed, float range, int damage);
private:
	D3DXVECTOR3		m_Pos;
	D3DXVECTOR3		m_OldPos;
	D3DXVECTOR3		m_Forward;
	D3DXVECTOR3		m_Right;
	CBillBoard		*m_Billboard;
	CDebugSphere	*m_Debug;
	float			m_Speed;
	float			m_Move;
	float			m_Range;
	int				m_Damage;

	static CBullet *m_Bullets[BULLET_MAX];
};

#endif // !_BULLET_H_

