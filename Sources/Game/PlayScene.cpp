#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include "ISceneRequest.h"
#include "DebugCamera.h"
#include "GridFloor.h"
#include "Player.h"
#include "Element.h"
#include "ElementManager.h"
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

	// ���f���I�u�W�F�N�g�𐶐�����
	m_player = std::make_unique<Player>();
	m_player->Create(L"bloom.cmo", L"Resources/Models/Protected");
	// �G�������g�}�l�[�W�����쐬����
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();
	m_pElements = m_elementManager->GetElements();
	//�f�o�b�O�J�����𐶐�����
	m_debugCamera = std::make_unique<DebugCamera>(directX.GetWidth(), directX.GetHeight());
	//�^�[�Q�b�g�J�����𐶐�����
	m_targetCamera = std::make_unique<TargetCamera>(m_player.get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, float(directX.GetWidth()) / float(directX.GetHeight()), 0.1f, 10000.0f);
	//�O���b�h���𐶐�����
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 200.0f, 50);
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
	m_player->Update(timer);
	// �G�������g�}�l�[�W���̍X�V
	m_elementManager->Update(timer);

	if (timer.GetFrameCount() % 30 == 0) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 80);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3::One*-3 + area_offset;
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3::One*3 + area_offset;
		m_elementManager->CreateElement(area_start, area_end, 1);
	}

	// �����蔻��
	const SphereCollider* player_collider = m_player->GetCollider();
	for (auto& element : *m_pElements) {
		// ���g�p�Ȃ��΂�
		if (!element) {
			continue;
		}
		const SphereCollider* element_collider = element->GetCollider();
		if (player_collider->Collision(element_collider)) {
			element->IsUsed(false);
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
	// ���f����`�悷��
	m_player->Render(view, projection);
	// �G�������g��`�悷��
	m_elementManager->Render(view, projection);
	
	spriteBatch->End();
}

/// <summary>
/// �v���C�V�[�����I������
/// </summary>
void PlayScene::Finalize() {

}

