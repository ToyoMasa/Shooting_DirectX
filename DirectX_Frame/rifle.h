//======================================================================
//	ƒ‰ƒCƒtƒ‹[rifle.h]
//
//======================================================================
#ifndef _RIFLE_H_
#define _RIFLE_H_

class CRifle : CWeapon
{
public:
	CRifle() {}
	~CRifle() {}

	void Init()override;
	void Update()override;
	void Shoot()override;
	static CRifle* Create();

private:

};

#endif // !_RIFLE_H_

