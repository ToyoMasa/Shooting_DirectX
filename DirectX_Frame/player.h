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

static const float PLAYER_MOVE_SPEED = 0.06f;
static const float PLAYER_JUMP_POWER = 1.8f;
static const float PLAYER_AIR_MOVE_SPEED = 0.9f;
static const float PLAYER_ADS_MOVE_SPEED = 0.75f;
static const float PLAYER_DASH_MOVE_SPEED_X = 0.6f;
static const float PLAYER_DASH_MOVE_SPEED_Z = 1.5f;
static const float VALUE_ROTATE_MOUSE = 0.003f;
static const float PLAYER_CUPSULE_RAD = 0.5f;

static const int HAVE_WEAPON = 2;

class CScene2D;
class CWeapon;
class CPlayerPatternBase;
class CNumber;

class CPlayer : public CCharacter
{
public:
	CPlayer::CPlayer() : CCharacter()
	{
		m_Type = CHARACTER_PLAYER;
		m_CameraLength = 2.0f;
		m_Camera = NULL;
		m_Knife = NULL;
		m_Hit = NULL;
		m_isPreDeath  = false;
		m_isADS = false;
		m_Pattern = NULL;
		m_DamagedEffect = NULL;
	}
	~CPlayer(){}

	void Init(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos);
	void Uninit()override;
	void Update()override;
	void Draw();
	CCamera* GetCamera() { return m_Camera; }
	bool GetPlayerDeath() { return m_isPreDeath; }
	static CPlayer* Create(SKINMESH_MODEL_ID modelId, D3DXVECTOR3 spawnPos);
	void Shoot();
	void TriggerRelease();
	void Death();
	void Rotate(D3DXVECTOR3 vec);
	void Rotate(const float& horizontal, const float& vertical);
	void ChangeWeapon(const int& id);
	void Move(const float& moveX, const float& moveZ);
	void MoveAir(const float& moveX, const float& moveY, const float& moveZ);
	void ADS();
	void SetADS(bool ads);
	void Reload();
	void SetWeaponADS(bool ads);
	void ChangePattern(CPlayerPatternBase* next);
	float GetJumpPower() { return m_JumpPower; }
	void SetJumpPower(float power) { m_JumpPower = power; }
	float GetUpValue() { return m_UpValue; }
	void SetUpValue(float value) { m_UpValue = value; }
	int GetShortestPoint() { return m_ShortestPoint; }
	CWeapon* GetUsingWeapon() { return m_UsingWeapon; }

	void Damaged(float damage)override;

private:
	D3DXMATRIX	m_LocalLocation;
	CCamera*	m_Camera;
	D3DXVECTOR3 m_LocalCameraPos;
	float		m_CameraLength;
	float		m_JumpPower;
	float		m_UpValue;
	CSound*		m_Knife;
	CSound*		m_Hit;
	bool		m_isPreDeath;
	bool		m_isADS;
	CWeapon*	m_UsingWeapon;
	CWeapon*	m_Weapon[HAVE_WEAPON];
	CEffekseer* m_BloodEffect;
	CScene2D*	m_DamagedEffect;
	CPlayerPatternBase* m_Pattern;
	CNumber*	m_AmmoNum;
	int			m_ShortestPoint;
};

#endif // !_PLAYER_H_
