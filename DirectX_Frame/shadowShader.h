//======================================================================
//	シャドウボリュームシェーダー[shadowShader.h]
//
//======================================================================
#ifndef _SHADOW_SHADER_H_
#define _SHADOW_SHADER_H_

#include "shader.h"

class CShaderShadow : public CShader
{
public:
	static CShaderShadow* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderShadow();
	CShaderShadow(const CShaderShadow& ref) {};
	CShaderShadow& operator = (const CShaderShadow& ref) {};
	~CShaderShadow() {}

	static CShaderShadow* m_Shader;
};

#endif // !_SHADOW_SHADER_H_
