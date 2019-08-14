#include "SceneManager.h"
#include "LogoScene.h"
#include "PlayScene.h"


/// <summary>
/// �V�[���̕ύX��v������
/// </summary>
/// <param name="sceneName">�V�[����</param>
/// <param name="type">���N�G�X�g�V�[���^�C�v</param>
void SceneManager::RequestScene(const std::string& sceneName, RequestSceneType type) {
	m_requestQueue.push(std::make_pair(sceneName, type));
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
	// �L�[�{�[�h�����Z�b�g����
	DirectX::Keyboard::Get().Reset();
}

/// <summary>
/// �V�[���}�l�[�W��������������
/// </summary>
void SceneManager::Initialize() {
	m_sceneCreateFunc.clear();
	m_sceneCreateFunc.emplace("Logo",   MakeSceneCreateFunc<LogoScene>());
	//m_sceneCreateFunc.emplace("Title",  MakeSceneCreateFunc<TitleScene>());
	m_sceneCreateFunc.emplace("Play",   MakeSceneCreateFunc<PlayScene>());
	//m_sceneCreateFunc.emplace("Result", MakeSceneCreateFunc<ResultScene>());
	std::queue<std::pair<std::string, RequestSceneType>>().swap(m_requestQueue);
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
/// <param name="spriteBatch"></param>
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
	std::pair<std::string, RequestSceneType>& requestData = m_requestQueue.front();
	if (!m_sceneCreateFunc.count(requestData.first)) {
		m_requestQueue.pop();
		return;
	}
	// �L�[�{�[�h�����Z�b�g����
	DirectX::Keyboard::Get().Reset();
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
