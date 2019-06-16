#pragma once
#ifndef SCENE_MANAGER_DEFINED
#define SCENE_MANAGER_DEFINED

#include <functional>
#include <unordered_map>
#include <queue>

#include <Framework\StepTimer.h>

#include "IScene.h"
#include "ISceneRequest.h"


/// <summary>
/// �V�[���}�l�[�W���N���X
/// </summary>
class SceneManager : public ISceneRequest {
public:
	// �V�[���̕ύX��v������
	void RequestScene(const std::string& sceneName, RequestSceneType type = RequestSceneType::ClearStack) override;
	// �V�[�����|�b�v����
	void PopScene(int num = 1) override;

public:
	// �V�[���}�l�[�W��������������
	virtual void Initialize();
	// �V�[���}�l�[�W�����X�V����
	virtual void Update(const DX::StepTimer& timer);
	// �V�[����`�悷��
	virtual void Render(DirectX::SpriteBatch* spriteBatch);

public:
	// �V�[����ύX����
	void ChangeScene();

private:
	// �V�[�������֐�
	using SceneCreateFunc = std::function<std::unique_ptr<IScene>()>;

	template<class Scene>
	/// <summary>
	/// �V�[�������֐����쐬����
	/// </summary>
	/// <returns>
	/// �V�[�������֐�
	/// </returns>
	SceneCreateFunc MakeSceneCreateFunc() {
		return []() {return std::make_unique<Scene>(); };
	}

private:
	// �V�[�������֐�
	std::unordered_map<std::string, SceneCreateFunc>     m_sceneCreateFunc;
	// �L���ȃV�[���̃��X�g
	std::list<std::unique_ptr<IScene>>                   m_activeScene;
	// �V�[���ύX�v���̃L���[
	std::queue<std::pair<std::string, RequestSceneType>> m_requestQueue;

};


#endif // !SCENE_MANAGER_DEFINED