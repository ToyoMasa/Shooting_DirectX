#ifndef SKINMESH_DATA_H_
#define SKINMESH_DATA_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <map>

// ����t���[���f�[�^
struct FrameData : public D3DXFRAME
{
	// �����s��
	D3DXMATRIXA16 m_CombinedTransformationMatrix;
};

// ���상�b�V���R���e�i
struct MeshContainer : public D3DXMESHCONTAINER
{
	// �}�e���A���Ŏg�p����e�N�X�`��
	LPDIRECT3DTEXTURE9 *m_TextureList;

	// �E�F�C�g�̐�
	DWORD m_BoneWeightNum;

	// �{�[���̐�
	DWORD m_BoneNum;

	// �{�[�����̃o�b�t�@
	LPD3DXBUFFER m_BoneBuffer;

	// �S�{�[���̃}�g���b�N�X�̃|�C���^�̔z��
	D3DXMATRIX **m_BoneMatrix;

	// �{�[���̃I�t�Z�b�g�s��
	D3DXMATRIX *m_BoneOffsetMatrix;
};

// �K�w�N���X(ID3DXAllocateHierarchy�Ŏg�����z�֐��̂ݒ�`)
class SkinMeshData : public ID3DXAllocateHierarchy
{
public:
	SkinMeshData() {}

	/*
	���b�V���R���e�i�̍쐬
	�߂�l�F
	HRESULT
	S_OK => ����
	E_FAIL => ���s

	�����F
	THIS_ LPCSTR name�F

	LPD3DXFRAME *new_frame�F

	���e�F
	D3DXLoadMeshHierarchyFromX�֐������s���ꂽ�ۂ�
	�e�t���[���̐������Ăяo�����֐�
	�����ɂ̓t���[���̖��O�ƃA�E�g�v�b�g�p�̒l���n����Ă���A
	����̃t���[�����쐬���ăt���[���f�[�^���R�s�[���A
	new_frame�ɓn��
	*/
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);

	/*
	���b�V���R���e�i�̍쐬
	�߂�l�F
	HRESULT
	S_OK => ����
	E_FAIL => ���s

	�����F
	THIS_ LPCSTR name�F
	���b�V���̖��O

	CONST D3DXMESHDATA *mesh_data�F
	���b�V���f�[�^

	CONST D3DXMATERIAL *material_data�F
	�}�e���A���f�[�^

	CONST D3DXEFFECTINSTANCE *effect_instance�F
	�G�t�F�N�g�f�[�^

	DWORD material_num�F
	�}�e���A���̐�

	CONST DWORD *adjacency�F
	�אڃ|���S���f�[�^

	LPD3DXSKININFO skin_info�F
	�X�L���f�[�^

	LPD3DXMESHCONTAINER *new_mesh_container�F
	�A�E�g�v�b�g�p���b�V���R���e�i�̃|�C���^

	���e�F
	D3DXLoadMeshHierarchyFromX�֐������s���ꂽ�ۂ�
	�e�t���[���̃R���e�i�̐������Ăяo�����֐�
	�����ɂ̓��b�V���R���e�i�Ɋi�[����Ă���f�[�^��
	�n����Ă���A���̃f�[�^������̃��b�V���R���e�i��
	�R�s�[����new_mesh_container�ɓn��
	*/
	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR,
		CONST D3DXMESHDATA *,
		CONST D3DXMATERIAL *,
		CONST D3DXEFFECTINSTANCE *,
		DWORD, CONST DWORD *,
		LPD3DXSKININFO,
		LPD3DXMESHCONTAINER *);

	/*
	�t���[���̍폜
	�߂�l�F
	HRESULT
	S_OK => ����
	E_FAIL => ���s

	�����F
	THIS_ LPD3DXFRAME frame�F
	�폜�Ώۂ̃t���[���f�[�^

	���e�F
	CreateFrame�œ��I�Ɋm�ۂ������������������

	*/
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);

	/*
	���b�V���R���e�i�̍폜
	�߂�l�F
	HRESULT
	S_OK => ����
	E_FAIL => ���s

	�����F
	THIS_ LPD3DXMESHCONTAINER base�F
	�폜�Ώۂ̃��b�V���R���e�i

	���e�F
	CreateMeshContainer�œ��I�Ɋm�ۂ������������������

	*/
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};
#endif
