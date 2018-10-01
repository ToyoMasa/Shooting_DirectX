//======================================================================
//	3D�|���S���w�b�_[scene3D]
//
//======================================================================
#ifndef _SCENESKINMESH_H_
#define _SCENESKINMESH_H_

#include <string>
#include "SkinMeshFile.h"

class CSceneSkinMesh : public CScene
{
public:
	CSceneSkinMesh::CSceneSkinMesh(int priority) : CScene(priority)
	{
		D3DXMatrixIdentity(&m_World);
		D3DXMatrixIdentity(&m_Move);
		D3DXMatrixIdentity(&m_Rotate);
		D3DXMatrixIdentity(&m_RotX);
		D3DXMatrixIdentity(&m_RotY);
		D3DXMatrixIdentity(&m_RotZ);
		D3DXMatrixIdentity(&m_Scale);

		m_SkinMeshFile = NULL;
		m_DefAnimSpeed = 0.025f;
		m_AnimPlaySpeed = m_DefAnimSpeed;
	}

	void Init(const std::string& modelName);
	void Uninit();
	void Update();
	void Draw();
	void SetWorld(D3DXMATRIX move);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);

	D3DXMATRIX GetMtxRotate() { return m_Rotate; }

	// �{�[���̃}�g���b�N�X�擾�i�{�[���̖��O�j
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);

	// �A�j���[�V�����֘A
	SkinMeshFile* GetSkinMeshFile() { return m_SkinMeshFile; }
	void ChangeAnim(UINT animID, float shiftTime);
	bool SetLoopTime(FLOAT time);
	// �A�j���[�V�����̍Đ����x���グ��
	void SetAnimPlaySpeed(float speed) { m_AnimPlaySpeed = m_DefAnimSpeed * speed; }
	// 1��Đ��̃A�j���\�V����
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID);
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID, float playSpeed);
	bool GetPlayMontage() { return m_SkinMeshFile->GetPlayMontage(); }
	// �A�j���[�V�����̍Đ����Ԃ��擾
	float GetWeightTime() { return m_SkinMeshFile->GetWeightTime(); }

	static CSceneSkinMesh* Create(const std::string& modelName);

private:
	D3DXMATRIX			m_World;		// ���[���h�ϊ��s��
	D3DXMATRIX			m_Move;			// ���s�ړ��s��
	D3DXMATRIX			m_Rotate;		// ��]�s��
	D3DXMATRIX			m_RotX;			// X����]�s��
	D3DXMATRIX			m_RotY;			// Y����]�s��
	D3DXMATRIX			m_RotZ;			// Z����]�s��
	D3DXMATRIX			m_Scale;		// �g��k���s��
	SkinMeshFile*		m_SkinMeshFile;
	float				m_AnimPlaySpeed;
	float				m_DefAnimSpeed;
};

#endif
