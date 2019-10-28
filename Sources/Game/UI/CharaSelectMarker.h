#pragma once
#ifndef CHARA_SELECT_MARKER_DEFINED
#define CHARA_SELECT_MARKER_DEFINED


#include <Utils\UIObserver.h>
#include <Utils\UISubject.h>


/// <summary>
/// �L�����Z���N�g�p�}�[�J�[
/// </summary>
class CharaSelectMarker : public UIObserver, public UISubject {
public:
	// �R���X�g���N�^
	CharaSelectMarker(int layer,
		const DirectX::SimpleMath::Vector2& pos,
		const TextureResource* pTexture,
		int textureIndex = 0);

public:
	// �I�u�U�[�o���X�V����
	void Update(ISubject* subject) override;

public:
	// �}�[�J�[���X�V����
	void Update(const DX::StepTimer& timer) override { timer; }
};


#endif // !CHARA_SELECT_MARKER_DEFINED
