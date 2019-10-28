#include "CharaIcon.h"
#include <Utils\Resource.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="layer">���C���[</param>
/// <param name="pos">���W</param>
/// <param name="pTexture">�e�N�X�`���ւ̃|�C���^</param>
/// <param name="textureIndex">�e�N�X�`���̃C���f�b�N�X</param>
CharaIcon::CharaIcon(int layer, const DirectX::SimpleMath::Vector2& pos, const TextureResource* pTexture, int textureIndex)
	: UISubject(UIEventID::CharaIcon, layer, pos, DirectX::SimpleMath::Vector2(100, 100), pTexture, textureIndex) {
	// �e�N�X�`���̃T�C�Y��ǂݍ���
	if (pTexture) {
		m_size = pTexture->GetSize();
	}
}

/// <summary>
/// �}�E�X�I�[�o�[������
/// </summary>
void CharaIcon::OnMouseOver() {
	// �ꎞ�I�ɃC�x���gID���}�E�X�I�[�o�[�ɕς��Ă���ʒm����
	UIEventID tmpID = m_eventID;
	m_eventID = UIEventID::MouseOver;
	Notify();
	m_eventID = tmpID;
}
