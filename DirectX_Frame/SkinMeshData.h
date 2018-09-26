#ifndef SKINMESH_DATA_H_
#define SKINMESH_DATA_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <map>

// 自作フレームデータ
struct FrameData : public D3DXFRAME
{
	// 合成行列
	D3DXMATRIXA16 m_CombinedTransformationMatrix;
};

// 自作メッシュコンテナ
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
};

// 階層クラス(ID3DXAllocateHierarchyで使う仮想関数のみ定義)
class SkinMeshData : public ID3DXAllocateHierarchy
{
public:
	SkinMeshData() {}

	/*
	メッシュコンテナの作成
	戻り値：
	HRESULT
	S_OK => 成功
	E_FAIL => 失敗

	引数：
	THIS_ LPCSTR name：

	LPD3DXFRAME *new_frame：

	内容：
	D3DXLoadMeshHierarchyFromX関数が実行された際に
	各フレームの数だけ呼び出される関数
	引数にはフレームの名前とアウトプット用の値が渡されており、
	自作のフレームを作成してフレームデータをコピーし、
	new_frameに渡す
	*/
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);

	/*
	メッシュコンテナの作成
	戻り値：
	HRESULT
	S_OK => 成功
	E_FAIL => 失敗

	引数：
	THIS_ LPCSTR name：
	メッシュの名前

	CONST D3DXMESHDATA *mesh_data：
	メッシュデータ

	CONST D3DXMATERIAL *material_data：
	マテリアルデータ

	CONST D3DXEFFECTINSTANCE *effect_instance：
	エフェクトデータ

	DWORD material_num：
	マテリアルの数

	CONST DWORD *adjacency：
	隣接ポリゴンデータ

	LPD3DXSKININFO skin_info：
	スキンデータ

	LPD3DXMESHCONTAINER *new_mesh_container：
	アウトプット用メッシュコンテナのポインタ

	内容：
	D3DXLoadMeshHierarchyFromX関数が実行された際に
	各フレームのコンテナの数だけ呼び出される関数
	引数にはメッシュコンテナに格納されているデータが
	渡されており、そのデータを自作のメッシュコンテナに
	コピーしてnew_mesh_containerに渡す
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
	フレームの削除
	戻り値：
	HRESULT
	S_OK => 成功
	E_FAIL => 失敗

	引数：
	THIS_ LPD3DXFRAME frame：
	削除対象のフレームデータ

	内容：
	CreateFrameで動的に確保したメモリを解放する

	*/
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);

	/*
	メッシュコンテナの削除
	戻り値：
	HRESULT
	S_OK => 成功
	E_FAIL => 失敗

	引数：
	THIS_ LPD3DXMESHCONTAINER base：
	削除対象のメッシュコンテナ

	内容：
	CreateMeshContainerで動的に確保したメモリを解放する

	*/
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};
#endif
