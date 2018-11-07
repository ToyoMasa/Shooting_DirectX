//======================================================================
//	アニメ付きモデル[sceneSkinMesh.cpp]　（2018/8/21）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneSkinMesh.h"

SkinMeshFile *CSceneSkinMesh::m_SkinMeshFiles[SM_ID_MAX] = { NULL };
SkinMeshFileAnimation *CSceneSkinMesh::m_Animations[SM_ID_MAX] = { NULL };

void CSceneSkinMesh::Init(const SKINMESH_MODEL_ID& id)
{
	//m_SkinMeshFile = new SkinMeshFile();
	//m_Animation = new SkinMeshFileAnimation();
	//m_SkinMeshFile->Load(SKINMESH_SOURCE[id], m_Animation);
	if (m_SkinMeshFiles[id] == NULL)
	{
		LoadFile(id);
	}

	m_Animation = new SkinMeshFileAnimation();
	m_Animations[id]->GetAnimController()->CloneAnimationController(
		m_Animations[id]->GetAnimController()->GetMaxNumAnimationOutputs(),
		m_Animations[id]->GetAnimController()->GetMaxNumAnimationSets(),
		m_Animations[id]->GetAnimController()->GetMaxNumTracks(),
		m_Animations[id]->GetAnimController()->GetMaxNumEvents(),
		&m_Animation->GetAnimController()
	);
	m_Animation->Init();

	m_ModelID = id;
}

void CSceneSkinMesh::Uninit()
{
	if (m_Animation != NULL)
	{
		delete m_Animation;
	}
	//if (m_SkinMeshFile != NULL)
	//{
	//	delete m_SkinMeshFile;
	//}
}

void CSceneSkinMesh::Update()
{

}

void CSceneSkinMesh::Draw()
{
	if (m_SkinMeshFiles[m_ModelID] != NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		m_Animation->UpdateAnim(m_AnimPlaySpeed);
		m_SkinMeshFiles[m_ModelID]->UpdateFrame(m_SkinMeshFiles[m_ModelID]->GetRootFrame(), &m_World);

		pDevice->SetTransform(D3DTS_WORLD, &m_World);

		// FVF(今から使用する頂点情報)の設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		m_SkinMeshFiles[m_ModelID]->Draw(&m_World);
	}
}

void CSceneSkinMesh::SetWorld(D3DXMATRIX move)
{
	m_World = move;
}

void CSceneSkinMesh::Move(D3DXVECTOR3 pos)
{
	D3DXMatrixTranslation(&m_Move, pos.x, pos.y, pos.z);

	m_World = m_Scale * m_Rotate * m_Move;
}

void CSceneSkinMesh::Rotate(D3DXVECTOR3 rot)
{
	D3DXMatrixRotationX(&m_RotX, rot.x);
	D3DXMatrixRotationY(&m_RotY, rot.y);
	D3DXMatrixRotationZ(&m_RotZ, rot.z);

	m_Rotate = m_RotX * m_RotY * m_RotZ;
	m_World = m_Scale * m_Rotate * m_Move;
}

void CSceneSkinMesh::Rotate(D3DXMATRIX rot)
{
	m_Rotate = rot;
	m_World = m_Scale * m_Rotate * m_Move;
}

void CSceneSkinMesh::Scale(D3DXVECTOR3 scale)
{
	D3DXMatrixScaling(&m_Scale, scale.x, scale.y, scale.z);

	m_World = m_Scale * m_Rotate * m_Move;
}

CSceneSkinMesh* CSceneSkinMesh::Create(const SKINMESH_MODEL_ID& id)
{
	CSceneSkinMesh* skinMesh = new CSceneSkinMesh(LAYER_OBJECT3D);
	skinMesh->Init(id);

	return skinMesh;
}

void CSceneSkinMesh::ChangeAnim(UINT animID, float shiftTime)
{
	m_Animation->ChangeAnim(animID, shiftTime);
	m_AnimPlaySpeed = m_DefAnimSpeed;
}

bool CSceneSkinMesh::SetLoopTime(FLOAT time)
{
	return m_Animation->SetLoopTime(m_Animation->GetCurrentAnim(), time);
}

void CSceneSkinMesh::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID)
{
	m_Animation->PlayMontage(animID, shiftTime, playTime, nextAnimID);
}

void CSceneSkinMesh::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID, float playSpeed)
{
	m_Animation->PlayMontage(animID, shiftTime, playTime, nextAnimID);
	SetAnimPlaySpeed(playSpeed);
}


D3DXMATRIX CSceneSkinMesh::GetBoneMatrix(LPSTR _BoneName)
{
	m_SkinMeshFiles[m_ModelID]->UpdateFrame(m_SkinMeshFiles[m_ModelID]->GetRootFrame(), &m_World);
	return m_SkinMeshFiles[m_ModelID]->GetBoneMatrix(_BoneName);
}
void CSceneSkinMesh::LoadFile(const SKINMESH_MODEL_ID& id)
{
	if (m_SkinMeshFiles[id] == NULL)
	{
		m_SkinMeshFiles[id] = new SkinMeshFile();
		m_Animations[id] = new SkinMeshFileAnimation();
		m_SkinMeshFiles[id]->Load(SKINMESH_SOURCE[id], m_Animations[id]);
	}
}

void CSceneSkinMesh::ReleaseFile(const SKINMESH_MODEL_ID& id)
{
	if (m_SkinMeshFiles[id] != NULL)
	{
		delete m_SkinMeshFiles[id];
		delete m_Animations[id];
	}
}

void CSceneSkinMesh::ReleaseFileAll()
{
	for (int i = 0; i < SM_ID_MAX; i++)
	{
		ReleaseFile((SKINMESH_MODEL_ID)i);
	}
}
