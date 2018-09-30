//======================================================================
//	���C�t��[rifle.h]
//
//======================================================================
#ifndef _RIFLE_H_
#define _RIFLE_H_

class CRifle : public CWeapon
{
public:
	CRifle() : CWeapon() {}
	~CRifle() {}

	void Init(CSceneSkinMesh *parent);
	void Uninit()override;
	void Update()override;
	void Shoot()override;
	static CRifle* Create(CSceneSkinMesh *parent);

private:

};

#endif // !_RIFLE_H_

