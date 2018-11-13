//======================================================================
//	シェーダー[shader.h]
//
//======================================================================
#ifndef _METAL_SHADER_H_
#define _METAL_SHADER_H_

#include "shader.h"

class CShaderMetal : public CShader
{
public:
	CShaderMetal() : CShader() {}
	~CShaderMetal() {}

	void ShaderSet(D3DXMATRIX world)override;
	void SetMaterial(D3DMATERIAL9 const&mat)override;
private:

};

#endif // !_METAL_SHADER_H_
