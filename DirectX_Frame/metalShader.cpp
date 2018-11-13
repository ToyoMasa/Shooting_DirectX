#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "shader.h"
#include "metalShader.h"

void CShaderMetal::ShaderSet(D3DXMATRIX world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 光の設定情報
	D3DXVECTOR4		light_dir(0.0f, -1.0f, 0.0f, 0.0f);			// 光の方向
	D3DXVECTOR4		diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// 平行光源の色
	D3DXVECTOR4		ambient(0.2f, 0.2f, 0.2f, 0.2f);			// 環境光
	D3DXVECTOR4		specular(0.2f, 0.2f, 0.2f, 0.2f);			// スペキュラ光
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos().x, CManager::GetCamera()->GetPos().y, CManager::GetCamera()->GetPos().z, 0.0f);

	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// 定数をセット(頂点シェーダー)
	m_VSConstantTable->SetMatrix(pDevice, "m_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "m_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "m_projection", &CManager::GetCamera()->GetProjection());	

	m_PSConstantTable->SetMatrix(pDevice, "m_world", &world);
	m_PSConstantTable->SetMatrix(pDevice, "m_view", &CManager::GetCamera()->GetView());
	m_PSConstantTable->SetMatrix(pDevice, "m_projection", &CManager::GetCamera()->GetProjection());

	m_VSConstantTable->SetVector(pDevice, "m_diffuse", &diffuse);
	m_VSConstantTable->SetVector(pDevice, "m_ambient", &ambient);
	m_VSConstantTable->SetVector(pDevice, "m_specular", &specular);
	m_VSConstantTable->SetVector(pDevice, "m_light_dir", &light_dir);

	m_PSConstantTable->SetVector(pDevice, "m_diffuse", &diffuse);
	m_PSConstantTable->SetVector(pDevice, "m_ambient", &ambient);
	m_PSConstantTable->SetVector(pDevice, "m_specular", &specular);
	m_PSConstantTable->SetVector(pDevice, "m_light_dir", &light_dir);

	m_PSConstantTable->SetVector(pDevice, "g_camerapos", &cameraPos);
}

void CShaderMetal::SetMaterial(D3DMATERIAL9 const&mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	// 環境光用のマテリアルをセット
	tempcolor.x = mat.Ambient.r;
	tempcolor.y = mat.Ambient.g;
	tempcolor.z = mat.Ambient.b;
	tempcolor.w = mat.Ambient.a;
	m_PSConstantTable->SetVector(pDevice, "g_ambient_mat", &tempcolor);
	m_VSConstantTable->SetVector(pDevice, "g_ambient_mat", &tempcolor);

	// ディフューズ光用のマテリアルをセット
	tempcolor.x = mat.Diffuse.r;
	tempcolor.y = mat.Diffuse.g;
	tempcolor.z = mat.Diffuse.b;
	tempcolor.w = mat.Diffuse.a;
	m_PSConstantTable->SetVector(pDevice, "g_diffuse_mat", &tempcolor);
	m_VSConstantTable->SetVector(pDevice, "g_diffuse_mat", &tempcolor);

	// エミッシブ光用のマテリアルをセット
	tempcolor.x = mat.Emissive.r;
	tempcolor.y = mat.Emissive.g;
	tempcolor.z = mat.Emissive.b;
	tempcolor.w = mat.Emissive.a;
	m_PSConstantTable->SetVector(pDevice, "g_emissive_mat", &tempcolor);
	m_VSConstantTable->SetVector(pDevice, "g_emissive_mat", &tempcolor);

	// スペキュラー光用のマテリアルをセット
	tempcolor.x = mat.Specular.r;
	tempcolor.y = mat.Specular.g;
	tempcolor.z = mat.Specular.b;
	tempcolor.w = mat.Specular.a;
	m_PSConstantTable->SetVector(pDevice, "g_specular_mat", &tempcolor);
	m_VSConstantTable->SetVector(pDevice, "g_specular_mat", &tempcolor);

	// パワー値をセット
	m_PSConstantTable->SetFloat(pDevice, "g_power", mat.Power);
	m_VSConstantTable->SetFloat(pDevice, "g_power", mat.Power);
}
