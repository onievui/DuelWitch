#include "CharaManualScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\Fade.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
CharaManualScene::CharaManualScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CharaManualScene::~CharaManualScene() {
}

/// <summary>
///	�L�����}�j���A���V�[��������������
/// </summary>
/// <param name="pSceneRequest">���N�G�X�g�V�[���C���^�t�F�[�X</param>
void CharaManualScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// �t�F�[�h�𐶐�����
	m_fade = std::make_unique<Fade>();
	m_fade->Initialize(Fade::State::FadeOut, CHARA_MANUAL_TIME, 0.8f);
	m_fadeTexture = std::make_unique<Fade>();
	m_fadeTexture->Initialize(Fade::State::FadeInAlpha, CHARA_MANUAL_TIME, 1.0f);

	m_canceled = false;

	// �|�[�Y����炷
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Pause);

}

/// <summary>
/// �L�����}�j���A���V�[�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CharaManualScene::Update(const DX::StepTimer& timer) {

	m_fade->Update(timer);
	m_fadeTexture->Update(timer);

	// �t�F�[�h����������܂ŏ������Ȃ�
	if (!m_fade->IsFinished()) {
		return;
	}

	// �L�����Z�������Ȃ�V�[����߂�
	if (m_canceled) {
		m_pSceneRequest->PopScene();
	}

	// �N���b�N���G�X�P�[�v�L�[�������āA�L�����Z������
	DirectX::Mouse::ButtonStateTracker* mouse_tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();
	const bool left_click = (mouse_tracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	const bool right_click = (mouse_tracker->rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (left_click || right_click || press_escape) {
		m_fade->Initialize(Fade::State::FadeIn, CHARA_MANUAL_TIME*1.25f, 0.0f);
		m_fade->SkipTime(0.2f);
		m_fadeTexture->Initialize(Fade::State::FadeOutAlpha, CHARA_MANUAL_TIME, 0.0f);
		m_canceled = true;
	}

}

/// <summary>
/// �L�����}�j���A���V�[����`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void CharaManualScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// �v���C��ʂɍ����d�˂ĈÂ�����
	m_fade->Render(spriteBatch);

	// �L�����}�j���A����`�悷��
	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaManual);
	DirectX::SimpleMath::Color color = DirectX::Colors::White;
	color.w = m_fadeTexture->GetAlpha();
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, color, 0.0f,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.25f, 1.25f));

	spriteBatch->End();
}

/// <summary>
/// �L�����}�j���A���V�[�����I������
/// </summary>
void CharaManualScene::Finalize() {
	
}

