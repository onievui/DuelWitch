#pragma once
#ifndef MENU_UI_DEFINED
#define MENU_UI_DEFINED


#include <Utils\ScaleUpUI.h>
enum class SoundID;


/// <summary>
/// ���j���[�pUI
/// </summary>
class MenuUI : public ScaleUpUI {
public:
	// �R���X�g���N�^
	MenuUI(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);
		
public:
	// UI���X�V����
	void Update(const DX::StepTimer& timer) override;

protected:
	// �}�E�X�N��������
	void OnMouseEnter() override;
	// �N���b�N������
	void OnClick() override;

public:
	// UI��I������
	void Select();
	// UI��I����������
	void Deselect()                       { m_isSelected = false; }
	// �N���b�N���������Ăяo��
	void Decide()                         { OnClick(); }
	// �}�E�X�N�����̌��ʉ���ݒ肷��
	void SetOnMouseEnterSound(SoundID id) { m_onMouseEnterSoundId = id; }
	// �N���b�N���̌��ʉ���ݒ肷��
	void SetOnClickSound(SoundID id)      { m_onClickSoundId = id; }

protected:
	// UI���I������Ă��邩�ǂ���
	bool                  m_isSelected;
	// �}�E�X�N�����̌��ʉ�
	SoundID               m_onMouseEnterSoundId;
	// �N���b�N���̌��ʉ�
	SoundID               m_onClickSoundId;
};

#endif // !MENU_UI_DEFINED
