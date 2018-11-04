//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4 	g_world;		// ワールド行列
float4x4	g_view;			// カメラ変換行列
float4x4	g_projection;		// プロジェクション変換行列
bool		g_tex;			// テクスチャありなしフラグ　true:あり

// 光関連
float3		g_light_dir;	// 平行光源の方向
float4		g_diffuse;		// デフューズ
float4		g_ambient;		// アンビエント
float4		g_specular;		// スペキュラ―

// マテリアル
float4		g_diffuse_mat;		// デフューズ
float4		g_emmisive_mat;		// エミッシブ
float4		g_ambient_mat;		// アンビエント
float4		g_specular_mat;		// スペキュラー
float		g_power;		// スペキュラパワー

//------------------------------------------------
// サンプラー1
//------------------------------------------------
sampler 	Sampler1 =
	sampler_state{
		MinFilter = LINEAR;
		MagFilter = LINEAR;
	};

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void main(	float3 in_pos : POSITION,
			float3 in_normal : NORMAL,
			float2 in_tex1 : TEXCOORD0,
 			out float4 out_pos : POSITION,
			out float4 out_color : COLOR0,
			out float2 out_tex : TEXCOORD0)
{
	float3 N;	// ワールド変換済み法線
	float3 L;	// 光のベクトル

	// 座標変換
	//out_pos = mul(in_pos, mul(mul(g_world, g_view), g_projection));	// 頂点と変換行列を掛ける
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
	out_pos = mul(out_pos,g_view);				// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos,g_projection);			// 頂点座標（カメラ座標系）をスクリーン座標系に変換
	
	// テクスチャ座標をそのまま出力
	out_tex = in_tex1;					// テクスチャ座標の出力

	// 法線をワールド変換して正規化
	N = normalize(mul(in_normal, (float3x3)g_world));

	// 平行光源の光を正規化
	L = normalize(-g_light_dir);	// 方向を反転している

	// 出力カラーを決める
	out_color = g_ambient * g_ambient_mat + g_diffuse * g_diffuse_mat * max(0.0, dot(L, N));
}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(	float4 in_color : COLOR0,
			float2 in_tex1 : TEXCOORD0,
			out float4 out_color : COLOR0)
{
	if (g_tex)
	{
		float4 tex_color = tex2D(Sampler1, in_tex1);
		// テクスチャの色と頂点の色を掛け合わせる
		out_color = in_color * tex_color;
	}
	else 
	{
		out_color = in_color;
	}
	out_color.a = 1;
}








