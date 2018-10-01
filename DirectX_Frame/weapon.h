//======================================================================
//	ïêäÌ[weapon.h]
//
//======================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

static const int WEAPON_MAX = 5;

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
	}
	~CWeapon() {}

	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Shoot() = 0;
	void Release();
	static void UpdateAll();
	static void ReleaseAll();

protected:
	CSceneModel		*m_Model;
	CScene2D		*m_Crosshair;
	CSceneSkinMesh	*m_Parent;
	D3DXVECTOR3		m_Pos;
	D3DXVECTOR3		m_Rot;
	D3DXVECTOR3		m_Scale;
	D3DXVECTOR3		m_MuzzlePos;
	D3DXMATRIX		m_ParentMatrix;
	D3DXMATRIX		m_LocalMatrix;
	float			m_Damage;
	int 			m_Rate;
	int				m_CoolDown;

	static CWeapon	*m_Weapons[WEAPON_MAX];
};

#endif // !_WEAPON_H_
