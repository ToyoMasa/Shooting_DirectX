//======================================================================
//	�X�L�����b�V���t�@�C���w�b�_[SkinMeshFile.h]
//
//======================================================================
#ifndef SKINMESH_FILE_H_
#define SKINMESH_FILE_H_

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "skinMeshData.h"

static const int MAX_ANIMATION = 20;

class SkinMeshFileAnimation
{
public:
	SkinMeshFileAnimation() : m_AnimController(NULL) {}
	~SkinMeshFileAnimation();

	LPD3DXANIMATIONCONTROLLER& GetAnimController() { return m_AnimController; }
	LPD3DXANIMATIONSET GetAnimSet(int id) { return m_AnimSet[id]; }

	void Init();
	void UpdateAnim(float time);

	// �ǉ�
	DWORD GetCurrentAnim() { return m_CurrentAnim; }
	// �A�j���[�V�����ؑ�
	void ChangeAnim(UINT animID, float shiftTime);
	// �A�j���[�V�����̑��x�ݒ�
	bool SetLoopTime(UINT animID, FLOAT time);
	// �A�j���ڍs���ԃZ�b�g
	void SetShiftTime(float time) { m_ShiftTime = time; }
	// 1��Đ��̃A�j���\�V����
	void PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID);
	void StopMontage() { m_bPlayMontage = false; }
	bool GetPlayMontage() { return m_bPlayMontage; }
	// �A�j���[�V�����̍Đ����Ԃ��擾
	float GetWeightTime() { return m_CurrentWeightTime; }

private:
	// �A�j���[�V�����Ǘ�
	LPD3DXANIMATIONCONTROLLER m_AnimController;

	// �A�j���[�V����
	LPD3DXANIMATIONSET m_AnimSet[MAX_ANIMATION];
	//���݂̃A�j���[�V����
	DWORD m_CurrentAnim;
	//���݂̃A�j���[�V�����f�[�^�g���b�N
	D3DXTRACK_DESC m_CurrentTrackDesc;
	float m_TrackSpeed;             // �g���b�N�X�s�[�h���ߒl
	float m_ShiftTime;              // �V�t�g����̂ɂ����鎞��
	float m_CurrentWeightTime;      // ���݂̃E�F�C�g����

	// 1�x����A�j���[�V����
	DWORD m_NextAnim;
	bool m_bPlayMontage;
	float m_MontageTime;

};

// �K�w�t��XFile
class SkinMeshFile
{
public:
	SkinMeshFile() :
		m_SkinMeshData(),
		m_RootFrame(NULL),
		m_AnimController(NULL)
	{}

	~SkinMeshFile();

	bool Load(std::string file_name, SkinMeshFileAnimation* anim);

	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME  );	

	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER  );

	void Draw(LPD3DXMATRIX matrix);
	void DrawWithShader(LPD3DXMATRIX matrix, CShader* shader);

	void DrawFrame(LPD3DXFRAME frame);
	void DrawFrameWithShader(LPD3DXFRAME frame, CShader* shader);

	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container);
	void DrawMeshContainerWithShader(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container, CShader* shader);

	void UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix);

private:
	// �Ώۂ̃{�[��������
	FrameData* SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame);
public:
	// �{�[���̃}�g���b�N�X�擾�i�{�[���̖��O�j
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);

	LPD3DXFRAME GetRootFrame() { return m_RootFrame; }

private:
	// �K�w�f�[�^
	SkinMeshData *m_SkinMeshData;

	// ���[�g�t���[��
	LPD3DXFRAME m_RootFrame;

	// �A�j���[�V�����Ǘ�
    LPD3DXANIMATIONCONTROLLER m_AnimController;

	// �ǉ�
	// �S�t���[���Q�Ɣz��
	std::vector<FrameData*> m_FrameArray;
	// ���b�V���R���e�i����̃t���[���Q�ƋL�^
	std::vector<FrameData*> m_IntoMeshFrameArray;

	// �S�Ẵt���[���|�C���^�i�[�����֐�
	void CreateFrameArray(LPD3DXFRAME _pFrame);

};

#endif