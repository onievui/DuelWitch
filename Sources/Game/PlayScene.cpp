#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include "ISceneRequest.h"
#include "DebugCamera.h"
#include "GridFloor.h"
#include "Player.h"
#include "Element.h"
#include "ElementManager.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "TargetCamera.h"
#include "Field.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayScene::PlayScene() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayScene::~PlayScene() {
}

/// <summary>
///	�v���C�V�[��������������
/// </summary>
/// <param name="pSceneRequest"></param>
void PlayScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11& directX = DirectX11::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX.GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX.GetDevice().Get());

	// �G�������g�}�l�[�W�����쐬����
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();
	// ���@�}�l�[�W���𐶐�����
	m_magicManager = std::make_unique<MagicManager>();
	m_magicManager->Initialize();
	m_pMagics = m_magicManager->GetMagics();

	// �v���C���[�𐶐�����
	m_players.emplace_back(std::make_unique<Player>(m_magicManager.get(), PlayerID::Player1,
		DirectX::SimpleMath::Vector3::Zero, Player::MoveDirection::Forward));
	m_players.emplace_back(std::make_unique<Player>(m_magicManager.get(), PlayerID::Player2,
		DirectX::SimpleMath::Vector3(0, 0, 150), Player::MoveDirection::Backward));
	m_players[0]->Create(L"bloom.cmo", L"Resources/Models/Protected");
	m_players[1]->Create(L"bloom.cmo", L"Resources/Models/Protected");
	m_players[0]->SetOtherPlayer(m_players[1].get());
	m_players[1]->SetOtherPlayer(m_players[0].get());

	//�f�o�b�O�J�����𐶐�����
	m_debugCamera = std::make_unique<DebugCamera>(directX.GetWidth(), directX.GetHeight());
	//�^�[�Q�b�g�J�����𐶐�����
	m_targetCamera = std::make_unique<TargetCamera>(m_players[0].get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, float(directX.GetWidth()) / float(directX.GetHeight()), 0.1f, 10000.0f);

	m_players[0]->SetCamera(m_targetCamera.get());
	m_players[1]->SetCamera(m_targetCamera.get());

	//�O���b�h���𐶐�����
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 400.0f, 200);
	// �t�B�[���h�𐶐�����
	m_field = std::make_unique<Field>();
	// �X�v���C�g�t�H���g�𐶐�����
	//m_font = std::make_unique<DirectX::SpriteFont>(DirectX11::Get().GetDevice().Get(), L"myfile.spritefont");
	
}

/// <summary>
/// �v���C�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	// �v���C���[�̍X�V
	for (auto& player : m_players) {
		player->Update(timer);
	}
	// �G�������g�}�l�[�W���̍X�V
	m_elementManager->Update(timer);
	// ���@�}�l�[�W���̍X�V
	m_magicManager->Update(timer);

	if (timer.GetFrameCount() % 30 == 0) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 80);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3::One*-3 + area_offset;
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3::One*3 + area_offset;
		m_elementManager->CreateElement(area_start, area_end, 1);
	}

	// �����蔻��
	// �v���C���[�ƃG�������g�̓����蔻��
	for (auto& element : *m_elementManager->GetElements()) {
		// ���g�p�Ȃ��΂�
		if (!element) {
			continue;
		}
		const SphereCollider* element_collider = element->GetCollider();
		for (auto& player : m_players) {
			if (player->GetCollider()->Collision(element_collider)) {
				element->IsUsed(false);
			}
		}
	}

	// �v���C���\�Ɩ��@�̓����蔻��
	for (auto& magic : *m_magicManager->GetMagics()) {
		// ���g�p�Ȃ��΂�
		if (!magic) {
			continue;
		}
		const SphereCollider* magic_collider = magic->GetCollider();
		for (auto& player : m_players) {
			// ���g�̖��@�Ƃ͔��肵�Ȃ�
			if (player->GetPlayerID() == magic->GetPlayerID()) {
				continue;
			}
			if (player->GetCollider()->Collision(magic_collider)) {
				magic->HitPlayer(*player->GetCollider());
			}
		}
	}

	// �v���C���[���m�̓����蔻��
	for (auto& player1 : m_players) {
		const SphereCollider* player_collider1 = player1->GetCollider();
		for (auto& player2 : m_players) {
			if (player1.get() == player2.get()) {
				continue;
			}
			if (player_collider1->Collision(player2->GetCollider())) {
				player1->CollisionPlayer(*player2);
				player2->CollisionPlayer(*player1);
			}
		}
	}



	// �t�B�[���h�̍X�V
	m_field->Update();

	// �f�o�b�O�J�����̍X�V
	m_debugCamera->Update();
	// �^�[�Q�b�g�J�����̍X�V
	m_targetCamera->Update();
	
}

/// <summary>
/// �v���C�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void PlayScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	//�r���[�s����擾����
	DirectX::SimpleMath::Matrix view = m_targetCamera->GetViewMatrix();
	// �ˉe�s����擾����
	DirectX::SimpleMath::Matrix projection = m_targetCamera->GetProjectionMatrix();

	//�O���b�h����`�悷��
	m_gridFloor->Render(view, projection);
	// �t�B�[���h��`�悷��
	m_field->Render(view, projection);
	// �v���C���[��`�悷��
	for (auto& player : m_players) {
		player->Render(view, projection);
	}
	// �G�������g��`�悷��
	m_elementManager->Render(view, projection);
	// ���@��`�悷��
	m_magicManager->Render(view, projection);
	
	spriteBatch->End();
}

/// <summary>
/// �v���C�V�[�����I������
/// </summary>
void PlayScene::Finalize() {

}

