//======================================================================
//	ïêäÌ[weapon.h]
//
//======================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

class CWeapon
{
public:
	CWeapon()
	{

	}
	~CWeapon()
	{

	}

	virtual void Init() {}
	virtual void Shoot() = 0;

private:
	CSceneModel *m_Model;
	D3DXVECTOR3 m_LocalPos;
	D3DXVECTOR3 m_LocalRot;
	float		m_Damage;
	float		m_Rate;
};

#endif // !_WEAPON_H_
