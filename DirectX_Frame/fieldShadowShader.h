//======================================================================
//	フィールドシャドウボリュームシェーダー[fieldShadowShader.h]
//
//======================================================================
#ifndef _FIELD_SHADOW_SHADER_H_
#define _FIELD_SHADOW_SHADER_H_

#include "shader.h"

class CShaderFieldShadow : public CShader
{
public:
	static CShaderFieldShadow* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderFieldShadow();
	CShaderFieldShadow(const CShaderFieldShadow& ref) {};
	CShaderFieldShadow& operator = (const CShaderFieldShadow& ref) {};
	~CShaderFieldShadow() {}

	static CShaderFieldShadow* m_Shader;
};

#endif // !_FIELD_SHADOW_SHADER_H_
