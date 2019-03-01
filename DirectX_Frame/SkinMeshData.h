//======================================================================
//	�X�L�����b�V���f�[�^�w�b�_[SkinMeshData.h]
//
//======================================================================
#ifndef SKINMESH_DATA_H_
#define SKINMESH_DATA_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <vector>

struct FrameData : public D3DXFRAME
{
	// �����s��
	D3DXMATRIXA16 m_CombinedTransformationMatrix;

	// �I�t�Z�b�g�s��
	D3DXMATRIX	  m_OffsetMat;
	// �s��e�[�u���̃C���f�b�N�X�ԍ�(�C���f�b�N�X�t�p)
	DWORD		  m_OffsetID;
};

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

	// �p���b�g�T�C�Y
	DWORD m_NumPaletteEntries;

	MeshContainer()
	{
		m_BoneMatrix = nullptr;
		m_BoneOffsetMatrix = nullptr;
	}
};

class SkinMeshData : public ID3DXAllocateHierarchy
{
public:
	SkinMeshData() {}

	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);

	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR,
		CONST D3DXMESHDATA *,
		CONST D3DXMATERIAL *,
		CONST D3DXEFFECTINSTANCE *,
		DWORD, CONST DWORD *,
		LPD3DXSKININFO,
		LPD3DXMESHCONTAINER *);

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);

	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};
#endif
