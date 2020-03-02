#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include <Utils\LamdaUtils.h>
#include <Utils\Resource.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Load\ResourceLoader.h>
#include "ISceneRequest.h"
#include <Game\Camera\DebugCamera.h>
#include <Game\Camera\TargetCamera.h>
#include <Game\Player\Player.h>
#include <Game\Player\PlayerID.h>
#include <Game\Player\PlayerManager.h>
#include <Game\Element\Element.h>
#include <Game\Element\ElementManager.h>
#include <Game\Magic\IMagic.h>
#include <Game\Magic\MagicManager.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Field\Field.h>
#include <Game\Field\GridFloor.h>
#include <Game\Collision\CollisionManager.h>
#include <Game\UI\Fade.h>
#include <Parameters\OptionParameter.h>


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

	// �}�E�X�𑊑΃��[�h�ɕύX����
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
	// �}�E�X���\���ɂ���
	ServiceLocater<MouseWrapper>::Get()->SetVisible(false);
	// �}�E�X���E�C���h�E���Ɏ��߂�
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(true);
	// �J�[�\������ʂ̒��S�Ɉړ�������
	ServiceLocater<MouseWrapper>::Get()->SetPos(DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f));

	m_isFinished = false;

	// ���\�[�X�����[�h����
	ResourceLoader::Load(ResourceLoaderID::PlayScene);

	// �p�����[�^�����[�h����
	m_parameterLoader = std::make_unique<PlayParameterLoader>();
	m_parameterLoader->Load();
	ServiceLocater<PlayParameterLoader>::Register(m_parameterLoader.get());
	
	// �}�E�X�̊��x��ݒ肷��
	ServiceLocater<MouseWrapper>::Get()->SetSensivity(m_parameterLoader->GetOptionParameter()->mouseParam.sensivity);


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
	
	//�f�o�b�O�J�����𐶐�����
	m_debugCamera = std::make_unique<DebugCamera>(directX->GetWidth(), directX->GetHeight());
	//�^�[�Q�b�g�J�����𐶐�����
	//DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f)
	m_targetCamera = std::make_unique<TargetCamera>(nullptr, DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
	//m_targetCamera = std::make_unique<TargetCamera>(nullptr, DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		PerspectiveFovInfo(Math::HarfPI*0.5f, static_cast<float>(directX->GetWidth()) / static_cast<float>(directX->GetHeight()), 0.1f, 5000.0f));

	ServiceLocater<TargetCamera>::Register(m_targetCamera.get());

	// �v���C���[�𐶐�����
	m_playerManager = std::make_unique<PlayerManager>();
	m_playerManager->Initialize(m_magicManager.get(), m_targetCamera.get());

	// �����蔻�菈���N���X�𐶐�����
	m_collisionManager = std::make_unique<CollisionManager>();

	//�O���b�h���𐶐�����
	m_gridFloor = std::make_unique<GridFloor>(ServiceLocater<DirectX::CommonStates>::Get(), 200.0f, 100);
	
	// �t�F�[�h�𐶐�����
	m_fade = std::make_unique<Fade>();
	m_fade->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);

	// BGM���Đ�����
	ServiceLocater<AudioManager>::Get()->PlayBgm(BgmID::Battle);
	// BGM���t�F�[�h�C��������
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, 0.0f, 1.0f);
}

/// <summary>
/// �v���C�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {

	// �t�F�[�h���X�V����
	m_fade->Update(timer);

	// �G�X�P�[�v�L�[�Ń|�[�Y��ʂ��Ăяo��
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		m_pSceneRequest->RequestScene(SceneID::Pause, RequestSceneType::StackScene);
	}

	// F2�L�[�Ńp�����[�^���ēǂݍ��݂���
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F2)) {
		m_parameterLoader->Reload();
		// �}�E�X�̊��x��ݒ肷��
		ServiceLocater<MouseWrapper>::Get()->SetSensivity(m_parameterLoader->GetOptionParameter()->mouseParam.sensivity);
	}

	// ���s�𔻒肷��
	if (!m_isFinished) {
		if (m_playerManager->Player1Win() || ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F3)) {
			m_nextSceneID = SceneID::WinResult;
			// ���������ꍇ�͔����t�F�[�h�A�E�g����
			m_fade->Initialize(Fade::State::FadeOut, 3.0f, 0.7f, static_cast<DirectX::SimpleMath::Color>(DirectX::Colors::White));
			m_isFinished = true;
		}
		else if (m_playerManager->Player1Lose() || ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F4)) {
			m_nextSceneID = SceneID::LoseResult;
			// �s�k�����ꍇ�͍����t�F�[�h�A�E�g����
			m_fade->Initialize(Fade::State::FadeOut, 3.0f, 0.8f, static_cast<DirectX::SimpleMath::Color>(DirectX::Colors::Black));
			m_isFinished = true;
		}
		if (m_isFinished) {
			// BGM���t�F�[�h�A�E�g������
			ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, 1.0f, 0.0f);
		}
	}
	else if(m_fade->IsFinished()) {
		// �t�F�[�h�A�E�g������������V�[���J�ڂ���
		m_pSceneRequest->RequestScene(m_nextSceneID, RequestSceneType::StackScene);
		// �}�E�X��\������
		ServiceLocater<MouseWrapper>::Get()->SetVisible(true);
		// �}�E�X���E�C���h�E���Ɉړ��ł���悤�ɂ���
		ServiceLocater<MouseWrapper>::Get()->ClipToWindow(false);
	}

	// �v���C���[�}�l�[�W�����X�V����
	m_playerManager->Update(timer);

	// �G�������g�}�l�[�W�����X�V����
	m_elementManager->SetRadius(m_field->GetRadius());
	m_elementManager->Update(timer);
	// ���@�}�l�[�W�����X�V����
	m_magicManager->Update(timer);
	// �G�t�F�N�g�}�l�[�W�����X�V����
	m_effectManager->Update(timer, m_targetCamera.get());

	// �����蔻����s��
	DetectCollision();

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
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

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
	m_playerManager->Render(view, projection, spriteBatch);

	// �G�t�F�N�g��`�悷��
	m_effectManager->Render(view, projection);

	// �V�[���̍ŏ��ƏI���Ńt�F�[�h�C���E�t�F�[�h�A�E�g����
	m_fade->Render(spriteBatch);

	spriteBatch->End();

	// �v���C���[1�̉�ʃG�t�F�N�g��`�悷��
	m_playerManager->RenderPlayer1Effect(spriteBatch);

}

/// <summary>
/// �v���C�V�[�����I������
/// </summary>
void PlayScene::Finalize() {
	//// �}�E�X���΃��[�h�ɕύX����
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	// �}�E�X��\������
	ServiceLocater<MouseWrapper>::Get()->SetVisible(true);
	// �}�E�X���E�C���h�E���Ɉړ��ł���悤�ɂ���
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(false);
	// ���\�[�X���������
	ResourceLoader::Release(ResourceLoaderID::PlayScene);
	// �p�����[�^���J������
	m_parameterLoader->Dispose();
	ServiceLocater<PlayParameterLoader>::Unregister();
	// �G�t�F�N�g�}�l�[�W�����T�[�r�X���P�[�^�����������
	ServiceLocater<EffectManager>::Unregister();
}

/// <summary>
/// �����蔻����s��
/// </summary>
void PlayScene::DetectCollision() {
	// �v���C���[�ƃt�B�[���h�̓����蔻����s��
	m_collisionManager->CollisionPlayerField(m_playerManager->GetPlayers(), m_field.get());
	
	// �v���C���[�ƃG�������g�̓����蔻����s��
	m_collisionManager->CollisionPlayerElement(m_playerManager->GetPlayers(), m_elementManager->GetElements());

	// ���@���m�̓����蔻����s��
	m_collisionManager->CollisionMagic(m_magicManager->GetMagics());

	// �v���C���[�Ɩ��@�̓����蔻����s��
	m_collisionManager->CollisionPlayerMagic(m_playerManager->GetPlayers(), m_magicManager->GetMagics());

	// �v���C���[���m�̓����蔻����s��
	m_collisionManager->CollisionPlayer(m_playerManager->GetPlayers());
	
}

