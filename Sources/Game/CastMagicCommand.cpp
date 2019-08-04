#include "CastMagicCommand.h"
#include "MagicFactory.h"

/// <summary>
///	�R���X�g���N�^
/// </summary>
CastMagicCommand::CastMagicCommand() {
	m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

/// <summary>
/// ���@�r���R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�^�C�}�[</param>
void CastMagicCommand::Execute(Player& player, const DX::StepTimer&  timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	elapsedTime;

	Transform& ref_transform = GetTransform(player);
	DirectX::Mouse::State mouse_state = DirectX::Mouse::Get().GetState();
	m_mouseTracker->Update(mouse_state);

	// �^�b�`�p�b�h���ƍ��N���b�N������ɔ������Ȃ�
	if (m_mouseTracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// ���C�̍쐬
		DirectX::SimpleMath::Ray ray = GetCamera(player).ScreenPointToRay(
			DirectX::SimpleMath::Vector3((float)mouse_state.x, (float)mouse_state.y, 0));
		// ���ʂ̍쐬
		DirectX::SimpleMath::Plane plane = CreatePlaneForMagic(ref_transform, GetMoveDirection(player));
		float distance;
		if (ray.Intersects(plane, distance)) {
			DirectX::SimpleMath::Vector3 ray_pos = ray.position + ray.direction * distance;
			const DirectX::SimpleMath::Vector3& player_pos = ref_transform.GetPosition();
			DirectX::SimpleMath::Vector3 direction = ray_pos - player_pos;
			direction.Normalize();
			std::list<ElementID>& ref_have_elements = GetHaveElements(player);
			// �G�������g���Ȃ��Ȃ�ʏ햂�@�𔭎˂���
			if (ref_have_elements.empty()) {
				GetMagicManager(player).CreateMagic(MagicID::Normal , player.GetPlayerID(), player_pos, direction);
			}
			else {
				ElementID element_id = GetHaveElements(player).front();
				GetHaveElements(player).pop_front();
				GetMagicManager(player).CreateMagic(element_id, player.GetPlayerID(), player_pos, direction);
			}
		}
	}
}

/// <summary>
/// ���@�̂��߂̃��C�p���ʂ̍쐬
/// </summary>
/// <param name="transform">�p��</param>
/// <param name="direction">�i�s����</param>
/// <returns>
/// ����
/// </returns>
DirectX::SimpleMath::Plane CastMagicCommand::CreatePlaneForMagic(const Transform& transform, Player::MoveDirection direction) {
	const DirectX::SimpleMath::Vector3& rot = transform.GetRotation();
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	DirectX::SimpleMath::Vector3 plane_normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ,
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion));
	DirectX::SimpleMath::Vector3 plane_pos = transform.GetPosition() + DirectX::SimpleMath::Vector3::UnitZ *
		(direction == Player::MoveDirection::Forward ? 20.0f : -20.0f);
	DirectX::SimpleMath::Plane plane = DirectX::SimpleMath::Plane(plane_pos, DirectX::SimpleMath::Vector3::UnitZ);
	return plane;
}
