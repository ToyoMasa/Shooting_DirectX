//======================================================================
//	ƒVƒ‡ƒbƒgƒKƒ“[shotgun.h]
//
//======================================================================
#ifndef _SHOTGUN_H_
#define _SHOTGUN_H_

class CShotgun : public CWeapon
{
public:
	CShotgun() : CWeapon() {}
	~CShotgun() {}

	void Init(CSceneSkinMesh *parent);
	void Uninit()override;
	void Update()override;
	void Shoot()override;
	static CShotgun* Create(CSceneSkinMesh *parent);

private:

};

#endif // !_SHOTGUN_H_
