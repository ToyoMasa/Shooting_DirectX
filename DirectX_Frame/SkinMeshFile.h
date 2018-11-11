#ifndef SKINMESH_FILE_H_
#define SKINMESH_FILE_H_

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "SkinMeshData.h"

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

	/*
		XFile�ǂݍ���
			�߂�l�F
				�ǂݍ��݌��ʁF
					�����F
						true
					���s�F
						false
			�����F
				std::string file_name�F
					�t�@�C����
			���e�F
				XFile�̓ǂݍ��݂��s��
	*/
	bool Load(std::string file_name, SkinMeshFileAnimation* anim);


	/*
		�{�[���s��̗̈�m��(�S�{�[��)
			�߂�l�F
				����
					�����FS_OK
					���s�FE_FAIL

			�����F					
				LPD3DXFRAME  
					�Ώۃt���[��

			���e�F
				�Ώۃt���[���̃{�[���s��̃������̈�̊m�ۊ֐������s����
				�q���A�Z��ɑ΂��Ă��̊֐����ċA�Ƃ��Ďg�p���A
				���[�܂Ŋm�ۂ��s���悤�ɂ���
	*/
	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME  );	

	/*
		�{�[���s��̗̈�m��(�S�{�[��)
			�߂�l�F
				����
					�����FS_OK
					���s�FE_FAIL

			�����F					
				LPD3DXMESHCONTAINER  
					�m�ۑΏۂ̃��b�V���R���e�i

			���e�F
				�R���e�i�Ŏg�p���Ă���{�[���s�񕪂̃������̈���m�ۂ���
				
	*/
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER  );

	/*
		�`��
			�߂�l�F
				�Ȃ�

			�����F
				LPD3DXMATRIX matrix
					�`��Ŏg�p���郏�[���h���W�s��

			���e�F
				XFile��`�悷��
				���̊֐����Ń��[�g�t���[���������Ƃ���
				DrawFrame���g�p����
				���̍ۂɂ���XFile��`�悷��I�u�W�F�N�g��
				���[���h���W�s��������œn��
	*/
	void Draw(LPD3DXMATRIX matrix);

	/*
		�t���[���̕`��
			�߂�l�F
				�Ȃ�

			�����F
				LPD3DXFRAME frame�F
					�t���[���f�[�^

			���e�F
				�����Ŏw�肳�ꂽ�t���[����`�悷��
				���̊֐����Ńt���[�����ێ����Ă���
				�S�Ẵ��b�V���R���e�i��`��(DrawMeshContainer)����
	*/
	void DrawFrame(LPD3DXFRAME frame);

	/*
		���b�V���R���e�i�̕`��
			�߂�l�F
				�Ȃ�

			�����F
				LPD3DXFRAME frame�F
					�t���[���f�[�^

				LPD3DXMESHCONTAINER container�F
					���b�V���R���e�i
			���e�F
				�w�肳�ꂽ���b�V���R���e�i��`�悷��
				�`��̓��e�͒ʏ��XFile�̕`����@�Ɠ������@��
				�`��ł���
	*/
	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container);

	/*
		�t���[���̍X�V
			�߂�l�F
				�Ȃ�

			�����F
				LPD3DXFRAME base�F
					�X�V�t���[��

				LPD3DXMATRIX parent_matrix�F
					�e�̎p���s��

			���e�F
				�e�̎p���s������ɂ��Ď����̎p���s����X�V����
				�q�A�Z�킪����ꍇ�͂�����ċA�ŌĂяo��
	*/
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