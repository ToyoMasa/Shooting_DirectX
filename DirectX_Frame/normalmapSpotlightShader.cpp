#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "normalmapSpotlightShader.h"

CShaderNormalmapSpotlight *CShaderNormalmapSpotlight::m_Shader = NULL;

CShaderNormalmapSpotlight::CShaderNormalmapSpotlight() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		SHADER_FILE[SH_ID_NORMALMAP_SPOTLIGHT].c_str(),						// シェーダーファイル名
		"main",							// エントリー関数名
		"vs_3_0");						// バージョン

	if (!sts)
	{
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}

	// ピクセルシェーダーコンパイル
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_NORMALMAP_SPOTLIGHT].c_str(),						// シェーダーファイル名
		"PS",							// エントリー関数名
		"ps_3_0");						// バージョン

	if (!sts)
	{
		MessageBox(NULL, "読み込みエラー", "読み込みエラー", MB_OK);
	}
}

CShaderNormalmapSpotlight* CShaderNormalmapSpotlight::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderNormalmapSpotlight();
	}

	return m_Shader;
}

void CShaderNormalmapSpotlight::Destroy()
{
	if (m_Shader != NULL)
	{
		delete m_Shader;
		m_Shader = NULL;
	}
}

void CShaderNormalmapSpotlight::ShaderSet(const D3DXMATRIX& world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 光の設定情報
	D3DXVECTOR4		lightDir(CManager::GetCamera()->GetFront(), 0.0f);		// 光の方向
	D3DXVECTOR4		diffuse(DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR, DEFAULT_DEFUSE_COLOR);			// 平行光源の色
	D3DXVECTOR4		ambient(DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR, DEFAULT_AMBIENT_COLOR);			// 環境光
	D3DXVECTOR4		specular(1.0f, 1.0f, 1.0f, 0.01f);			// スペキュラ光
	D3DXVECTOR4		cameraPos = D3DXVECTOR4(CManager::GetCamera()->GetPos().x, CManager::GetCamera()->GetPos().y, CManager::GetCamera()->GetPos().z, 0.0f);
	float			lightStrength = 30.0f;

	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// 定数をセット(頂点シェーダー)
	m_VSConstantTable->SetMatrix(pDevice, "g_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "g_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "g_projection", &CManager::GetCamera()->GetProjection());

	m_PSConstantTable->SetVector(pDevice, "g_light_pos", &cameraPos);
	m_PSConstantTable->SetVector(pDevice, "g_light_dir", &lightDir);
	m_PSConstantTable->SetVector(pDevice, "g_light_diff", &diffuse);
	m_PSConstantTable->SetVector(pDevice, "g_light_specular", &specular);
	m_PSConstantTable->SetVector(pDevice, "g_light_ambient", &ambient);
	m_PSConstantTable->SetVector(pDevice, "g_falloff_param", &D3DXVECTOR4(60.0f, 0.01f, 0.1f, 0.2f));
	m_PSConstantTable->SetVector(pDevice, "g_light_param", &D3DXVECTOR4(0.01f, cosf(D3DXToRadian(70.0f) / 2.0f), 1.0f / (cosf(D3DXToRadian(30.0f) / 2.0f) - cosf(D3DXToRadian(45.0f) / 2.0f)), 1.0f));

	m_PSConstantTable->SetVector(pDevice, "g_light_pos", &cameraPos);
	m_PSConstantTable->SetVector(pDevice, "g_camera_pos", &cameraPos);
}

void CShaderNormalmapSpotlight::SetMaterial(const D3DMATERIAL9& mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	//環境光用のマテリアルをセット
	tempcolor.x = 0.2f;
	tempcolor.y = 0.2f;
	tempcolor.z = 0.2f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_ambient", &tempcolor);

	// ディフューズ光用のマテリアルをセット
	tempcolor.x = 0.6f;
	tempcolor.y = 0.6f;
	tempcolor.z = 0.6f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_diffuse", &tempcolor);

	// エミッシブ光用のマテリアルをセット
	tempcolor.x = 0.5f;
	tempcolor.y = 0.5f;
	tempcolor.z = 0.5f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_emissive", &tempcolor);

	// スペキュラー光用のマテリアルをセット
	tempcolor.x = 0.05f;
	tempcolor.y = 0.05f;
	tempcolor.z = 0.05f;
	tempcolor.w = 1.0f;
	m_PSConstantTable->SetVector(pDevice, "g_mat_specular", &tempcolor);

	// パワー値をセット
	m_PSConstantTable->SetFloat(pDevice, "g_mat_power", 200);
	m_PSConstantTable->SetFloat(pDevice, "g_alpha", 1.0f);
}
