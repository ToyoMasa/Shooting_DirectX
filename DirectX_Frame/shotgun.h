//======================================================================
//	ƒVƒ‡ƒbƒgƒKƒ“[shotgun.h]
//
//======================================================================
#ifndef _SHOTGUN_H_
#define _SHOTGUN_H_

class CSound;

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
	void SetADS(bool ads)override;
	void ChangeCrosshair(int nextTex)override;
	void Recoil(float recoilX, float recoilY)override;
	void RecoilUpdate()override;
	void ReleaseTrigger()override;
	void SoundShot()override;

private:
	CSound* m_ShotSE;
	bool m_isReleaseTrigger;
};

#endif // !_SHOTGUN_H_
