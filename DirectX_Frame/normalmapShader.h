//======================================================================
//	ノーマルマップ対応シェーダー[normalmapShader.h]
//
//======================================================================
#ifndef _NORMALMAP_SHADER_H_
#define _NORMALMAP_SHADER_H_

#include "shader.h"

class CShaderNormalmap : public CShader
{
public:
	static CShaderNormalmap* GetShader();
	static void Destroy();
	void ShaderSet(D3DXMATRIX world)override;
	void SetMaterial(D3DMATERIAL9 const&mat)override;

private:
	CShaderNormalmap();
	CShaderNormalmap(const CShaderNormalmap& ref) {};
	CShaderNormalmap& operator = (const CShaderNormalmap& ref) {};
	~CShaderNormalmap() {}

	static CShaderNormalmap* m_Shader;
};

#endif // !_SKINMESH_SHADER_H_