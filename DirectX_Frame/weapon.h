//======================================================================
//	ïêäÌ[weapon.h]
//
//======================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

static const int WEAPON_MAX = 5;
static const int FLASH_MAX = 1;

class CDebug;
class CBillBoard;

class CWeapon
{
public:
	CWeapon()
	{
		for (int i = 0; i < WEAPON_MAX; i++)
		{
			if (m_Weapons[i] == NULL)
			{
				m_Weapons[i] = this;
				break;
			}
		}

		m_Model = NULL;
		m_Crosshair = NULL;
		m_Parent = NULL;
		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		D3DXMatrixIdentity(&m_ParentMatrix);
		D3DXMatrixIdentity(&m_MuzzleMatrix);
		m_BulletDebug = NULL;
		m_isActive = false;
		m_isADS = false;

		m_Flash = NULL;
		m_isFlash = false;
	}
	~CWeapon() {}

	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Shoot() = 0;
	void Release();
	static void UpdateAll();
	static void ReleaseAll();
	void SetActive(bool active);
	void SetADS(bool ads);

	void ChangeCrosshair(int nextTex);

	CSceneModel*	GetModel() { return m_Model; }
	CScene2D*		GetCrosshair() { return m_Crosshair; }
	CSceneSkinMesh* GetParent() { return m_Parent; }
	D3DXVECTOR3		GetPos() { return m_Pos; }
	D3DXVECTOR3		GetRot() { return m_Rot; }
	D3DXVECTOR3		GetScale() { return m_Scale; }
	D3DXVECTOR3		GetMuzzlePos() { return m_MuzzlePos; }
	D3DXMATRIX		GetParentMatrix() { return m_ParentMatrix; }
	D3DXMATRIX		GetMuzzleMatrix() { return m_MuzzleMatrix; }
	CDebug*			GetBulletDebug() { return m_BulletDebug; }
	CBillBoard*		GetFlash() { return m_Flash; }
	int				GetFlashAlpha() { return m_FlashAlpha; }
	float			GetDamage() { return m_Damage; }
	float 			GetRate() { return m_Rate; }
	float			GetCoolDown() { return m_CoolDown; }
	bool			GetisActive() { return m_isActive; }
	bool			GetisADS() { return m_isADS; }

protected:
	CSceneModel		*m_Model;
	CScene2D		*m_Crosshair;
	CSceneSkinMesh	*m_Parent;
	D3DXVECTOR3		m_Pos;
	D3DXVECTOR3		m_Rot;
	D3DXVECTOR3		m_Scale;
	D3DXVECTOR3		m_MuzzlePos;
	D3DXMATRIX		m_ParentMatrix;
	D3DXMATRIX		m_MuzzleMatrix;
	CDebug*			m_BulletDebug;
	CBillBoard*		m_Flash;
	int				m_FlashAlpha;
	float			m_Damage;
	float 			m_Rate;
	float			m_CoolDown;
	bool			m_isActive;
	bool			m_isADS;
	bool			m_isFlash;

	static CWeapon	*m_Weapons[WEAPON_MAX];
};

#endif // !_WEAPON_H_
