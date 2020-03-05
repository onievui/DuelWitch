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
	// UI���X�V����
	void Update(const DX::StepTimer& timer) override;

public:
	// �}�E�X�N��������
	void OnMouseEnter() override;
	// �}�E�X�I�[�o�[������
	void OnMouseOver() override;
	// �N���b�N������
	void OnClick() override;

public:
	// UI��I������
	void Select();
	// UI��I����������
	void Deselect() { m_isSelected = false; }

private:
	// UI���I������Ă��邩�ǂ���
	bool                  m_isSelected;

};


#endif // !CHARA_ICON_DEFINED
