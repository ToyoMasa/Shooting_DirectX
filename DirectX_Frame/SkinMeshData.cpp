#include "common.h"
#include "main.h"
#include <map>
#include "SkinMeshData.h"

HRESULT SkinMeshData::CreateFrame(THIS_ LPCSTR name, LPD3DXFRAME *new_frame)
{
	// 自作フレームの作成
	FrameData *frame = new FrameData();
	ZeroMemory(frame, sizeof(FrameData));
	*new_frame = frame;

	// 名前のコピー
	if (name != NULL)
	{
		frame->Name = new char[lstrlenA(name) + 1];
		strcpy_s(frame->Name, lstrlenA(name) + 1, name);
	}
	else {
		frame->Name = NULL;
	}

	return S_OK;
}

HRESULT SkinMeshData::CreateMeshContainer(
	THIS_ LPCSTR name,
	CONST D3DXMESHDATA *mesh_data,
	CONST D3DXMATERIAL *material_data,
	CONST D3DXEFFECTINSTANCE *effect_instance,
	DWORD material_num,
	CONST DWORD *adjacency,
	LPD3DXSKININFO skin_info,
	LPD3DXMESHCONTAINER *new_mesh_container)
{
	// メッシュの取得
	LPD3DXMESH mesh = mesh_data->pMesh;
	// 自作メッシュコンテナの作成
	MeshContainer *container = new MeshContainer;
	// デバイス
	LPDIRECT3DDEVICE9 device = NULL;
	// ポリゴンの数
	int porigon_num = 0;

	if (container == NULL)
	{
		return E_FAIL;
	}
	ZeroMemory(container, sizeof(MeshContainer));

	// 名前のコピー
	container->Name = new char[lstrlenA(name) + 1];
	strcpy_s(container->Name, lstrlenA(name) + 1, name);

	// ポリゴン数の取得
	porigon_num = mesh->GetNumFaces();

	// デバイスの取得
	mesh->GetDevice(&device);

	// メッシュのコピー
	container->MeshData.pMesh = mesh;
	container->MeshData.Type = D3DXMESHTYPE_MESH;

	// 隣接ポリゴン番号のコピー
	container->pAdjacency = new DWORD[porigon_num * 3];
	memcpy(container->pAdjacency, adjacency, sizeof(DWORD) * porigon_num * 3);

	// マテリアルのコピー
	container->NumMaterials = material_num;
	if (material_num > 0)
	{
		container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
		container->m_TextureList = new LPDIRECT3DTEXTURE9[container->NumMaterials];
		memcpy(container->pMaterials, material_data, sizeof(D3DXMATERIAL) * material_num);

		// ファイル読み込み
		for (int i = 0; i < material_num; i++)
		{
			if (container->pMaterials[i].pTextureFilename != NULL)
			{
				LPDIRECT3DTEXTURE9 texture;

				// 追加
				char filename[256];
				ZeroMemory(filename, sizeof(filename));
				sprintf_s(filename, "data/textures/%s", container->pMaterials[i].pTextureFilename);

				if (SUCCEEDED(D3DXCreateTextureFromFileA(
					device,
					filename,
					&texture)))
				{
					container->m_TextureList[i] = texture;
				}
			}
			else {
				container->m_TextureList[i] = NULL;
			}
		}
	}
	else {
		// マテリアルの数が0だったら時の仮のマテリアル追加
		container->NumMaterials = 1;
		container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
		container->m_TextureList = NULL;
		container->pMaterials[0].pTextureFilename = NULL;
		memset(&container->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		container->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		container->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		container->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		container->pMaterials[0].MatD3D.Diffuse.a = 1.0f;
		container->pMaterials[0].MatD3D.Ambient = container->pMaterials[0].MatD3D.Diffuse;
	}

	// スキン情報があれば情報を取得する
	if (skin_info != NULL)
	{
		container->pSkinInfo = skin_info;
		skin_info->AddRef();
		// ボーンの数を取得
		DWORD bone_num = container->m_BoneNum = skin_info->GetNumBones();
		container->m_BoneOffsetMatrix = new D3DXMATRIX[bone_num];

		for (DWORD i = 0; i < bone_num; i++)
		{
			// オフセット行列をコピーする
			memcpy(&container->m_BoneOffsetMatrix[i], container->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DXMATRIX));
		}

		// ボーンの最適化
		if (container->pSkinInfo->ConvertToBlendedMesh(
			mesh,						// 入力メッシュ
			NULL,						// 未使用
			container->pAdjacency,		// メッシュの隣接データ(入力)
			NULL,						// メッシュの隣接データ(出力)
			NULL,						// ポリゴンの新規インデックスのバッファ
			NULL,						// 頂点の新規インデックスのバッファ
			&container->m_BoneWeightNum,// １つの頂点に影響を及ぼす重みの数
			&container->m_BoneNum,		// ボーンの数
			&container->m_BoneBuffer,	// ボーンデータが格納されたバッファ
			&container->MeshData.pMesh	// 変換後のメッシュ
		))
		{
			return E_FAIL;
		}
	}

	*new_mesh_container = container;

	return S_OK;
}

HRESULT SkinMeshData::DestroyFrame(THIS_ LPD3DXFRAME frame)
{
	if (frame != NULL)
	{
		if (frame->pMeshContainer != NULL)
		{
			DestroyMeshContainer(frame->pMeshContainer);
		}
		
		if (frame->Name != NULL)
		{
			delete[](frame->Name);
		}

		if (frame->pFrameSibling != NULL)
		{
			DestroyFrame(frame->pFrameSibling);
		}

		if (frame->pFrameFirstChild != NULL)
		{
			DestroyFrame(frame->pFrameFirstChild);
		}

		delete(frame);
	}
	return S_OK;
}

HRESULT SkinMeshData::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER base)
{
	MeshContainer *container = (MeshContainer*)base;

	if (container->Name != NULL)
	{
		delete[](container->Name);
	}

	if (container->pAdjacency != NULL)
	{
		delete[](container->pAdjacency);
	}

	if (container->pMaterials != NULL)
	{
		delete[](container->pMaterials);
	}

	if (container->NumMaterials > 0)
	{
		for (int i = 0; i < container->NumMaterials; i++)
		{
			if (container->m_TextureList[i] != NULL)
			{
				container->m_TextureList[i]->Release();
				container->m_TextureList[i] = NULL;
			}
		}
		delete[](container->m_TextureList);
	}

	if (container->pSkinInfo != NULL)
	{
		container->pSkinInfo->Release();
	}

	if (container->m_BoneBuffer != NULL)
	{
		container->m_BoneBuffer->Release();
	}

	if (container->m_BoneMatrix != NULL)
	{
		delete[](container->m_BoneMatrix);
	}

	if (container->m_BoneOffsetMatrix != NULL)
	{
		delete[](container->m_BoneOffsetMatrix);
	}

	if (container->MeshData.pMesh != NULL)
	{
		container->MeshData.pMesh->Release();
		container->MeshData.pMesh = NULL;
	}

	delete(container);

	return S_OK;
}
