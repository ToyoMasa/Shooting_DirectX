#include "common.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "sceneModel.h"
#include "box.h"

CCamera		*CShader::m_pCamera = NULL;
CShader		*CShader::m_Shaders[SHADER_FILE_MAX] = { NULL };

void CShader::Init(int id)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	m_pCamera = CManager::GetCamera();

	D3DXCreateEffectFromFile(
		pDevice,
		SHADER_FILE[id].c_str(),
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&m_pEffect,
		NULL
	);

	HRESULT hr;

	do {
		hr = m_pEffect->FindNextValidTechnique(m_hTech, &m_hTechNext);
		if (FAILED(hr))
		{
			return;	// エラー
		}

		if (m_hTechNext)
		{
			// テクニックのハンドル(m_hTechNext)を取得
			D3DXTECHNIQUE_DESC TechDesc;
			m_pEffect->GetTechniqueDesc(m_hTechNext, &TechDesc); // テクニックの名前を取得
																 // TechDesc.Nameにテクニック名が入ってる
		}
		m_hTech = m_hTechNext;
	} while (m_hTech != NULL);
}

void CShader::Uninit()
{
	if (m_pEffect)
	{
		m_pEffect->Release();
	}

}

void CShader::Draw(CScene* scene)
{
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, scene->GetPos().x, scene->GetPos().y, scene->GetPos().z);
	m_World = world;

	if (m_pCamera != NULL)
	{
		m_pEffect->SetMatrix("World", &m_World);
		m_pEffect->SetMatrix("View", &m_pCamera->GetView());
		m_pEffect->SetMatrix("Proj", &m_pCamera->GetProjection());

		D3DXHANDLE hTVPShader;		// テクニック「TVPShader」のハンドル
		hTVPShader = m_pEffect->GetTechniqueByName("TVPShader");
		if (hTVPShader == NULL)
		{
			return;	// テクニック「TVPShader」が見つからなかった
		}

		m_pEffect->SetTechnique(hTVPShader);	// ハンドルを使ってテクニック「TVPShader」を選択する

		UINT numPass;

		HRESULT hr;

		hr = m_pEffect->Begin(&numPass, 0);

		if (FAILED(hr))
		{
			MessageBox(NULL, "テクニックの開始に失敗しました", "エラー", MB_OK);
			return;	// テクニックの開始に失敗
		}

		for (UINT iPass = 0; iPass < numPass; iPass++)
		{
			hr = m_pEffect->BeginPass(iPass);	// iPass番目のパスの実行を開始
			if (FAILED(hr))
			{
				MessageBox(NULL, "パスの実行に失敗しました", "エラー", MB_OK);
				break;	// パスの実行に失敗
			}

			// ↓BeginPassの後でID3DXEffect::Set～メソッドを使った場合必要になる
			// m_pEffect->CommitChanges(); // (このコード例ではない)

			// ここで実際にメッシュを描画する
			scene->Draw();

			m_pEffect->EndPass();
		}

		hr = m_pEffect->End();
		if (FAILED(hr))
		{

		}
	}
}

void CShader::Draw(CScene* scene, LPD3DXMESH mesh, DWORD id)
{
	D3DXMATRIX world;
	CSceneModel* temp = (CSceneModel*)scene;

	if (m_pCamera != NULL)
	{
		m_pEffect->SetMatrix("View", &m_pCamera->GetView());
		m_pEffect->SetMatrix("Proj", &m_pCamera->GetProjection());

		D3DXHANDLE hTVPShader;		// テクニック「TVPShader」のハンドル
		hTVPShader = m_pEffect->GetTechniqueByName("TVPShader");
		if (hTVPShader == NULL)
		{
			return;	// テクニック「TVPShader」が見つからなかった
		}

		m_pEffect->SetTechnique(hTVPShader);	// ハンドルを使ってテクニック「TVPShader」を選択する

		UINT numPass;

		HRESULT hr;

		hr = m_pEffect->Begin(&numPass, 0);

		if (FAILED(hr))
		{
			MessageBox(NULL, "テクニックの開始に失敗しました", "エラー", MB_OK);
			return;	// テクニックの開始に失敗
		}

		for (UINT iPass = 0; iPass < numPass; iPass++)
		{
			hr = m_pEffect->BeginPass(iPass);	// iPass番目のパスの実行を開始
			if (FAILED(hr))
			{
				MessageBox(NULL, "パスの実行に失敗しました", "エラー", MB_OK);
				break;	// パスの実行に失敗
			}

			// ↓BeginPassの後でID3DXEffect::Set～メソッドを使った場合必要になる
			// m_pEffect->CommitChanges(); // (このコード例ではない)

			// ここで実際にメッシュを描画する
			mesh->DrawSubset(id);

			m_pEffect->EndPass();
		}

		hr = m_pEffect->End();
		if (FAILED(hr))
		{
			MessageBox(NULL, "シェーダーの適用に失敗しました", "エラー", MB_OK);
		}
	}
}

void CShader::SetWorld(D3DXMATRIX world)
{
	m_pEffect->SetMatrix("World", &world);
}

void CShader::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pEffect->SetTexture(0, texture);
}

void CShader::LoadShader()
{
	// シェーダーファイル読み込み
	for (int i = 0; i < SHADER_FILE_MAX; i++)
	{
		if (m_Shaders[i] == NULL)
		{
			m_Shaders[i] = new CShader();
			m_Shaders[i]->Init(i);
		}
	}
}

void CShader::ReleaseShader()
{
	// シェーダーファイルの解放
	for (int i = 0; i < SHADER_FILE_MAX; i++)
	{
		if (m_Shaders[i] != NULL)
		{
			m_Shaders[i]->Uninit();
			delete m_Shaders[i];
			m_Shaders[i] = NULL;
		}
	}
}
