//======================================================================
//	アイテムクラス[targetCapsule.h]
//
//======================================================================
#ifndef _TARGETCAPSULE_H_
#define _TARGETCAPSULE_H_

#include "item.h"

class CTargetCapsule : public CItem
{
public:
	CTargetCapsule() : CItem() {}
	~CTargetCapsule() {}

	void Init(D3DXVECTOR3 pos);
	void Uninit()override;
	void Update()override;
	void HitBullet();
	static CTargetCapsule* Create(D3DXVECTOR3 pos);

private:
	CSceneModel*	m_Model;
	Capsule			m_CollisionCapsule;
	float			m_Life;
};

#endif // !_TARGETCAPSULE_H_
