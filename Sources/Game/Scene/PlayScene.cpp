#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include <Utils\LamdaUtils.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Load\ResourceLoader.h>
#include "ISceneRequest.h"
#include <Game\Camera\DebugCamera.h>
#include <Game\Camera\TargetCamera.h>
#include <Game\Player\Player.h>
#include <Game\Player\PlayerID.h>
#include <Game\Element\Element.h>
#include <Game\Element\ElementManager.h>
#include <Game\Magic\IMagic.h>
#include <Game\Magic\MagicManager.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Field\Field.h>
#include <Game\Field\GridFloor.h>
#include <Game\Collision\Collision.h>


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

	// �}�E�X�𑊑΃��[�h�ɕύX����
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
	// �J�[�\������ʂ̒��S�Ɉړ�������
	ServiceLocater<MouseWrapper>::Get()->SetPos(DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f));

	// ���\�[�X�����[�h����
	ResourceLoader::Load(ResourceLoaderID::PlayScene);

	// �p�����[�^�����[�h����
	m_parameterLoader = std::make_unique<PlayParameterLoader>();
	m_parameterLoader->Load();
	ServiceLocater<PlayParameterLoader>::Register(m_parameterLoader.get());

	// �G�t�F�N�g�}�l�[�W���𐶐�����
	m_effectManager = std::make_unique<EffectManager>();
	m_effectManager->Initialize();
	// �G�t�F�N�g�}�l�[�W�����T�[�r�X���P�[�^�ɓo�^����
	ServiceLocater<EffectManager>::Register(m_effectManager.get());

	// �t�B�[���h�𐶐�����
	m_field = std::make_unique<Field>();

	// �G�������g�}�l�[�W�����쐬����
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();
	// ���@�}�l�[�W���𐶐�����
	m_magicManager = std::make_unique<MagicManager>();
	m_magicManager->Initialize();
	m_pMagics = m_magicManager->GetMagics();
	
	// �v���C���[�𐶐�����
	DirectX::SimpleMath::Vector3 player_pos(0, 0, -75);
	DirectX::SimpleMath::Quaternion player_pos_rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / 3);
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player1, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player2, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player3, player_pos));

	//�f�o�b�O�J�����𐶐�����
	m_debugCamera = std::make_unique<DebugCamera>(directX->GetWidth(), directX->GetHeight());
	//�^�[�Q�b�g�J�����𐶐�����
	m_targetCamera = std::make_unique<TargetCamera>(nullptr, DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		PerspectiveFovInfo(Math::HarfPI*0.5f, static_cast<float>(directX->GetWidth()) / static_cast<float>(directX->GetHeight()), 0.1f, 5000.0f));

	// �v���C���[������������
	m_players[0]->Initialize(m_magicManager.get(), m_targetCamera.get(), m_players);
	m_players[1]->Initialize(m_magicManager.get(), m_targetCamera.get(), m_players);
	m_players[2]->Initialize(m_magicManager.get(), m_targetCamera.get(), m_players);

	//�O���b�h���𐶐�����
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 200.0f, 100);
	
}

/// <summary>
/// �v���C�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	// �G�X�P�[�v�L�[�Ń|�[�Y��ʂ��Ăяo��
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		m_pSceneRequest->RequestScene("Pause", RequestSceneType::StackScene);
	}

	// F2�L�[�Ńp�����[�^���ēǂݍ��݂���
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F2)) {
		m_parameterLoader->Reload();
	}

	// �v���C���[���X�V����
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// �G�������g�}�l�[�W�����X�V����
	m_elementManager->SetRadius(m_field->GetRadius());
	m_elementManager->Update(timer);
	// ���@�}�l�[�W�����X�V����
	m_magicManager->Update(timer);
	// �G�t�F�N�g�}�l�[�W�����X�V����
	m_effectManager->Update(timer, m_targetCamera.get());

	// �v���C���[�ƃt�B�[���h�̓����蔻��
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		m_field->CollisionCheckPlayer(*(*itr));
	}

	// ���g�p�Ȃ��΂������𐶐�
	auto live_pred = LamdaUtils::NotNull();
	
	// �����蔻��
	// �v���C���[�ƃG�������g�̓����蔻��
	for (std::vector<Element*>::iterator element_itr = LamdaUtils::FindIf(*m_elementManager->GetElements(), live_pred),
		element_end = m_elementManager->GetElements()->end();
		element_itr != element_end;
		LamdaUtils::FindIfNext(element_itr, element_end, live_pred)) {
		const Collider* element_collider = (*element_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			if (Collision::HitCheck(element_collider, (*player_itr)->GetCollider())) {
				(*player_itr)->GetElement((*element_itr)->GetID());
				(*element_itr)->SetUsed(false);
			}
		}
	}
	

	// ���@���m�̓����蔻��
	for (std::vector<IMagic*>::iterator itr = LamdaUtils::FindIf(*m_magicManager->GetMagics(), live_pred),
		end = m_magicManager->GetMagics()->end();
		itr != end;) {
		const Collider* collider = (*itr)->GetCollider();
		std::vector<IMagic*>::iterator next = std::find_if(itr + 1, end, live_pred);
		for (std::vector<IMagic*>::iterator itr2 = next; itr2 != end; itr2 = std::find_if(itr2 + 1, end, live_pred)) {
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

	// �v���C���\�Ɩ��@�̓����蔻��
	for (std::vector<IMagic*>::iterator magic_itr = LamdaUtils::FindIf(*m_magicManager->GetMagics(), live_pred),
		magic_end = m_magicManager->GetMagics()->end();
		magic_itr != magic_end;
		LamdaUtils::FindIfNext(magic_itr, magic_end, live_pred)) {
		const Collider* magic_collider = (*magic_itr)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator player_itr = m_players.begin(); player_itr != m_players.end(); ++player_itr) {
			// ���g�̖��@�Ƃ͔��肵�Ȃ�
			if ((*player_itr)->GetPlayerID() == (*magic_itr)->GetPlayerID()) {
				continue;
			}
			if (Collision::HitCheck(magic_collider, (*player_itr)->GetCollider())) {
				(*magic_itr)->HitPlayer((*player_itr)->GetCollider());
				(*player_itr)->HitMagic(*magic_itr);
			}
		}
	}

	// �v���C���[���m�̓����蔻��
	for (std::vector<std::unique_ptr<Player>>::iterator itr1 = m_players.begin(); itr1 != m_players.end() - 1; ++itr1) {
		const Collider* collider1 = (*itr1)->GetCollider();
		for (std::vector<std::unique_ptr<Player>>::iterator itr2 = itr1 + 1; itr2 != m_players.end(); ++itr2) {
			if (Collision::HitCheck(collider1, (*itr2)->GetCollider())) {
				(*itr1)->HitPlayer(**itr2);
				(*itr2)->HitPlayer(**itr1);
			}
		}
	}

	// �t�B�[���h�̍X�V
	m_field->Update(timer);

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
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	//�r���[�s����擾����
	DirectX::SimpleMath::Matrix view = m_targetCamera->GetViewMatrix();
	// �ˉe�s����擾����
	DirectX::SimpleMath::Matrix projection = m_targetCamera->GetProjectionMatrix();

	//�O���b�h����`�悷��
	//m_gridFloor->Render(view, projection);
	// �t�B�[���h��`�悷��
	m_field->Render(view, projection);

	// �G�������g��`�悷��
	m_elementManager->Render(view, projection);
	// ���@��`�悷��
	m_magicManager->Render(view, projection);
	
	// �v���C���[��`�悷��
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, projection, spriteBatch);
	}

	// �G�t�F�N�g��`�悷��
	m_effectManager->Render(view, projection);

	spriteBatch->End();
}

/// <summary>
/// �v���C�V�[�����I������
/// </summary>
void PlayScene::Finalize() {
	// �}�E�X���΃��[�h�ɕύX����
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	// ���\�[�X���������
	ResourceLoader::Release(ResourceLoaderID::PlayScene);
	// �p�����[�^���J������
	m_parameterLoader->Dispose();
	ServiceLocater<PlayParameterLoader>::Unregister();
	// �G�t�F�N�g�}�l�[�W�����T�[�r�X���P�[�^�����������
	ServiceLocater<EffectManager>::Unregister();
}

