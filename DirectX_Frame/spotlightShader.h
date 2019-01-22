//======================================================================
//	シェーダー[shader.h]
//
//======================================================================
#ifndef _SPOTLIGHT_SHADER_H_
#define _SPOTLIGHT_SHADER_H_

#include "shader.h"

static const D3DXVECTOR4 FALLOFF_PARAM(30.0f, 0.7f, 0.4f, 0.15f);
static const D3DXVECTOR4 SPOTLIGHT_PARAM(0.01f, cosf(D3DXToRadian(120.0f) / 2.0f), 1.0f / (cosf(D3DXToRadian(70.0f) / 2.0f) - cosf(D3DXToRadian(10.0f) / 2.0f)), 1.0f);

class CShaderSpotlight : public CShader
{
public:
	static CShaderSpotlight* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderSpotlight();
	CShaderSpotlight(const CShaderSpotlight& ref) {};
	CShaderSpotlight& operator = (const CShaderSpotlight& ref) {};
	~CShaderSpotlight() {}

	static CShaderSpotlight* m_Shader;
};

#endif // !_SPOTLIGHT_SHADER_H_
