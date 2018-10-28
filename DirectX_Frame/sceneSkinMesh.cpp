//======================================================================
//	アニメ付きモデル[sceneSkinMesh.cpp]　（2018/8/21）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneSkinMesh.h"

static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL); 

void CSceneSkinMesh::Init(const std::string& modelName)
{
	m_SkinMeshFile = new SkinMeshFile();
	m_Animation = new SkinMeshFileAnimation();
	m_SkinMeshFile->Load(modelName, m_Animation);
}

void CSceneSkinMesh::Uninit()
{
	if (m_Animation != NULL)
	{
		delete m_Animation;
	}
	if (m_SkinMeshFile != NULL)
	{
		delete m_SkinMeshFile;
	}
}

void CSceneSkinMesh::Update()
{
	if (m_SkinMeshFile != NULL)
	{
		m_Animation->UpdateAnim(m_AnimPlaySpeed);

		m_SkinMeshFile->UpdateFrame(m_SkinMeshFile->GetRootFrame(), &m_World);
	}
}

void CSceneSkinMesh::Draw()
{
	if (m_SkinMeshFile != NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if (pDevice == NULL)
		{
			return;
		}

		pDevice->SetTransform(D3DTS_WORLD, &m_World);

		// FVF(今から使用する頂点情報)の設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		m_SkinMeshFile->Draw(&m_World);
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

CSceneSkinMesh* CSceneSkinMesh::Create(const std::string& modelName)
{
	CSceneSkinMesh* skinMesh = new CSceneSkinMesh(LAYER_OBJECT3D);
	skinMesh->Init(modelName);

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
	return m_SkinMeshFile->GetBoneMatrix(_BoneName);
}
