#include "RenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>


void RenderCommand::RenderPlayerModel(const Player& player, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const PlayerStatus& ref_status = GetStatus(player);

	// �_���[�W��̓v���C���[��_�ł�����
	const bool not_damaged = ref_status.damageTimer <= 0.0f;
	const bool time_sin_positive = sin(ref_status.damageTimer*Math::PI2 * 2) > 0;
	if (not_damaged || time_sin_positive) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::Bloom)->GetResource();
		const std::unique_ptr<DirectX::Model>& model2 = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::Chara)->GetResource();

		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		const DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

		// �L�������f���̃I�t�Z�b�g
		DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateRotationX(-Math::HarfPI);
		mat *= DirectX::SimpleMath::Matrix::CreateScale(0.5f);
		mat *= DirectX::SimpleMath::Matrix::CreateTranslation(0, 0.625f, -0.2f);
		mat *= GetTransform(player).GetMatrix();

		// ���f����`�悷��
		model->Draw(context, *states, GetTransform(player).GetMatrix(), view, proj);
		model2->Draw(context, *states, mat, view, proj);

		// �����蔻���`�悷��
		//GetCollider(player).Render(view, proj, DirectX::SimpleMath::Color(1, 1, 1, 0.3f), true);
	}
}
