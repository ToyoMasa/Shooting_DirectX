//======================================================================
//	スキンメッシュ用シャドウボリュームシェーダー[shadowShader.h]
//
//======================================================================
#ifndef _SKINMESH_SHADOW_SHADER_H_
#define _SKINMESH_SHADOW_SHADER_H_

#include "shader.h"

class CShaderSkinmeshShadow : public CShader
{
public:
	static CShaderSkinmeshShadow* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderSkinmeshShadow();
	CShaderSkinmeshShadow(const CShaderSkinmeshShadow& ref) {};
	CShaderSkinmeshShadow& operator = (const CShaderSkinmeshShadow& ref) {};
	~CShaderSkinmeshShadow() {}

	static CShaderSkinmeshShadow* m_Shader;
};

#endif // !_SKINMESH_SHADOW_SHADER_H_
