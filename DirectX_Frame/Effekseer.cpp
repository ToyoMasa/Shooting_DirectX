
#include "common.h"
#include "main.h"
#include "camera.h"
#include "Effekseer.h"

CEffekseer* CEffekseer::Create(Effect EffectType, CCamera* camera)
{
	CEffekseer* Effekseer = new CEffekseer(EffectType, LAYER_OBJECT3D);
	Effekseer->Init(camera);
	return Effekseer;
}
void  CEffekseer::CreateScene(Effect EffectType)
{
	CEffekseer* Effekseer = new CEffekseer(EffectType, LAYER_OBJECT3D);
}

void CEffekseer::LoadEffect()
{
	//エフェクトの読み込み
	effekseerEffect_ = Effekseer::Effect::Create(effekseerManager_, (const EFK_CHAR*)fileName_[effectType_].fileName);
}
void CEffekseer::Init(CCamera* camera)
{
	m_Camera = camera;
}
void CEffekseer::Uninit()
{
	// エフェクトの停止
	this->Stop();

	//エフェクトを解放。再生中の場合は再生が終了した際、自動的に解放
	ES_SAFE_RELEASE(effekseerEffect_);

	//エフェクト管理用インスタンスを破棄
	effekseerManager_->Destroy();

	//描画用インスタンスを破棄
	effekseerRenderer_->Destroy();

	effekseerHandle_ = NULL;

}
void CEffekseer::Update()
{

	//CCamera::CAMERA_STATUS camera = CCamera::GetMainCamera();

	this->SetView(m_Camera->GetPos(), m_Camera->GetAt(), m_Camera->GetUp());

	playing_ = effekseerManager_->Exists(effekseerHandle_);
	if (!playing_)
	{
		if (this->repeat_)
		{
			//エフェクトの再生
			Play();
		}
		else
		{
			Stop();
		}
	}

	effekseerManager_->Flip();
	//すべてのエフェクトの更新
	effekseerManager_->Update();
}
void CEffekseer::Draw()
{
	//effekseerRenderer_->SetRenderMode(Effekseer::RenderMode::Wireframe);
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}
	D3DXMATRIX mtxScale, mtxTrans, mtxWorld, mtxRotateX, mtxRotateY, mtxRotateZ;

	D3DXMatrixTranslation(&mtxTrans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixScaling(&mtxScale, transform_.scale.x, transform_.scale.y, transform_.scale.z);
	D3DXMatrixRotationX(&mtxRotateX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationZ(&mtxRotateZ, D3DXToRadian(transform_.rotate.z));

	mtxWorld = mtxScale;
	mtxWorld *= mtxRotateX;
	mtxWorld *= mtxRotateY;
	mtxWorld *= mtxRotateZ;
	mtxWorld *= mtxTrans;

	this->SetMatrix(mtxWorld);

	if (GetVisible())
	{
		effekseerRenderer_->BeginRendering();;
		effekseerManager_->Draw();
		effekseerRenderer_->EndRendering();
	}
}
void CEffekseer::SetProj()
{
	//投影行列の更新
	
	effekseerRenderer_->SetProjectionMatrix(Effekseer::Matrix44().PerspectiveFovLH(D3DXToRadian(90.0f),                           //視野角
		(float)SCREEN_WIDTH / SCREEN_HEIGHT,   //アスペクト比
		0.1f, 10000.0f));
}
void CEffekseer::SetView(D3DXVECTOR3 Pos, D3DXVECTOR3 At, D3DXVECTOR3 Up)
{
	//カメラ行列の更新
	effekseerRenderer_->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(
		Effekseer::Vector3D(Pos.x, Pos.y, Pos.z),
		Effekseer::Vector3D(At.x, At.y, At.z),
		Effekseer::Vector3D(Up.x, Up.y, Up.z)
	));
}
void CEffekseer::SetLocation(D3DXVECTOR3 pos)
{
	transform_.pos = pos;
	effekseerManager_->SetLocation(effekseerHandle_, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}
void CEffekseer::SetSpeed(float speed)
{
	effekseerManager_->SetSpeed(effekseerHandle_, speed);
}
void CEffekseer::RepeatEffect(bool repeat)
{
	this->repeat_ = repeat;
}
void CEffekseer::Play()
{
	//エフェクトの再生
	effekseerHandle_ = effekseerManager_->Play(effekseerEffect_, transform_.pos.x, transform_.pos.y, transform_.pos.z);

	effekseerManager_->Flip();

	
	playing_ = true;
}
void CEffekseer::Stop()
{
	//エフェクトの停止
	effekseerManager_->StopEffect(effekseerHandle_);
}
void CEffekseer::SetMatrix(D3DXMATRIX mat)
{
	Effekseer::Matrix43 matrix;
	matrix.Value[0][0] = mat._11;
	matrix.Value[0][1] = mat._12;
	matrix.Value[0][2] = mat._13;
	matrix.Value[1][0] = mat._21;
	matrix.Value[1][1] = mat._22;
	matrix.Value[1][2] = mat._23;
	matrix.Value[2][0] = mat._31;
	matrix.Value[2][1] = mat._32;
	matrix.Value[2][2] = mat._33;
	matrix.Value[3][0] = mat._41;
	matrix.Value[3][1] = mat._42;
	matrix.Value[3][2] = mat._43;

	effekseerManager_->SetMatrix(effekseerHandle_,matrix);
}
bool CEffekseer::GetPlaying()
{
	return playing_;
}
void CEffekseer::SetVisible(bool flag)
{
	visible_ = flag;
}
bool CEffekseer::GetVisible()
{
	return visible_;
}
void CEffekseer::SetPosition(float x, float y, float z)
{
	transform_.pos = D3DXVECTOR3(x,y,z);
}
void CEffekseer::SetPosition(D3DXVECTOR3 trans)
{
	transform_.pos = trans;
}
D3DXVECTOR3 CEffekseer::GetPosition()
{
	return transform_.pos;
}
void CEffekseer::SetRotate(float x, float y, float z)
{
	transform_.rotate = D3DXVECTOR3(x,y,z);
}
void CEffekseer::SetRotate(D3DXVECTOR3 rot)
{
	transform_.rotate = rot;
}
D3DXVECTOR3 CEffekseer::GetRotate()
{
	return transform_.rotate;
}
void CEffekseer::SetScale(float x, float y, float z)
{
	transform_.scale = D3DXVECTOR3(x,y,z);
}
void CEffekseer::SetScale(D3DXVECTOR3 scale)
{
	transform_.scale = scale;
}
D3DXVECTOR3 CEffekseer::GetScale()
{
	return transform_.scale;
}
void CEffekseer::Paused(bool paused)
{
	this->paused_ = paused;
	effekseerManager_->SetPaused(effekseerHandle_, paused);
}
