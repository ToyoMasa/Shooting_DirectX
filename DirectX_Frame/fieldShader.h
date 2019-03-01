//======================================================================
//	フィールド用シェーダー[fieldShader.h]
//
//======================================================================
#ifndef _FIELD_SHADER_H_
#define _FIELD_SHADER_H_

#include "shader.h"

//*********************************************************
//	フィールド用シェーダー
//*********************************************************
class CShaderField : public CShader
{
public:
	static CShaderField* GetShader();
	static void Destroy();
	void ShaderSet(const D3DXMATRIX& world)override;
	void SetMaterial(const D3DMATERIAL9& mat)override;

private:
	CShaderField();
	CShaderField(const CShaderField& ref) {};
	CShaderField& operator = (const CShaderField& ref) {};
	~CShaderField() {}

	static CShaderField* m_Shader;
};

#endif // !_FIELD_SHADER_H_
