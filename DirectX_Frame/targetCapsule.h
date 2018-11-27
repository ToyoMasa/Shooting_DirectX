//======================================================================
//	アイテムクラス[targetCapsule.h]
//
//======================================================================
#ifndef _TARGETCAPSULE_H_
#define _TARGETCAPSULE_H_

#include "item.h"

class CEffekseer;

class CTargetCapsule : public CItem
{
public:
	CTargetCapsule() : CItem() { m_Type = ITEM_TYPE_CAPSULE; }
	~CTargetCapsule() {}

	void Init(D3DXVECTOR3 pos);
	void Uninit()override;
	void Update()override;
	void HitBullet();
	Capsule GetCapsule() { return m_CollisionCapsule; }
	static CTargetCapsule* Create(D3DXVECTOR3 pos);

private:
	CSceneModel*	m_Model;
	Capsule			m_CollisionCapsule;
	CEffekseer*		m_Smoke[2];
	CEffekseer*		m_Flame;
	float			m_Life;
	bool			m_isDestroyed;
};

#endif // !_TARGETCAPSULE_H_
