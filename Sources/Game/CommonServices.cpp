#include "CommonServices.h"
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>


/// <summary>
/// �R�����T�[�r�X������������
/// </summary>
void CommonServices::Initialize() {
	// �T�[�r�X�𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	RegisterService(m_keyboardStateTracker);
	RegisterService(m_mouseStateTracker);
	// ���\�[�X�}�l�[�W���𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	RegisterResourceManager(m_textureResourceManager, L"�e�N�X�`��");
	RegisterResourceManager(m_geometricPrimitiveResourceManager, L"�W�I���g���b�N�v���~�e�B�u");
	RegisterResourceManager(m_modelResourceManager, L"���f��");
	RegisterResourceManager(m_fontResourceManager, L"�t�H���g");

}

/// <summary>
/// �I���������s��
/// </summary>
void CommonServices::Finalize() {
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Remove();
	ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Remove();
	ServiceLocater<ResourceManager<TextureResource>>::Remove();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Remove();
	ServiceLocater<ResourceManager<ModelResource>>::Remove();
	ServiceLocater<ResourceManager<FontResource>>::Remove();
}
