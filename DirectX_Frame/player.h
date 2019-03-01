//======================================================================
//	�v���C���[[player.h]
//
//======================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "character.h"
#include "sceneSkinMesh.h"
#include "effekseer.h"
#include "sound.h"
#include "tutorialBase.h"

//*********************************************************
//	��{�ړ���
//*********************************************************
static const float PLAYER_MOVE_SPEED = 0.05f;

//*********************************************************
//	ADS���̈ړ����x�ቺ�{��
//*********************************************************
static const float PLAYER_ADS_MOVE_SPEED = 0.75f;

//*********************************************************
//	�����Ă��鎞�̍��E�ړ��{��
//*********************************************************
static const float PLAYER_DASH_MOVE_SPEED_X = 0.6f;

//*********************************************************
//	�����Ă��鎞�̑O�����ړ��{��
//*********************************************************
static const float PLAYER_DASH_MOVE_SPEED_Z = 1.5f;

//*********************************************************
//	�J������]��
//*********************************************************
static const float VALUE_ROTATE_MOUSE = 0.003f;

//*********************************************************
//	�v���C���[�̓����蔻��J�v�Z���̔��a
//*********************************************************
static const float PLAYER_CUPSULE_RAD = 0.5f;

//*********************************************************
//	�������Ă��镐��̐�
//*********************************************************
static const int HAVE_WEAPON = 2;

class CScene2D;
class CWeapon;
class CPlayerPatternBase;
class CNumber;

//*********************************************************
//	�v���C���[�N���X
//*********************************************************
class CPlayer : public CCharacter
{
public:
	CPlayer::CPlayer() : CCharacter()
	{
		m_Type = CHARACTER_PLAYER;
		m_CameraLength = 2.0f;
		m_Camera = NULL;
		m_SoundReload = NULL;
		m_isPreDeath  = false;
		m_isADS = false;
		m_Pattern = NULL;
		m_DamagedEffect = NULL;
		m_Tutorial = NULL;
		m_Rader = NULL;
		m_Radio_Wave = NULL;
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
	void ChangeWeapon();
	void Move(const float& moveX, const float& moveZ);
	void MoveAir(const float& moveX, const float& moveY, const float& moveZ);
	void ADS();
	void SetADS(bool ads);
	void Reload();
	void SetWeaponADS(bool ads);
	void PlayReload();
	void ChangePattern(CPlayerPatternBase* next);
	void ChangeTutorial(CTutorialBase* next);
	float GetJumpPower() { return m_JumpPower; }
	void SetJumpPower(float power) { m_JumpPower = power; }
	float GetUpValue() { return m_UpValue; }
	void SetUpValue(float value) { m_UpValue = value; }
	CWeapon* GetUsingWeapon() { return m_UsingWeapon; }

	void Damaged(float damage)override;

private:
	D3DXMATRIX			m_LocalLocation;
	CCamera*			m_Camera;
	D3DXVECTOR3			m_LocalCameraPos;
	float				m_CameraLength;
	float				m_JumpPower;
	float				m_UpValue;
	CSound*				m_SoundReload;
	bool				m_isPreDeath;
	bool				m_isADS;
	CWeapon*			m_UsingWeapon;
	CWeapon*			m_Weapon[HAVE_WEAPON];
	CEffekseer*			m_BloodEffect;
	CScene2D*			m_DamagedEffect;
	CScene2D*			m_AmmoBackScreen;
	CScene2D*			m_BulletIcon;
	CScene2D*			m_Mission;
	CScene2D*			m_Rader;
	CScene2D*			m_Radio_Wave;
	CPlayerPatternBase* m_Pattern;
	CTutorialBase*		m_Tutorial;
	CNumber*			m_AmmoNum;
};

#endif // !_PLAYER_H_
