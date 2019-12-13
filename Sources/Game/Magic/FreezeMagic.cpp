#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include <Game\Collision\SphereCollider.h>

#include <Utils\ConstBuffer.h>
#include <Game\Camera\TargetCamera.h>

/// <summary>
/// コンストラクタ
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic()
	, m_pPlayerPos()
	, m_time()
	, m_rotateRadius() {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.radius);

	// 定数バッファの作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(IceBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());
}

/// <summary>
/// デストラクタ
/// </summary>
FreezeMagic::~FreezeMagic() {
}

/// <summary>
/// 氷魔法を生成する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">方向</param>
void FreezeMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	static_cast<SphereCollider*>(m_collider.get())->SetRadius(parameter.radius);
	m_pPlayerPos = &pos;
	m_time = 0.0f;
	m_rotateRadius = 0.0f;
	m_color = DirectX::Colors::SkyBlue + DirectX::SimpleMath::Color(0, 0, 0, 0.8f);
	// 向きをXZ方向に変換する
	m_vel = dir;
	m_vel.y = 0.0f;

	m_lifeTime = parameter.lifeTime;
}

/// <summary>
/// 氷魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	// 生存時間が無くなったら消滅する
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}

	// タイマーを進める
	m_time += elapsed_time;

	// プレイヤーの位置を中心とする
	DirectX::SimpleMath::Vector3 center_pos = *m_pPlayerPos;
	// 回転半径を徐々に特定の値に近づける
	m_rotateRadius = Math::Lerp(m_rotateRadius, parameter.rotateRadius, 0.2f);
	// 時間に応じて回転させる
	DirectX::SimpleMath::Quaternion rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, m_time*parameter.rotateSpeed);
	// 回転後の位置を求める
	DirectX::SimpleMath::Vector3 add_pos = DirectX::SimpleMath::Vector3::Transform(m_vel*m_rotateRadius, rot);
	
	// プレイヤーの位置に回転した位置を足す
	m_transform.SetPosition(center_pos + add_pos);

	// オブジェクト自身も回転させる
	m_transform.SetRotation(rot);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 氷魔法を開放する
/// </summary>
void FreezeMagic::Lost() {

}

/// <summary>
/// 氷魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void FreezeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::FreezeMagic);

	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	// 定数バッファの作成
	IceBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	DirectX::SimpleMath::Vector3 eye = ServiceLocater<TargetCamera>::Get()->GetEyeVector();
	cbuff.eye = DirectX::SimpleMath::Vector4(eye.x, eye.y, eye.z, 0);

	//定数バッファの内容更新
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// シェーダに定数バッファを割り当てる
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(2, 1, cb);

	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Ice);

	resource->GetResource()->Draw(m_world, view, proj, m_color, texture->GetResource().Get(), false,[=]() {
		ID3D11BlendState* blendstate = states->NonPremultiplied();
		// 透明判定処理
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
		// 深度バッファに参照する
		context->OMSetDepthStencilState(states->DepthDefault(), 0);
		// 時計回りカリング
		context->RSSetState(states->CullClockwise());

		// ピクセルシェーダにサンプラーを割り当てる
		ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
		context->PSSetSamplers(0, 1, sampler);
		// 各シェーダを割り当てる
		VertexShaderResource* vertex_shader = ServiceLocater<ResourceManager<VertexShaderResource>>::Get()
			->GetRawResource(VertexShaderID::Ice);
		const PixelShaderResource* pixel_shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()
			->GetResource(PixelShaderID::Default);

		// 入力レイアウトを割り当てる
		context->IASetInputLayout(vertex_shader->GetInputLayout());

		context->VSSetShader(vertex_shader->GetResource().Get(), nullptr, 0);
		context->PSSetShader(pixel_shader->GetResource().Get(), nullptr, 0);
	});
	//m_collider->Render(view, proj);

	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

/// <summary>
/// ダメージを取得する
/// </summary>
/// <returns>
/// ダメージ量
/// </returns>
float FreezeMagic::GetPower() const {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	return parameter.power*m_info.powerRate;
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void FreezeMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void FreezeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// 炎魔法と衝突したら消える
	if (other_id == MagicID::Fire) {
		m_isUsed = false;
	}
}
