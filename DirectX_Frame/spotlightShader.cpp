#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "shader.h"
#include "spotlightShader.h"

CShaderSpotlight *CShaderSpotlight::m_Shader = NULL;

CShaderSpotlight::CShaderSpotlight() : CShader()
{
	bool sts;
	sts = VertexShaderCompile(
		SHADER_FILE[SH_ID_SPOTLIGHT].c_str(),						// シェーダーファイル名
		"main",							// エントリー関数名
		"vs_3_0");						// バージョン

	if (!sts) 
	{
		MessageBox(NULL, "エラー", "エラー", MB_OK);
	}

	// ピクセルシェーダーコンパイル
	sts = PixelShaderCompile(
		SHADER_FILE[SH_ID_SPOTLIGHT].c_str(),						// シェーダーファイル名
		"PS",							// エントリー関数名
		"ps_3_0");						// バージョン

	if (!sts) 
	{
		MessageBox(NULL, "読み込みエラー", "読み込みエラー", MB_OK);
	}
}

CShaderSpotlight* CShaderSpotlight::GetShader()
{
	if (m_Shader == NULL)
	{
		m_Shader = new CShaderSpotlight();
	}

	return m_Shader;
}

void CShaderSpotlight::Destroy()
{
	if (m_Shader)
	{
		delete m_Shader;
	}
}

void CShaderSpotlight::ShaderSet(const D3DXMATRIX& world)
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
	m_PSConstantTable->SetVector(pDevice, "g_falloff_param", &D3DXVECTOR4(40.0f, 0.08f, 0.1f, 0.2f));
	m_PSConstantTable->SetVector(pDevice, "g_light_param", &D3DXVECTOR4(0.01f, cosf(D3DXToRadian(70.0f) / 2.0f), 1.0f / (cosf(D3DXToRadian(30.0f) / 2.0f) - cosf(D3DXToRadian(45.0f) / 2.0f)), 1.0f));
}

void CShaderSpotlight::SetMaterial(const D3DMATERIAL9& mat)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	D3DXVECTOR4  tempcolor;

	 //環境光用のマテリアルをセット
	tempcolor.x = 0.8f;
	tempcolor.y = 0.8f;
	tempcolor.z = 0.8f;
	tempcolor.w = 1.0f;
	tempcolor.x = mat.Ambient.r;
	tempcolor.y = mat.Ambient.g;
	tempcolor.z = mat.Ambient.b;
	tempcolor.w = mat.Ambient.a;
	m_PSConstantTable->SetVector(pDevice, "g_mat_ambient", &tempcolor);

	// ディフューズ光用のマテリアルをセット
	tempcolor.x = mat.Diffuse.r;
	tempcolor.y = mat.Diffuse.g;
	tempcolor.z = mat.Diffuse.b;
	tempcolor.w = mat.Diffuse.a;
	m_PSConstantTable->SetVector(pDevice, "g_mat_diffuse", &tempcolor);

	// エミッシブ光用のマテリアルをセット
	tempcolor.x = mat.Emissive.r;
	tempcolor.y = mat.Emissive.g;
	tempcolor.z = mat.Emissive.b;
	tempcolor.w = mat.Emissive.a;
	m_PSConstantTable->SetVector(pDevice, "g_mat_emissive", &tempcolor);

	// スペキュラー光用のマテリアルをセット
	tempcolor.x = mat.Specular.r;
	tempcolor.y = mat.Specular.g;
	tempcolor.z = mat.Specular.b;
	tempcolor.w = mat.Specular.a;
	m_PSConstantTable->SetVector(pDevice, "g_mat_specular", &tempcolor);

	// パワー値をセット
	m_PSConstantTable->SetFloat(pDevice, "g_mat_power", mat.Power);
	m_PSConstantTable->SetFloat(pDevice, "g_alpha", 1.0f);
}
