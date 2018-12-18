//======================================================================
//	カメラ処理 [camera.cpp]　（2018/4/17）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "camera.h"
#include "manager.h"

CCamera::CCamera()
{
	m_Pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_At    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Projection;
	m_View;
	m_Fov = 90.0f;
	m_Dot = 0;

	m_ArmLength = 0.0f;
	m_ArmLength2D = 0.0f;

	// *--右方向ベクトルを求める--*
	// 上方向ベクトルと前方向ベクトルの外積を求めると2つのベクトルに対して直角のベクトル
	// 2つのベクトルのなす平面の向いてる方向＝法線
	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);	// 外積
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{

}

void CCamera::Init(D3DXVECTOR3 pos, D3DXVECTOR3 at)
{
	m_Pos = pos;
	m_At = at;

	m_Front = at - pos;
	D3DXVec3Normalize(&m_Front, &m_Front); 

	D3DXVECTOR3 vec = m_At - m_Pos;
	m_ArmLength = D3DXVec3Length(&vec);

	D3DXVECTOR3 at2 = m_At;
	at2.y = 0.0f;
	D3DXVECTOR3 position = m_Pos;
	position.y = 0.0f;
	vec = at2 - position;
	m_ArmLength2D = D3DXVec3Length(&vec);
}

void CCamera::Uninit()
{

}

void CCamera::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//	--カメラの処理--
	// 現在の座標を保存

	//if (CManager::GetDebug())
	{
		ImGui::Begin("Camera", 0);
		ImGui::Text("Front:X = %.2f Y = %.2f Z = %.2f", m_Front.x, m_Front.y, m_Front.z);
		ImGui::Text("Right:X = %.2f Y = %.2f Z = %.2f", m_Right.x, m_Right.y, m_Right.z);
		ImGui::Text("Pos  :X = %.2f Y = %.2f Z = %.2f", m_Pos.x, m_Pos.y, m_Pos.z);
		ImGui::End();
	}

	// ビュー行列の作成
	D3DXMatrixLookAtLH(&m_View, &m_Pos, &m_At, &m_Up);	//ビュー行列を作ってくれる便利関数

	// プロジェクション（パースペクティブ）行列の作成
	// 引数＝(行列ポインタ, 画角, アスペクト比, ニアクリップ(0より大きい値、1で1mくらい), ファークリップ)
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DXToRadian(m_Fov) /* またはD3DX_PI / 3　*/, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_CLIP, FAR_CLIP);
}

void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

void CCamera::SetAt(D3DXVECTOR3 at)
{
	m_At = at;
}

void CCamera::SetFov(float fov)
{
	m_Fov = fov;
}

void CCamera::Move(D3DXVECTOR3 pos)
{
	m_Pos += pos;
	m_At += pos;
}

void CCamera::Move(float horizontal, float vertical)
{
	//*********************************************************
	//	左右移動
	//*********************************************************
	{
		D3DXVec3Normalize(&m_Right, &m_Right);
		m_Pos += m_Right * CAMERA_MOOVE_SPEED * horizontal;
		m_At += m_Right * CAMERA_MOOVE_SPEED * horizontal;

	}

	//*********************************************************
	//	前後移動
	//*********************************************************
	{
		// 前方向を求める
		m_Front = m_At - m_Pos;

		// m_Frontを正規化する
		D3DXVec3Normalize(&m_Front, &m_Front);

		D3DXVECTOR3 vec = m_Front; // 移動方向
		vec.y = 0;

		// vecを正規化する
		D3DXVec3Normalize(&vec, &vec);

		m_Pos += vec * CAMERA_MOOVE_SPEED * vertical;
		m_At += vec * CAMERA_MOOVE_SPEED * vertical;
	}
}

void CCamera::Rotation(float horizontal, float vertical)
{
	//*********************************************************
	//	左右回転
	//*********************************************************
	{
		// 回転行列を宣言
		D3DXMATRIX mtxRotation;

		// 回転行列(Y軸回転)を作る(回転速度)
		D3DXMatrixRotationY(&mtxRotation, CAMERA_AT_RAD * horizontal);

		D3DXVECTOR3 dir = m_At - m_Pos;

		D3DXVec3TransformNormal(&dir, &dir, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)

		m_At = dir + m_Pos;

		D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
		D3DXVec3Normalize(&m_Front, &m_Front);
		D3DXVec3Normalize(&m_Right, &m_Right);
	}

	//*********************************************************
	//	上下回転
	//*********************************************************
	{
		// 回転行列を宣言
		D3DXMATRIX mtxRotation;

		// 回転行列を作る(回転速度)
		D3DXMatrixRotationAxis(&mtxRotation, &m_Right, CAMERA_AT_RAD * vertical);

		D3DXVECTOR3 dir = m_At - m_Pos;

		D3DXVec3TransformNormal(&dir, &dir, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)

		D3DXVECTOR3 front;	

		D3DXVec3TransformNormal(&front, &m_Front, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)

		D3DXVECTOR3 vec = front; // 移動方向
		vec.y = 0;

		// vecを正規化する
		D3DXVec3Normalize(&vec, &vec);

		m_Dot = D3DXVec3Dot(&front, &vec);

		// 上下の限界を設ける
		if (m_Dot > 0.5f)
		{
			m_At = dir + m_Pos;
			//	m_Pos = m_At - dir;
			D3DXVec3TransformNormal(&m_Front, &m_Front, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
			D3DXVec3TransformNormal(&m_Right, &m_Right, &mtxRotation);	// ベクトルの座標変換(出力, 入力, 変換行列)
			D3DXVec3Normalize(&m_Front, &m_Front);
			D3DXVec3Normalize(&m_Right, &m_Right);
		}
	}
}

CCamera* CCamera::Create()
{
	CCamera* camera= new CCamera();
	camera->Init(D3DXVECTOR3(0.0f, 3.0f, -3.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f));

	return camera;
}

CCamera* CCamera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 at)
{
	CCamera* camera = new CCamera();
	camera->Init(pos, at);

	return camera;
}

D3DXMATRIX CCamera::GetView()
{
	return m_View;
}

void CCamera::SetRot(D3DXVECTOR3 rot)
{

}
