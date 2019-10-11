#include "AIRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// AI�`��R�}���h������������
/// </summary>
/// <param name="player">�v���C���[</param>
void AIRenderCommand::Initialize(Player& player) {
	player;
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	//�o�b�`�G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);
}

/// <summary>
/// AI�`��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void AIRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	player, timer;
}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void AIRenderCommand::Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	// �_���[�W��̓v���C���[��_�ł�����
	const auto& ref_status = GetStatus(player);
	if (ref_status.damageTimer <= 0.0f || sin(ref_status.damageTimer*Math::PI2 * 2) > 0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		const DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
		// ���f����`�悷��
		model->Draw(context, *states, GetTransform(player).GetMatrix(), view, proj);
		// �����蔻���`�悷��
		GetCollider(player).Render(view, proj, DirectX::SimpleMath::Color(1, 1, 1, 0.3f), true);
	}

	// HP�o�[��`�悷��
	RenderHpBar(player, spriteBatch);
}

/// <summary>
/// HP�o�[��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void AIRenderCommand::RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::HpBar);

	const Camera& camera = GetCamera(player);
	const Transform& transform = GetTransform(player);
	const float scale = 0.15f;

	// �����x�N�g���Ƃ̂Ȃ��p�����߂�
	DirectX::SimpleMath::Vector3 eye = camera.GetEyeVector();
	DirectX::SimpleMath::Vector3 target = transform.GetPosition() - camera.GetEyePosition();
	// �J���������ɂ���Ȃ珈�����Ȃ�
	if (eye.Dot(target) < 0.0f) {
		return;
	}

	// HP�o�[�̃��[���h���W
	DirectX::SimpleMath::Vector3 world_pos = transform.GetPosition() + DirectX::SimpleMath::Vector3(0, 3, 0);
	// HP�o�[�̃X�N���[�����W
	DirectX::SimpleMath::Vector2 screen_pos = camera.WorldToScreenPoint(world_pos);

	// ���̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(2).Get(), screen_pos, nullptr,
		DirectX::Colors::White, 0, texture->GetCenter(), scale);

	// �摜�̋�`���쐬����
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const Player::Status& status = GetStatus(player);
	RECT rect;
	rect.left = 0; rect.top = 0;
	// HP�̊����ɉ����ĕ`��͈͂����߂�
	rect.right = static_cast<LONG>(size.x*status.hp / status.maxHp);
	rect.bottom = static_cast<LONG>(size.y);

	// �Ԃ̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(1).Get(), screen_pos, &rect,
		DirectX::Colors::White, 0, texture->GetCenter(), scale);
	// �΂̕�����`�悷��
	spriteBatch->Draw(texture->GetResource(0).Get(), screen_pos, &rect,
		DirectX::Colors::White, 0, texture->GetCenter(), scale);
}
