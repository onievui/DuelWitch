#pragma once
#ifndef ISCENE_DEFINED
#define ISCENE_DEFINED

#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// �V�[���C���^�t�F�[�X�N���X
/// </summary>
class IScene {
public:
	// �V�[��������������
	virtual void Initialize(ISceneRequest* pSceneRequest) = 0;
	// �V�[�����X�V����
	virtual void Update(const DX::StepTimer& timer) = 0;
	// �V�[����`�悷��
	virtual void Render(DirectX::SpriteBatch* spriteBatch) = 0;
	// �V�[�����I������
	virtual void Finalize() = 0;

};

#endif // !ISCENE_DEFINED


