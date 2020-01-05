#pragma once
#ifndef SOUND_SCALE_UP_UI_DEFINED
#define SOUND_SCALE_UP_UI_DEFINED


#include <Utils\ScaleUpUI.h>
enum class SoundID;


/// <summary>
/// ScaleUpUI�̉��t����
/// </summary>
class SoundScaleUpUI : public ScaleUpUI {
public:
	// �R���X�g���N�^
	SoundScaleUpUI(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);
		

protected:
	// �}�E�X�N��������
	void OnMouseEnter() override;
	// �N���b�N������
	void OnClick() override;

public:
	// �}�E�X�N�����̌��ʉ���ݒ肷��
	void SetOnMouseEnterSound(SoundID id) { m_onMouseEnterSoundId = id; }
	// �N���b�N���̌��ʉ���ݒ肷��
	void SetOnClickSound(SoundID id)      { m_onClickSoundId = id; }

protected:
	// �}�E�X�N�����̌��ʉ�
	SoundID               m_onMouseEnterSoundId;
	// �N���b�N���̌��ʉ�
	SoundID               m_onClickSoundId;
};

#endif // !SOUND_SCALE_UP_UI_DEFINED
