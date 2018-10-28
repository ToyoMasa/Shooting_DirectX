// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------
float4x4 World;		// �ϊ��s��
float4x4 View;		// �ϊ��s��
float4x4 Proj;		// �ϊ��s��

float4x4 mWVP;

float4 vLightDir; // ���C�g�̕���
float4 vColor;  // ���C�g�����b�V���̐F
float3 vEyePos;  // �J�����̈ʒu�i���[�J�����W�n�j

// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos   : POSITION;
    float4 Color  : COLOR0;
    float3 N   : TEXCOORD0;
    float3 Eye   : TEXCOORD1;
float2 Tex : TEXCOORD2;
};
//�e�N�X�`��
texture Tex;
sampler Samp = sampler_state
{
 Texture =<Tex>;
 MinFilter =LINEAR;
 MagFilter =LINEAR;
 MipFilter =NONE;

 AddressU =Clamp;
 AddressV =Clamp;
};
// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VertexShader_Main(
      float4 Pos    : POSITION,          // ���[�J���ʒu���W
      float4 Normal : NORMAL,            // �@���x�N�g��
float2 Tex : TEXCOORD
){
 VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^
 
 // ���W�ϊ�
 //out_pos = mul(in_pos, mul(mul(World, View), Proj));	// ���_�ƕϊ��s����|����
 Out.Pos = mul(Pos, mul(mul(World, View), Proj));	// ���_�ƕϊ��s����|����
 
 // �g�U���{����
 float amb = -vLightDir.w; // �����̋���
 float3 L = -vLightDir; // ���[�J�����W�n�ł̃��C�g�x�N�g��
 Out.Color = vColor * max(amb, dot(Normal, -vLightDir));
 
 // ���ʔ��˗p�̃x�N�g��
 Out.N   = Normal.xyz;
 Out.Eye = vEyePos - Pos.xyz;
Out.Tex = Tex;//�e�N�X�`�����W
 
 return Out;
}
// -------------------------------------------------------------
float4 PixelShader_Main(VS_OUTPUT In) : COLOR
{  
 float3 L = -vLightDir.xyz;
 float3 H = normalize(L + normalize(In.Eye)); // �n�[�t�x�N�g��
 float3 N = normalize(In.N);
 
   // return (In.Color+ pow(max(0,dot(N, H)), 10))*tex2D( Samp, In. Tex);// �g�U���{����
 return In.Color*1.5*tex2D( Samp, In. Tex)+ pow(max(0,dot(N, H)), 40); 
    //+ pow(max(0,dot(N, H)), 10); // ���ʔ��ˌ�
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