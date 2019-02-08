//======================================================================
//	3D�|���S���w�b�_[scene3D]
//
//======================================================================
#ifndef _SCENESKINMESH_H_
#define _SCENESKINMESH_H_

#include <string>
#include "SkinMeshFile.h"

// ���f���Ǘ��pID
enum SKINMESH_MODEL_ID
{
	SM_ID_PLAYER,
	SM_ID_ZOMBIE_A,
	SM_ID_ZOMBIE_B,
	SM_ID_MAX
};

static const std::string SKINMESH_SOURCE[] =
{
	"data/models/player_hand.x",
	"data/models/zombieA.x",
	"data/models/zombieB.x",
};

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

		m_ModelID = SM_ID_MAX;
		m_SkinMeshFile = NULL;
		m_DefAnimSpeed = 0.025f;
		m_AnimPlaySpeed = m_DefAnimSpeed;
		m_NotDrawCount = 1;
		m_isAlwaysDraw = false;
	}

	void Init(const SKINMESH_MODEL_ID& id);
	void Uninit();
	void Update();
	void Draw();
	void DrawShadow();
	void DrawWithShader()override;
	void SetWorld(D3DXMATRIX move);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);
	void SetIsAlwaysDraw(bool draw) { m_isAlwaysDraw = draw; }

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
	void StopMontage() { m_Animation->StopMontage(); }
	bool GetPlayMontage() { return m_Animation->GetPlayMontage(); }
	// �A�j���[�V�����̍Đ����Ԃ��擾
	float GetWeightTime() { return m_Animation->GetWeightTime(); }

	static CSceneSkinMesh* Create(const SKINMESH_MODEL_ID& id);
	static void LoadAll();
	static void LoadFile(const SKINMESH_MODEL_ID& id);
	static void ReleaseFile(const SKINMESH_MODEL_ID& id);
	static void ReleaseFileAll();

private:
	D3DXMATRIX				m_World;		// ���[���h�ϊ��s��
	D3DXMATRIX				m_Move;			// ���s�ړ��s��
	D3DXMATRIX				m_Rotate;		// ��]�s��
	D3DXMATRIX				m_RotX;			// X����]�s��
	D3DXMATRIX				m_RotY;			// Y����]�s��
	D3DXMATRIX				m_RotZ;			// Z����]�s��
	D3DXMATRIX				m_Scale;		// �g��k���s��
	SkinMeshFile*			m_SkinMeshFile;
	SkinMeshFileAnimation*	m_Animation;
	SKINMESH_MODEL_ID		m_ModelID;
	float					m_AnimPlaySpeed;
	float					m_DefAnimSpeed;
	int						m_NotDrawCount;
	bool					m_isAlwaysDraw;
	static SkinMeshFile*			m_SkinMeshFiles[SM_ID_MAX];
	static SkinMeshFileAnimation*	m_Animations[SM_ID_MAX];
};

#endif
