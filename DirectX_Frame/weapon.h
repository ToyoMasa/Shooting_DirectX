//======================================================================
//	ïêäÌ[weapon.h]
//
//======================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

class CWeapon
{
public:
	CWeapon() {}
	~CWeapon() {}

	virtual void Init() {}
	virtual void Update() = 0;
	virtual void Shoot() = 0;

private:
	CSceneModel *m_Model;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_MuzzlePos;
	D3DXVECTOR3 m_Rot;
	float		m_Damage;
	int 		m_Rate;
	int			m_CoolDown;
};

#endif // !_WEAPON_H_
