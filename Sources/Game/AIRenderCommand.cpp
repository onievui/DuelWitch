#include "AIRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>


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
	spriteBatch;
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
}
