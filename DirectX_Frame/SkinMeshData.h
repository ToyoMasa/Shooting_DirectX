//======================================================================
//	スキンメッシュデータヘッダ[SkinMeshData.h]
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
	// 合成行列
	D3DXMATRIXA16 m_CombinedTransformationMatrix;

	// オフセット行列
	D3DXMATRIX	  m_OffsetMat;
	// 行列テーブルのインデックス番号(インデックス付用)
	DWORD		  m_OffsetID;
};

struct MeshContainer : public D3DXMESHCONTAINER
{
	// マテリアルで使用するテクスチャ
	LPDIRECT3DTEXTURE9 *m_TextureList;

	// ウェイトの数
	DWORD m_BoneWeightNum;

	// ボーンの数
	DWORD m_BoneNum;

	// ボーン情報のバッファ
	LPD3DXBUFFER m_BoneBuffer;

	// 全ボーンのマトリックスのポインタの配列
	D3DXMATRIX **m_BoneMatrix;

	// ボーンのオフセット行列
	D3DXMATRIX *m_BoneOffsetMatrix;

	// パレットサイズ
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
