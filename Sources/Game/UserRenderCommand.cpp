#include "UserRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// ���[�U�`��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void UserRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	player, timer;
}

/// <summary>
/// �v���C���[��`�悷��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void UserRenderCommand::Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
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
		GetCollider(player).Render(view, proj);
	}

	// �����G�������g��`�悷��
	const std::list<ElementID>& have_elements = GetHaveElements(player);
	int i = have_elements.size() - 1;
	for (std::list<ElementID>::const_reverse_iterator itr = have_elements.rbegin(); itr != have_elements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(20 + i * 40.0f, 630.0f), nullptr,
			DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.5f, 1.5f));
		--i;
	}

	// �Ə���`�悷��
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	spriteBatch->Draw(texture->GetResource().Get(),
		mouse_pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
}
