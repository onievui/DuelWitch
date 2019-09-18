#include "CommonServices.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// �R�����T�[�r�X������������
/// </summary>
void CommonServices::Initialize() {
	// �T�[�r�X�𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	RegisterService(m_commonStates, ServiceLocater<DirectX11>::Get()->GetDevice().Get());
	RegisterService(m_keyboardStateTracker);
	RegisterService(m_mouseWrapper, ServiceLocater<DirectX11>::Get()->GetHWnd());
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
	ServiceLocater<DirectX::CommonStates>::Unregister();
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Unregister();
	ServiceLocater<MouseWrapper>::Unregister();
	ServiceLocater<ResourceManager<TextureResource>>::Unregister();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Unregister();
	ServiceLocater<ResourceManager<ModelResource>>::Unregister();
	ServiceLocater<ResourceManager<FontResource>>::Unregister();
}
