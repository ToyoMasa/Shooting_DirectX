//======================================================================
//	ミッション目標カプセルクラス [targetCapsule.cpp]　（2018/11/24）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "scene2D.h"
#include "sceneModel.h"
#include "sceneSkinMesh.h"
#include "targetCapsule.h"

void CTargetCapsule::Init(D3DXVECTOR3 pos)
{
	m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_CAPSULE]);
	m_Pos = pos;
	m_Model->Move(m_Pos);
}

void CTargetCapsule::Uninit()
{
	if (m_Model)
	{
		m_Model->Release();
		m_Model = NULL;
	}
}

void CTargetCapsule::Update()
{

}

CTargetCapsule* CTargetCapsule::Create(D3DXVECTOR3 pos)
{
	CTargetCapsule* capsule = new CTargetCapsule();
	capsule->Init(pos);

	return capsule;
}
