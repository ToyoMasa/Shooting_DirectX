//------------------------------------------------
// グローバル変数
//------------------------------------------------
// 座標変換用
float4x4	g_world;					// ワールド変換行列
float4x4 	g_view;						// カメラ変換行列
float4x4 	g_projection;				// プロジェクション変換行列
// テクスチャ
texture		g_texture;					// テクスチャ
bool		g_tex;						// テクスチャのありなし　false:なし　true:あり
// 光
float4		g_specular;					// スペキュラー光
float3		g_light_dir;				// 平行光源の方向
// マテリアル
float4		g_specular_mat;				// スペキュラー
float		g_power;					// スペキュラー光のパワー値

float4      g_camerapos;				// カメラ座標

//------------------------------------------------
// サンプラー1
//------------------------------------------------
sampler Sampler1 =
sampler_state {
	Texture = <g_texture>;
	MinFilter = LINEAR;		// リニアフィルタ（縮小時）
	MagFilter = LINEAR;		// リニアフィルタ（拡大時）
};

//------------------------------------------------
// サンプラー2
//------------------------------------------------
sampler NormalmapSampler1 =
sampler_state {
	Texture = <g_normaltexture>;
	MinFilter = LINEAR;		// リニアフィルタ（縮小時）
	MagFilter = LINEAR;		// リニアフィルタ（拡大時）
};

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void main(float3 in_pos : POSITION,
	float3 in_normal : NORMAL,
	float4 in_col1 : COLOR0,
	float2 in_tex1 : TEXCOORD0,
	float3 in_binormal : BINORMAL,
	float3 in_tangent : TANGENT,
	out float4 out_pos : POSITION,
	out float2 out_tex1 : TEXCOORD0,
	out float3 out_lightvector : TEXCOORD1,
	out float3 out_posforps : TEXCOORD2,
	out float3 out_cameraposforps : TEXCOORD3)
{
	float3	N;		// ワールド空間上の法線ベクトル
	float3  P;      // ワールド座標
	float3  V;      // 視線ベクトル
	float3	L;		// 光の差し込む方向
	float3  B;		// ワールド空間の従法線ベクトル
	float3  T;		// ワールド空間の接ベクトル

					// 座標変換
	out_pos = mul(float4(in_pos, 1.0f), g_world);
	P = (float3)out_pos;
	out_pos = mul(out_pos, g_view);
	out_pos = mul(out_pos, g_projection);

	// 従法線をワールド空間に変換
	B = mul(in_binormal, (float3x3)g_world);

	// 接ベクトルをワールド空間に変換
	T = mul(in_tangent, (float3x3)g_world);

	// 法線ベクトルをワールド空間に変換
	N = mul(in_normal, (float3x3)g_world);

	// 正規化
	B = normalize(B);
	T = normalize(T);
	N = normalize(N);

	// 平行光の差し込む方向	単位ベクトル化
	//out_lightvector = g_light_dir;
	L = normalize(-g_light_dir);

	// 平行光の差し込む方向　接ベクトル空間に変換するために
	// 接ベクトル空間行列
	//    TTT0
	//    BBB0
	//    NNN0
	//    0001
	//     の逆行列を求める
	//       (転置でＯＫ）
	//    TBN0
	//    TBN0
	//    TBN0
	//    0001
	out_lightvector.x = dot(L, T);
	out_lightvector.y = dot(L, B);
	out_lightvector.z = dot(L, N);

	// カメラ座標（接ベクトル空間）
	//out_cameraposforps = (float3)g_camerapos;
	out_cameraposforps.x = dot((float3)g_camerapos, T);
	out_cameraposforps.y = dot((float3)g_camerapos, B);
	out_cameraposforps.z = dot((float3)g_camerapos, N);

	// ワールド座標（接ベクトル空間）
	//out_posforps = P;
	out_posforps = dot(P, T);
	out_posforps = dot(P, B);
	out_posforps = dot(P, N);

	// テクスチャ座標をそのまま出力する
	out_tex1 = in_tex1;

}
//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(float4 in_specular		: COLOR1,
	float2 in_tex1				: TEXCOORD0,
	float3 in_lightvector		: TEXCOORD1,
	float3 in_pos				: TEXCOORD2,
	float3 in_camerapos			: TEXCOORD3,
	out float4 out_color : COLOR0)
{
	float4 tex_color = tex2D(Sampler1, in_tex1);

	// ノーマルマップから法線情報を取得
	float4 Normal = tex2D(NormalmapSampler1, in_tex1);
	Normal = Normal * 2.0f - 1.0f;	// -1から1の範囲に変換
	Normal = normalize(Normal);

	// 接ベクトル空間の光の入射ベクトルを正規化する
	float3 Light = normalize(in_lightvector);

	float4 diffuse;

	// 法線マップを使用してディフューズ計算
	diffuse = max(0, dot(Light, Normal.xyz));

	// スペキュラ光計算
	//float4 specular = float4(0.2, 0.2, 0.2, 1.0);

	// 視線ベクトル計算
	float3 V = in_camerapos - in_pos;
	V = normalize(V);

	// ハーフベクトル計算
	float3 H = normalize(Light + V);

	// ブリンフォンでスペキュラ光を計算
	float4 specular = pow(max(0, dot(H, Normal.xyz)), 200);
	//float4 specular = g_specular * g_specular_mat *
	//	pow(max(0, dot((float3)Normal, H)), g_power);

	//色の出力
	if (g_tex)
	{
		// テクスチャの色と頂点の色を掛け合わせる
		out_color = diffuse * tex_color + specular;
		out_color.a = 1.0f;
	}
	else 
	{
		out_color = diffuse + specular;
	}
}
