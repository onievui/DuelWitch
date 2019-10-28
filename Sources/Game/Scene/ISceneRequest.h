#pragma once
#ifndef ISECENE_REQUEST_DEFINED
#define ISECENE_REQUEST_DEFINED


/// <summary>
/// ���N�G�X�g�V�[���^�C�v
/// </summary>
enum class RequestSceneType {
	StackScene,		//���܂ł̃V�[���̏�ɒǉ�����
	ClearStack		//���܂ł̃V�[���������Ēǉ�����
};


/// <summary>
/// �V�[�����N�G�X�g�C���^�t�F�[�X�N���X
/// </summary>
class ISceneRequest {
public:
	// �V�[���̕ύX��v������
	virtual void RequestScene(const std::string& sceneName, RequestSceneType type = RequestSceneType::ClearStack) = 0;
	// �V�[�����|�b�v����
	virtual void PopScene(int num = 1) = 0;
};

#endif // !ISECENE_REQUEST_DEFINED


