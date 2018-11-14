//------------------------------------------------
// グローバル変数
//------------------------------------------------

float4x4	g_world;					// ワールド変換行列
float4x4 	g_view;						// カメラ変換行列
float4x4 	g_projection;				// プロジェクション変換行列

bool		g_tex;						// テクスチャのありなし　false:なし　true:あり

										// 光
float4		g_diffuse;					// ディフューズ
float4		g_emmisive;					// エミッシブ
float4		g_ambient;					// 環境光
float4		g_specular;					// スペキュラー光

float3		g_light_dir;				// 平行光源の方向

										// マテリアル
float4		g_diffuse_mat;				// ディフューズ光
float4		g_emmissive_mat;				// エミッシブ光
float4		g_ambient_mat;				// 環境光
float4		g_specular_mat;				// スペキュラー
float		g_power;					// スペキュラー光のパワー値

										// スペキュラ光の計算用
float4		g_camerapos;				// カメラ位置

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
// 頂点シェーダ
//------------------------------------------------
void main(float3 in_pos		: POSITION,
	float3 in_normal : NORMAL,
	float2 in_tex1 : TEXCOORD0,
	out float4 out_pos : POSITION,		//変換後頂点出力	
	out float2 out_tex1 : TEXCOORD0,	//テクスチャ座標出力
	out float3 out_normal : TEXCOORD1,	//ワールド変換後法線出力
	out float3 out_posforps : TEXCOORD2)	//ワールド変換後頂点出力
{

	// 座標変換して出力（ワールド変換後の頂点も出力）
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
	out_posforps = out_pos.xyz;							// ワールド変換済み座標
	out_pos = mul(out_pos, g_view);						// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_projection);				// 頂点座標（カメラ座標系）をスクリーン座標系に変換


	// テクスチャ座標をそのまま出力する
	out_tex1 = in_tex1;			// テクスチャ座標の出力


	// 法線をワールド空間上のベクトルに変換して、単位ベクトル化してから出力
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(float2 in_tex1		: TEXCOORD0,	//テクスチャ座標入力
	float3 in_normal : TEXCOORD1,	//ピクセルの法線入力
	float3 in_posforps : TEXCOORD2,	//ピクセルの座標入力
	out float4 out_color : COLOR0)			//ピクセルカラー出力
{
	float3	N;		// ワールド空間上の法線ベクトル
	float3	L;		// 光の差し込む方向
	float3	P;		// ワールド空間上の頂点座標
	float3	V;		// （カメラ座標ー頂点座標）ベクトル
	float3  H;		//  ハーフベクトル（視線ベクトルと光の方向ベクトル）

					// ワールド空間での座標
	P = in_posforps;

	// ワールド空間での法線ベクトル（正規化が必要）
	N = normalize(in_normal);

	// 平行光の差し込む方向	単位ベクトル化
	L = normalize(-(g_light_dir));

	// 視線ベクトルを求める＆正規化
	V = g_camerapos.xyz - P;
	V = normalize(V);

	// ハーフベクトルを求める
	H = normalize(L + V);

	// 出力カラーを決める
	float4 diffuse = g_ambient * g_ambient_mat +
		g_diffuse * g_diffuse_mat *
		max(0.0, dot(L, N));
	diffuse.w = 1.0f; 

	// スペキュラー光による色を計算
	float4 specular = g_specular * g_specular_mat *
		pow(max(0, dot(N, H)), 50);



	//色の出力
	if (g_tex) {
		float4 tex_color = tex2D(Sampler1, in_tex1);
		// テクスチャの色と頂点の色を掛け合わせる
		out_color = saturate(diffuse * tex_color + specular);
	}
	else {
		out_color = saturate(diffuse + specular);
	}
}
