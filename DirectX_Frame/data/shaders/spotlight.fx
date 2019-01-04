//// �O���[�o���ϐ�
////float4x4 	g_matWorld[4] : WORLD;  // ���[���h�ϊ��s��z��
//float4x4	g_world;		// �ϊ��s��
//float4x4	g_view;			// �ϊ��s��
//float4x4	g_projection;		// �ϊ��s��
////int 		g_blendNum;       	// �u�����h����z��̐�
//bool		g_tex;			// �e�N�X�`������Ȃ��t���O�@true:����
//
//float3		g_lightposition;
//float		g_lightstrength;
//float3		g_lightdirection;
//float		g_lightcos; // �X�|�b�g���C�g�̊p�x�̃R�T�C���l
//float		g_lightdecay; // ����
//
////------------------------------------------------
//// �T���v���[1
////------------------------------------------------
//sampler Sampler1 =
//sampler_state {
//	Texture = <g_texture>;
//	MinFilter = LINEAR;		// ���j�A�t�B���^�i�k�����j
//	MagFilter = LINEAR;		// ���j�A�t�B���^�i�g�厞�j
//};
//
//void main(float3 in_pos 	: POSITION,
//	float3 in_normal 		: NORMAL,
//	float2 in_tex1			: TEXCOORD0,
//	out float4 out_pos			: POSITION,
//	out float2 out_tex1			: TEXCOORD0,
//	out float3 out_normal 		: TEXCOORD1,
//	out float3 out_lightdir 	: TEXCOORD2)
//{
//
//	// ���W�ϊ����ďo�́i���[���h�ϊ���̒��_���o�́j
//	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
//	out_pos = mul(out_pos, g_view);						// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
//	out_pos = mul(out_pos, g_projection);				// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�
//
//	// �@�������[���h��ԏ�̃x�N�g���ɕϊ����āA�P�ʃx�N�g�������Ă���o��
//	out_normal = normalize(mul(in_normal, (float3x3)g_world));
//
//	out_lightdir = out_pos - g_lightposition;
//
//	// �e�N�X�`�����W�����̂܂܏o�͂���
//	out_tex1 = in_tex1;			// �e�N�X�`�����W�̏o��
//
//}
//
//void PS(float2 in_tex1		: TEXCOORD0,	//�e�N�X�`�����W����
//	float3 in_normal 		: TEXCOORD1,	//�s�N�Z���̖@������
//	float3 in_lightdir 		: TEXCOORD2,
//	out float4 out_color 	: COLOR0)			//�s�N�Z���J���[�o��
//{
//	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//	float ambient = 0.1f;
//
//	float3 ldir = normalize(in_lightdir);
//	float shade = 0;
//	float coneFactor = dot(ldir, normalize(g_lightdirection)); //< �X�|�b�g���C�g�͈͓̔����ǂ���
//	if(coneFactor >= g_lightcos) 
//	{
//		shade = dot(normalize(in_normal), - ldir) * pow(coneFactor, g_lightdecay) * g_lightstrength; // �����ƃ��C�g�̋������l��
// 	}
//
//	//�F�̏o��
//	if (g_tex)
//	{
//		float4 tex_color = tex2D(Sampler1, in_tex1);
//		out_color = tex_color * (shade + ambient);
//	}
//	else
//	{
//		out_color = color * (shade + ambient);
//	}
//}

float4x4	g_world;				// ���[���h�ϊ��s��
float4x4 	g_view;					// �J�����ϊ��s��
float4x4 	g_projection;				// �v���W�F�N�V�����ϊ��s��

// main�֐�
void main(
	float3 in_pos			: POSITION,
	float3 in_normal		: NORMAL,
	float4 in_diff			: COLOR0,
	float2 in_tex			: TEXCOORD0,
	out float4 out_pos		: POSITION,
	out float4 out_diff		: COLOR0,
	out float2 out_tex		: TEXCOORD0,
	out float3 out_normal	: TEXCOORD1,
	out float3 out_posforps : TEXCOORD2)
{
	float3 lWorldNrm;
	float3 lViewNrm;


	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���W�ϊ����ďo�́i���[���h�ϊ���̒��_���o�́j
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
	out_posforps = out_pos.xyz;							// ���[���h�ϊ��ςݍ��W
	out_pos = mul(out_pos, g_view);						// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	out_pos = mul(out_pos, g_projection);				// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�

	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �J�n )

	// �@�������[���h��ԏ�̃x�N�g���ɕϊ����āA�P�ʃx�N�g�������Ă���o��
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

	// �@�����r���[��Ԃ̊p�x�ɕϊ� =========================================( �I�� )

	out_diff = in_diff;
	out_tex = in_tex;
}

// ���C�g�p�����[�^
float4	g_light_pos;               // ���W( �r���[��� )
float3	g_light_dir;              // ����( �r���[��� )
float4	g_light_diff;                // �f�B�t���[�Y�J���[
float4	g_light_specular;               // �X�y�L�����J���[
float4	g_light_ambient;                // �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������
float4	g_falloff_param;  // x:�L������  y:�X�|�b�g���C�g�pFallOff  z:�����ɂ�錸�������p�p�����[�^�O  w:�����ɂ�錸�������p�p�����[�^�P
float4	g_light_param;      // x:�����ɂ�錸�������p�p�����[�^�Q  y:�X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )  z:�X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

float4  g_mat_ambient;
float4	g_mat_emissive;		// �G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[
float4	g_mat_diffuse;
float4	g_mat_specular;
float4	g_mat_power;
float	g_alpha;		// �s�����x��


//------------------------------------------------
// �T���v���[1
//------------------------------------------------
sampler Sampler1 =
sampler_state {
	Texture = <g_texture>;
	MinFilter = LINEAR;		// ���j�A�t�B���^�i�k�����j
	MagFilter = LINEAR;		// ���j�A�t�B���^�i�g�厞�j
};

// main�֐�
void PS(
	float2 in_tex		: TEXCOORD0,
	float4 in_diff : COLOR0,
	float3 in_normal : TEXCOORD1,    // �@��( �r���[��� )
	float3 in_posforps      : TEXCOORD2,    // ���W( �r���[��� ))
	out float4 out_color	: COLOR0)
{
	float4 TextureDiffuseColor;
	float4 SpecularColor;
	float3 Normal;
	float DiffuseAngleGen;
	float4 TotalDiffuse;
	float4 TotalSpecular;
	float3 V_to_Eye;
	float3 TempF3;
	float Temp;
	float3 lLightTemp;
	float lLightDistancePow2;
	float lLightGen;
	float3 lLightDir;
	float lLightDirectionCosA;


	// �@���̏���
	Normal = normalize(in_normal);

	// ���_���W���王�_�ւ̃x�N�g���𐳋K��
	V_to_Eye = normalize(g_light_pos.xyz - in_posforps);

	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
	TotalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	TotalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// �X�|�b�g���C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// ���C�g�����x�N�g���̌v�Z
	lLightDir = normalize(in_posforps - g_light_pos.xyz);


	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �J�n )

	// ���������v�Z ------------------

	// ���_�ƃ��C�g�ʒu�Ƃ̋����̓������߂�
	lLightTemp = in_posforps - g_light_pos.xyz;
	lLightDistancePow2 = length(lLightTemp) * length(lLightTemp);

	// �������̌v�Z lLightGen = 1 / ( �����l0 + �����l1 * ���� + �����l2 * ( ���� * ���� ) )
	lLightGen = 1.0f / (g_falloff_param.z + g_falloff_param.w * length(lLightTemp) + g_light_param.x * lLightDistancePow2);

	// --------------------------------


	// �X�|�b�g���C�g�����v�Z --------

	// ���C�g�����x�N�g���ƃ��C�g�ʒu���璸�_�ʒu�ւ̃x�N�g���̓���( ���� Cos a )���v�Z 
	lLightDirectionCosA = dot(lLightDir, g_light_dir);

	// �X�|�b�g���C�g�����v�Z  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
	lLightGen *= saturate(pow(abs(max(lLightDirectionCosA - g_light_param.y, 0.0f) * g_light_param.z), g_falloff_param.y));

	// --------------------------------


	// �L�������O�������猸�������ő�ɂ��鏈��
	lLightGen *= step(length(lLightTemp), g_falloff_param.x);

	// �����E�X�|�b�g���C�g�����l�v�Z =======================================( �I�� )


	// �f�B�t���[�Y�F�v�Z

	// �f�B�t���[�Y�p�x�������v�Z
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));

	// �f�B�t���[�Y�J���[�~�ϒl += ( ���C�g�̃f�B�t���[�Y�J���[ * �}�e���A���f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������  ) * �����E�X�|�b�g���C�g�̊p�x������
	TotalDiffuse += (g_light_diff * in_diff * DiffuseAngleGen + g_light_ambient) * lLightGen;
	//TotalDiffuse = (g_light_diff * in_diff * DiffuseAngleGen + g_light_ambient);

	// �X�y�L�����J���[�v�Z

	// �n�[�t�x�N�g���̌v�Z
	TempF3 = normalize(V_to_Eye - lLightDir);

	// Temp = pow( max( 0.0f, N * H ), g_mat_power.x )
	Temp = pow(max(0.0f, dot(Normal, TempF3)), g_mat_power.x);

	// �X�y�L�����J���[�~�ϒl += Temp * �����E�X�|�b�g���C�g�̊p�x������ * ���C�g�̃X�y�L�����J���[
	TotalSpecular += Temp * lLightGen.x * g_light_specular;

	// �X�|�b�g���C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )



	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	// TotalDiffuse = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
	//TotalDiffuse += g_mat_ambient * g_mat_emissive;

	// SpecularColor = ���C�g�̃X�y�L�����J���[�~�ϒl * �}�e���A���̃X�y�L�����J���[
	SpecularColor = TotalSpecular * g_mat_specular;

	// �o�̓J���[ = TotalDiffuse * �e�N�X�`���J���[ + SpecularColor
	TextureDiffuseColor = tex2D(Sampler1, in_tex);
	out_color.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + SpecularColor.rgb;

	// �A���t�@�l = �e�N�X�`���A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	out_color.a = TextureDiffuseColor.a * in_diff.a * g_alpha;

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

}