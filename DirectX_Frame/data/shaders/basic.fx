//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
float4x4 World;		// �ϊ��s��
float4x4 View;		// �ϊ��s��
float4x4 Proj;		// �ϊ��s��
sampler		Sampler1 =
	sampler_state{
		MinFilter = LINEAR;
		MagFilter = LINEAR;
	};

//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void VertexShader_Main(		float3 in_pos : POSITION,
			float4 in_color : COLOR0,
			float2 in_tex : TEXCOORD0,
 				out float4 out_pos : POSITION,
				out float4 out_color : COLOR0,
				out float2 out_tex : TEXCOORD0)
{
	// ���W�ϊ�
	out_pos = mul(in_pos, mul(mul(World, View), Proj));	// ���_�ƕϊ��s����|����
	out_color = in_color;					// �F�͂��̂܂�
	out_tex = in_tex;					// �e�N�X�`�����W�̏o��
}

//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PixelShader_Main(		float4 in_color : COLOR0,
			float2 in_tex : TEXCOORD0,	// �s�N�Z����UV���W
			out float4 out_color : COLOR0)
{
    // �Œ�̐F�ŏo��
//	out_color = in_color;			// R G B A	

	// �T���v���[����e�N�Z�����擾
	float4 texcol = tex2D(Sampler1, in_tex);	

	//out_color = texcol;	// �e�N�Z���F���o��

	// ���_�J���[�ƃu�����h
	//out_color = texcol * in_color;

	// ���_�J���[�Ɖ��Z
	//out_color = texcol + in_color;

	// �e�N�Z���F�𔽓]
	out_color = 1.0f - texcol;
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TVPShader
{
	pass pass0
	{
		// �X�e�[�g�ݒ�
		// �V�F�[�_�ݒ�
		VertexShader = compile vs_2_0 VertexShader_Main();	// ���_�V�F�[�_�̐ݒ�
		PixelShader = compile ps_2_0 PixelShader_Main();	// �s�N�Z���V�F�[�_�̐ݒ�
	}
}