#include "RenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>


/// <summary>
/// ��HP�̍X�V
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void RenderCommand::UpdateRedHpBar(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	const PlayerStatus& status = GetStatus(player);

	// �_���[�W���󂯂��ꍇ�̓^�C�}�[���N������
	if (m_preHp > status.hp) {
		m_preHp = status.hp;
		m_redHpTime = RED_HP_TIME;
	}

	// �_���[�W����łȂ��ꍇ
	if (m_redHpTime <= 0.0f) {
		// ��HP�������c���Ă����猸��������
		if (m_redHp > status.hp) {
			m_redHp -= RED_HP_SPEED * elapsed_time;
			if (m_redHp < status.hp) {
				m_redHp = status.hp;
			}
		}
	}
	// �_���[�W���󂯂���̏ꍇ
	else {
		// ��莞�Ԍo�߂���܂ő҂�
		m_redHpTime -= elapsed_time;
	}
}

/// <summary>
/// �v���C���[���f���̕`��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void RenderCommand::RenderPlayerModel(const Player& player, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const PlayerStatus& ref_status = GetStatus(player);
	
	// �_���[�W��̓v���C���[��_�ł�����
	const bool not_damaged = ref_status.damageTimer <= 0.0f;
	const bool time_sin_positive = sin(ref_status.damageTimer*Math::PI2 * 2) > 0;
	if (not_damaged || time_sin_positive) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::Bloom)->GetResource();
		const std::unique_ptr<DirectX::Model>& model2 = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::Chara)->GetResource(ref_status.charaId);

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
