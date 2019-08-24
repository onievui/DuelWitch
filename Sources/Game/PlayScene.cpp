#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\LoadDataManager.h>
#include "ResourceLoader.h"
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
#include "Collision.h"


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
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	// ���\�[�X�����[�h����
	ResourceLoader::Load(ResourceLoaderID::PlayScene);

	// �p�����[�^�����[�h����
	LoadDataManager::GetIns()->Load(LoadDataID::PlayScene);

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
	m_players[0]->Create();
	m_players[1]->Create();
	m_players[0]->SetOtherPlayer(m_players[1].get());
	m_players[1]->SetOtherPlayer(m_players[0].get());

	//�f�o�b�O�J�����𐶐�����
	m_debugCamera = std::make_unique<DebugCamera>(directX->GetWidth(), directX->GetHeight());
	//�^�[�Q�b�g�J�����𐶐�����
	m_targetCamera = std::make_unique<TargetCamera>(m_players[0].get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, static_cast<float>(directX->GetWidth()) / static_cast<float>(directX->GetHeight()), 0.1f, 10000.0f);

	m_players[0]->SetCamera(m_targetCamera.get());
	m_players[1]->SetCamera(m_targetCamera.get());

	//�O���b�h���𐶐�����
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 400.0f, 200);
	// �t�B�[���h�𐶐�����
	m_field = std::make_unique<Field>();
	
}

/// <summary>
/// �v���C�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	// �v���C���[�̍X�V
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// �G�������g�}�l�[�W���̍X�V
	m_elementManager->Update(timer);
	// ���@�}�l�[�W���̍X�V
	m_magicManager->Update(timer);

	static float time = 0;
	time += static_cast<float>(timer.GetElapsedSeconds());
	if (time >= 10.0f) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 23);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3(-3, -3, -2);
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3(3, 3, 2);
		m_elementManager->CreateElement(area_start + area_offset, area_end + area_offset, 3);
		area_offset.z = 127.0f;
		m_elementManager->CreateElement(area_start + area_offset, area_end + area_offset, 3);
		time -= 10.0f;
	}

	// �����蔻��
	// �v���C���[�ƃG�������g�̓����蔻��
	for (std::vector<Element*>::iterator element_itr = m_elementManager->GetElements()->begin(); 
		element_itr != m_elementManager->GetElements()->end(); ++element_itr) {
		// ���g�p�Ȃ��΂�
		if (!*element_itr) {
			continue;
		}
		const SphereCollider* element_collider = (*element_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			if (Collision::HitCheck(element_collider,(*player_itr)->GetCollider())) {
				(*player_itr)->GetElement((*element_itr)->GetID());
				(*element_itr)->IsUsed(false);
			}
		}
	}

	// ���@���m�̓����蔻��
	{
		// ���g�p�Ȃ��΂��������`
		auto pred = [](IMagic* magic) {return magic; };
		// ���@���m�̓����蔻��
		std::vector<IMagic*>::iterator itr = std::find_if(m_magicManager->GetMagics()->begin(), m_magicManager->GetMagics()->end(), pred);
		for (std::vector<IMagic*>::iterator end = m_magicManager->GetMagics()->end(); itr != end;) {
			const SphereCollider* collider = (*itr)->GetCollider();
			std::vector<IMagic*>::iterator next = std::find_if(itr + 1, end, pred);
			for (std::vector<IMagic*>::iterator itr2 = next; itr2 != end; itr2 = std::find_if(itr2 + 1, end, pred)) {
				// ����v���C���[�̖��@�Ȃ画�肵�Ȃ�
				if ((*itr)->GetPlayerID() == (*itr2)->GetPlayerID()) {
					continue;
				}
				if (Collision::HitCheck(collider, (*itr2)->GetCollider())) {
					(*itr)->HitMagic(*itr2);
					(*itr2)->HitMagic(*itr);
				}
			}
			itr = next;
		}
	}

	// �v���C���\�Ɩ��@�̓����蔻��
	for (std::vector<IMagic*>::iterator magic_itr = (*m_magicManager->GetMagics()).begin();
		magic_itr != (*m_magicManager->GetMagics()).end(); ++magic_itr) {
		// ���g�p�Ȃ��΂�
		if (!*magic_itr) {
			continue;
		}
		const SphereCollider* magic_collider = (*magic_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			// ���g�̖��@�Ƃ͔��肵�Ȃ�
			if ((*player_itr)->GetPlayerID() == (*magic_itr)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(magic_collider,(*player_itr)->GetCollider())) {
				(*magic_itr)->HitPlayer(*(*player_itr)->GetCollider());
				(*player_itr)->HitMagic(*magic_itr);
			}
		}
	}

	// �v���C���[���m�̓����蔻��
	for (std::vector<std::unique_ptr<Player>>::iterator itr1 = m_players.begin(); itr1 != m_players.end() - 1; ++itr1) {
		const SphereCollider* collider1 = (*itr1)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator itr2 = itr1 + 1; itr2 != m_players.end(); ++itr2) {
			if (Collision::HitCheck(collider1, (*itr2)->GetCollider())) {
				(*itr1)->HitPlayer(**itr2);
				(*itr2)->HitPlayer(**itr1);
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
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, projection, spriteBatch);
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
	// ���\�[�X���������
	ResourceLoader::Release();
	// �p�����[�^���J������
	LoadDataManager::GetIns()->Dispose(LoadDataID::PlayScene);
}

