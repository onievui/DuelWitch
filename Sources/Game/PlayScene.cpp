#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include "ISceneRequest.h"


/// <summary>
///	�v���C�V�[��������������
/// </summary>
/// <param name="sceneRequest"></param>
void PlayScene::Initialize(ISceneRequest* sceneRequest) {
	m_sceneRequest = sceneRequest;
	// �X�v���C�g�t�H���g�𐶐�����
	m_font = std::make_unique<DirectX::SpriteFont>(DirectX11::Get().GetDevice().Get(), L"myfile.spritefont");
}

/// <summary>
/// �v���C�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	timer;
	
}

/// <summary>
/// �v���C�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void PlayScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	
	spriteBatch->End();
}

/// <summary>
/// �v���C�V�[�����I������
/// </summary>
void PlayScene::Finalize() {

}

