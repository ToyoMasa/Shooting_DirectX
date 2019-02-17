//======================================================================
//	ƒ‰ƒCƒtƒ‹[rifle.h]
//
//======================================================================
#ifndef _RIFLE_H_
#define _RIFLE_H_

class CSound;

static const int RIFLE_SHOT_SE_NUM = 6;

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
	void SetADS(bool ads)override;
	void Recoil(float recoilX, float recoilY)override;
	void RecoilUpdate()override;
	void ReleaseTrigger()override;
	void SoundShot()override;

private:
	CSound* m_ShotSE[RIFLE_SHOT_SE_NUM];
	CScene2D* m_Reticle;
};

#endif // !_RIFLE_H_

