#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

class CCamera;
class CScene;

enum SHADERFILE_LIST
{
	SHADER_FILE_001,
	SHADER_FILE_PHONG,
	SHADER_FILE_BASIC,
	SHADER_FILE_MAX,
};

// シェーダーファイルリスト
static const std::string SHADER_FILE[] =
{
	"data/shaders/shader.fx",
	"data/shaders/phong.fx",
	"data/shaders/basic.fx",
};

static const DWORD FVF_SHADER = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

class CShader
{
public:
	CShader()
	{
		m_hTech = NULL;
		D3DXMatrixIdentity(&m_World);
	}
	~CShader(){}

	void Init(int id);
	void Uninit();
	void Draw(CScene* scene);
	void Draw(CScene* scene, LPD3DXMESH mesh, DWORD id);
	void SetWorld(D3DXMATRIX world);
	void SetTexture(LPDIRECT3DTEXTURE9 texture);
	static void LoadShader();
	static void ReleaseShader();
	static void SetCamera(CCamera* camera) { m_pCamera = camera; }
	static CShader* GetShader(int id) { return m_Shaders[id]; }

private:
	D3DXMATRIX m_World;	// 描画に使う変換行列

	static CCamera* m_pCamera;

	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_hTech;			// 現在のテクニックのハンドル
	D3DXHANDLE m_hTechNext;		// 次のテクニックのハンドル
	static CShader			*m_Shaders[SHADER_FILE_MAX];
};

#endif // !_SHADER_H_
