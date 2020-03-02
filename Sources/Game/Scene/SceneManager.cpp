#include "SceneManager.h"
#include <Utils\ErrorMessage.h>
#include <Utils\ServiceLocater.h>
#include "LogoScene.h"
#include "TitleScene.h"
#include "CharaSelectScene.h"
#include "CharaManualScene.h"
#include "PlayScene.h"
#include "PauseScene.h"
#include "WinResultScene.h"
#include "LoseResultScene.h"



/// <summary>
/// �V�[���̕ύX��v������
/// </summary>
/// <param name="sceneId">�V�[��ID</param>
/// <param name="type">���N�G�X�g�V�[���^�C�v</param>
void SceneManager::RequestScene(SceneID sceneId, RequestSceneType type) {
	m_requestQueue.push(std::make_pair(sceneId, type));
}

/// <summary>
/// �V�[�����|�b�v����
/// </summary>
/// <param name="num">�|�b�v����V�[���̐�</param>
void SceneManager::PopScene(int num) {
	while (!m_activeScene.empty() && num > 0) {
		m_activeScene.back()->Finalize();
		m_activeScene.pop_back();
		--num;
	}
	//// �L�[�{�[�h�ƃ}�E�X�����Z�b�g����
	//ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->Reset();
	//ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Get()->Reset();

}

/// <summary>
/// �V�[���}�l�[�W��������������
/// </summary>
void SceneManager::Initialize() {
	m_sceneCreateFunc.clear();
	m_sceneCreateFunc.emplace(SceneID::Logo,        MakeSceneCreateFunc<LogoScene>());
	m_sceneCreateFunc.emplace(SceneID::Title,       MakeSceneCreateFunc<TitleScene>());
	m_sceneCreateFunc.emplace(SceneID::CharaSelect, MakeSceneCreateFunc<CharaSelectScene>());
	m_sceneCreateFunc.emplace(SceneID::CharaManual, MakeSceneCreateFunc<CharaManualScene>());
	m_sceneCreateFunc.emplace(SceneID::Play,        MakeSceneCreateFunc<PlayScene>());
	m_sceneCreateFunc.emplace(SceneID::Pause,       MakeSceneCreateFunc<PauseScene>());
	m_sceneCreateFunc.emplace(SceneID::WinResult,   MakeSceneCreateFunc<WinResultScene>());
	m_sceneCreateFunc.emplace(SceneID::LoseResult,  MakeSceneCreateFunc<LoseResultScene>());
	std::queue<std::pair<SceneID, RequestSceneType>>().swap(m_requestQueue);
}

/// <summary>
/// �V�[���}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void SceneManager::Update(const DX::StepTimer& timer) {
	// �V�[���̕ύX�v��������ΕύX����
	while (!m_requestQueue.empty()) {
		ChangeScene();
	}

	// ��ԐV�����V�[�����X�V����
	m_activeScene.back()->Update(timer);
}

/// <summary>
/// �V�[����`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void SceneManager::Render(DirectX::SpriteBatch* spriteBatch) {
	// �L���ȃV�[�����Â����̂���`�悷��
	for (std::list<std::unique_ptr<IScene>>::iterator itr = m_activeScene.begin(); itr != m_activeScene.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
}

/// <summary>
/// �V�[����ύX����
/// </summary>
void SceneManager::ChangeScene() {
	std::pair<SceneID, RequestSceneType>& requestData = m_requestQueue.front();
	if (!m_sceneCreateFunc.count(requestData.first)) {
		ErrorMessage(L"�s���ȃV�[������ǂݍ������Ƃ��܂���");
		m_requestQueue.pop();
		return;
	}
	//// �L�[�{�[�h�ƃ}�E�X�����Z�b�g����
	//ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->Reset();
	//ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Get()->Reset();

	// �V�[����j������ꍇ
	if (requestData.second == RequestSceneType::ClearStack) {
		// �L���ȃV�[����������I������
		while (!m_activeScene.empty()) {
			m_activeScene.back()->Finalize();
			m_activeScene.pop_back();
		}
	}
	m_activeScene.push_back(m_sceneCreateFunc[requestData.first]());
	m_activeScene.back()->Initialize(static_cast<ISceneRequest*>(this));
	m_requestQueue.pop();
}
