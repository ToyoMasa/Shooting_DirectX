//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4	g_world;				// ワールド変換行列
float4x4 	g_view;					// カメラ変換行列
float4x4 	g_projection;			// プロジェクション変換行列

// ライトパラメータ
float4		g_light_pos;			// 座標(ビュー空間)
float3		g_light_dir;			// 方向(ビュー空間)
float4		g_light_diff;           // ディフューズカラー
float4		g_light_specular;		// スペキュラカラー
float4		g_light_ambient;		// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
float4		g_falloff_param;		// x:距離  y:内筒と外筒間のぼかし  z:減衰処理パラメータ1（スペキュラ―の強さに影響）  w:減衰処理パラメータ2（明るさに影響）
float4		g_light_param;			// x:減衰処理パラメータ3（光の届く距離に影響）  y:スポットライトパラメータ1( cos( Phi / 2.0f ) )  z:スポットライト用パラメータ2( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

float4		g_mat_ambient;
float4		g_mat_emissive;
float4		g_mat_diffuse;
float4		g_mat_specular;
float4		g_mat_power;
float		g_alpha;

float4		g_camera_pos;			// カメラ座標

// 光源の位置にカメラを置いたカメラ行列
float4x4    g_light_view;
float4x4	g_light_projection;

float4x4    g_matuv;

//------------------------------------------------
// サンプラー1
//------------------------------------------------
sampler Sampler1 =
sampler_state
{
	Texture = <g_texture>;
	MinFilter = LINEAR;		// リニアフィルタ（縮小時）
	MagFilter = LINEAR;		// リニアフィルタ（拡大時）
};

sampler ProjectionShadowSampler =
sampler_state
{
	MipFilter = POINT;
	MinFilter = POINT;		// リニアフィルタ（縮小時）
	MagFilter = POINT;		// リニアフィルタ（拡大時）
	AddressU = CLAMP;
	AddressV = CLAMP;
};

// main関数
void main(
	float3 in_pos : POSITION,
	float3 in_normal : NORMAL,
	float4 in_diff : COLOR0,
	float2 in_tex : TEXCOORD0,
	out float4 out_pos : POSITION,
	out float4 out_diff : COLOR0,
	out float2 out_tex : TEXCOORD0,
	out float3 out_normal : TEXCOORD1,
	out float3 out_posforps : TEXCOORD2,
	out float4 out_shadowtexuv : TEXCOORD3,
	out float4 out_color : COLOR1)
{
	// 座標変換して出力（ワールド変換後の頂点も出力）
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
	out_posforps = out_pos.xyz;							// ワールド変換済み座標
	out_pos = mul(out_pos, g_view);						// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_projection);				// 頂点座標（カメラ座標系）をスクリーン座標系に変換
						
	// 法線をワールド空間上のベクトルに変換して、単位ベクトル化してから出力
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

	out_diff = in_diff;
	out_tex = in_tex;

	// ライトの目線によるワールドビュー射影変換をする
	float4x4 mat;
	mat = mul(g_world, g_light_view);
	mat = mul(mat, g_light_projection);
	out_shadowtexuv = mul(float4(in_pos, 1.0f), mat);

	// 法線とライトの方向から頂点の色を決定
	// 濃くなりすぎないように調節しています
	float3 N = normalize(mul(in_normal, (float3x3)g_world));
	float3 LightDirect = normalize(float3(g_light_view._13, g_light_view._23, g_light_view._33));
	out_color = float4(0.0, 0.6, 1.0, 1.0) * (0.3f + dot(N, -LightDirect) * (1 - 0.3f));
}

// main関数
void PS(
	float2 in_tex	: TEXCOORD0,
	float4 in_diff : COLOR0,
	float4 in_color : COLOR1,
	float3 in_normal : TEXCOORD1,    // 法線( ビュー空間 )
	float3 in_posforps : TEXCOORD2,    // 座標( ビュー空間 ))
	float4 in_shadowtexuv : TEXCOORD3, // 影のUV座標
	out float4 out_color : COLOR0)
{
	float4 tex_color;
	float4 specular_color;
	float3 normal;
	float diffuse_angle_gen;
	float4 diffuse;
	float4 specular;
	float3 vtx_to_camera;
	float3 half_vec;
	float power;
	float3 light_to_vtx;
	float light_gen;
	float3 light_dir;
	float light_dir_cosa;

	// 法線の準備
	normal = normalize(in_normal);

	// 頂点座標から視点へのベクトルを正規化
	vtx_to_camera = normalize(g_camera_pos.xyz - in_posforps);

	// ディフューズカラーとスペキュラカラーの初期化
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ライト方向ベクトルの計算
	light_dir = normalize(in_posforps - g_light_pos.xyz);

	// 頂点とライト位置との距離の二乗を求める
	light_to_vtx = in_posforps - g_light_pos.xyz;
	float temp = length(light_to_vtx) * length(light_to_vtx);

	// 減衰率の計算
	light_gen = 1.0f / (g_falloff_param.z + g_falloff_param.w * length(light_to_vtx) + g_light_param.x * temp);

	// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積を計算 
	light_dir_cosa = dot(light_dir, g_light_dir);

	// スポットライト減衰計算
	light_gen *= saturate(pow(abs(max(light_dir_cosa - g_light_param.y, 0.0f) * g_light_param.z), g_falloff_param.y));

	// 有効距離外の時は減衰率を最大にする
	light_gen *= step(length(light_to_vtx), g_falloff_param.x);

	// ディフューズ角度減衰率計算
	diffuse_angle_gen = saturate(dot(normal, -light_dir));

	// ディフューズに減衰計算を追加
	diffuse += (g_light_diff * in_diff * diffuse_angle_gen + g_light_ambient) * light_gen;

	// ハーフベクトルの計算
	half_vec = normalize(vtx_to_camera - light_dir);

	// パワーの計算
	power = pow(max(0.0f, dot(normal, half_vec)), g_mat_power.x);

	// スペキュラカラーの計算
	specular = power * light_gen.x * g_light_specular;

	// ディフューズカラーにアンビエントカラーとエミッシブカラーを加算
	diffuse = max(diffuse, g_mat_ambient * in_diff);

	// マテリアルのスペキュラカラー
	specular_color = specular * g_mat_specular;

	// ライト目線によるZ値の再算出
	float ZValue = in_shadowtexuv.z / in_shadowtexuv.w;

	// テクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + in_shadowtexuv.x / in_shadowtexuv.w) * 0.5f;
	TransTexCoord.y = (1.0f - in_shadowtexuv.y / in_shadowtexuv.w) * 0.5f;

	// 同じ座標のZ値を抽出
	float SM_Z = tex2D(ProjectionShadowSampler, TransTexCoord).x;

	// 出力カラー
	tex_color = tex2D(Sampler1, in_tex);
	out_color.rgb = tex_color.rgb * diffuse.rgb + specular_color.rgb;

	// 透明度
	out_color.a = tex_color.a * in_diff.a * g_alpha;

	// 影を追加
	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	float4 shadow_color;
	if (ZValue < SM_Z + 0.005f)
	{
		out_color.rgb = tex_color.rgb * diffuse.rgb * 0.15f;
	}
}
