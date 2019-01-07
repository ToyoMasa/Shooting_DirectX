//======================================================================
//	スポットライト対応スキンメッシュシェーダー[skinmeshSpotlightShader.h]
//
//======================================================================
#ifndef _SKINMESH_SPOTLIGHT_SHADER_H_
#define _SKINMESH_SPOTLIGHT_SHADER_H_

#include "shader.h"

class CShaderSkinmeshSpotlight : public CShader
{
public:
	static CShaderSkinmeshSpotlight* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderSkinmeshSpotlight();
	CShaderSkinmeshSpotlight(const CShaderSkinmeshSpotlight& ref) {};
	CShaderSkinmeshSpotlight& operator = (const CShaderSkinmeshSpotlight& ref) {};
	~CShaderSkinmeshSpotlight() {}

	static CShaderSkinmeshSpotlight* m_Shader;
};

#endif // !_SKINMESH_SPOTLIGHT_SHADER_H_
