//======================================================================
//	ÉvÉåÉCÉÑÅ[[player.h]
//
//======================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"
#include "sceneSkinMesh.h"
#include "Effekseer.h"
#include "sound.h"

static const float PLAYER_MOVE_SPEED = 0.08f;

class CScene2D;

class CPlayer : public CCharacter
{
public:
	CPlayer::CPlayer() : CCharacter()
	{
		m_Type = CHARACTER_PLAYER;
		m_CameraLength = 2.0f;
		m_Camera = NULL;
		m_Text_Attack = NULL;
		m_Knife = NULL;
		m_Hit = NULL;
		m_isPreDeath  = false;
		m_isPreAttack = false;
		m_isGameOver = false;
	}
	~CPlayer(){}

	void Init(int modelId, D3DXVECTOR3 spawnPos);
	void Uninit()override;
	void Update()override;
	void Draw();
	CCamera* GetCamera() { return m_Camera; }
	bool GetPlayerDeath() { return m_isPreDeath; }
	static CPlayer* Create(int modelId, D3DXVECTOR3 spawnPos);
	void Attack();
	void Death();
	void Rotate(D3DXVECTOR3 vec);
	void Rotate(float horizontal, float vertical);

private:
	D3DXMATRIX m_LocalLocation;
	CCamera* m_Camera;
	D3DXVECTOR3 m_LocalCameraPos;
	float m_CameraLength;
	CScene2D* m_Text_Attack;
	CScene2D* m_Caution;
	Capsule m_AttackingCollsion;
	CSound* m_Knife;
	CSound* m_Hit;
	bool m_isPreDeath;
	bool m_isPreAttack;
	bool m_isGameOver;

	CEffekseer* m_BloodEffect;
};

#endif // !_PLAYER_H_
