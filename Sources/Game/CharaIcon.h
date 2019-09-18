#pragma once
#ifndef CHARA_ICON_DEFINED
#define CHARA_ICON_DEFINED


#include <Utils\UISubject.h>


/// <summary>
/// �L�����A�C�R��
/// </summary>
class CharaIcon : public UISubject {
public:
	// �R���X�g���N�^
	CharaIcon(int layer,
		const DirectX::SimpleMath::Vector2& pos,
		const TextureResource* pTexture,
		int textureIndex = 0);

public:
	// �}�E�X�I�[�o�[������
	void OnMouseOver() override;
};


#endif // !CHARA_ICON_DEFINED
