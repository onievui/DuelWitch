#include "SoundScaleUpUI.h"
#include <Utils\ServiceLocater.h>
#include <Utils\AudioManager.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="eventID">�C�x���gID</param>
/// <param name="layer">���C���[</param>
/// <param name="pos">���W</param>
/// <param name="size">�T�C�Y</param>
/// <param name="pTexture">�e�N�X�`���ւ̃|�C���^</param>
/// <param name="textureIndex">�e�N�X�`���̃C���f�b�N�X</param>
SoundScaleUpUI::SoundScaleUpUI(UIEventID eventID, int layer,
	const DirectX::SimpleMath::Vector2& pos, const DirectX::SimpleMath::Vector2& size,
	const TextureResource* pTexture, int textureIndex)
	: ScaleUpUI(eventID, layer, pos, size, pTexture, textureIndex)
	, m_onMouseEnterSoundId(SoundID::Cursor)
	, m_onClickSoundId(SoundID::Decision) {
}


/// <summary>
/// �}�E�X�N��������
/// </summary>
void SoundScaleUpUI::OnMouseEnter() {
	// ���ʉ���炷
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onMouseEnterSoundId);
}

/// <summary>
/// �N���b�N������
/// </summary>
void SoundScaleUpUI::OnClick() {
	ScaleUpUI::OnClick();
	// ���ʉ���炷������ǉ�����
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onClickSoundId);
}
