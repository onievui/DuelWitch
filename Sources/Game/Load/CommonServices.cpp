#include "CommonServices.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\Resource.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\InputManager.h>
#include <Game\Scene\ShareData\ShareData.h>


/// <summary>
/// �T�[�r�X�ێ��N���X
/// </summary>
class CommonServices::Services {
public:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                         commonStates;
	// �L�[�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>       keyboardStateTracker;
	// �}�E�X���b�p�\
	std::unique_ptr<MouseWrapper>                                  mouseWrapper;
	// �p�b�h�g���b�J�[
	std::unique_ptr<DirectX::GamePad::ButtonStateTracker>          padStateTracker;
	// ���̓}�l�[�W��
	std::unique_ptr<InputManager>                                  inputManager;

	// �V�[���Ԃŋ��L����f�[�^
	std::unique_ptr<ShareData>                                     shareData;

	// ���\�[�X�}�l�[�W��
	std::unique_ptr<ResourceManager<TextureResource>>              textureResourceManager;
	std::unique_ptr<ResourceManager<GeometricPrimitiveResource>>   geometricPrimitiveResourceManager;
	std::unique_ptr<ResourceManager<ModelResource>>                modelResourceManager;
	std::unique_ptr<ResourceManager<SoundResource>>                soundResourceManager;
	std::unique_ptr<ResourceManager<BgmResource>>                  bgmResourceManager;
	std::unique_ptr<ResourceManager<FontResource>>                 fontResourceManager;
	std::unique_ptr<ResourceManager<VertexShaderResource>>         vertexShaderResourceManager;
	std::unique_ptr<ResourceManager<GeometryShaderResource>>       geometryShaderResourceManager;
	std::unique_ptr<ResourceManager<PixelShaderResource>>          pixelShaderResourceManager;

	// �I�[�f�B�I�}�l�[�W��
	std::unique_ptr<AudioManager>                                  audioManager;
};


/// <summary>
/// �R���X�g���N�^
/// </summary>
CommonServices::CommonServices() {
	m_services = std::make_unique<Services>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CommonServices::~CommonServices() {

}

/// <summary>
/// ���[�u�R���X�g���N�^
/// </summary>
/// <param name="from">���[�u��</param>
CommonServices::CommonServices(CommonServices&& from) = default;

/// <summary>
/// ���[�u������Z�q�̃I�[�o�[���[�h
/// </summary>
/// <param name="from">���[�u��</param>
/// <returns>
/// �����
/// </returns>
CommonServices& CommonServices::operator=(CommonServices&& from) = default;

/// <summary>
/// �R�����T�[�r�X������������
/// </summary>
void CommonServices::Initialize() {
	// �T�[�r�X�𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	RegisterService(m_services->commonStates, ServiceLocater<DirectX11>::Get()->GetDevice().Get());
	RegisterService(m_services->keyboardStateTracker);
	RegisterService(m_services->mouseWrapper, ServiceLocater<DirectX11>::Get()->GetHWnd());
	RegisterService(m_services->padStateTracker);
	RegisterService(m_services->inputManager);
	RegisterService(m_services->shareData);
	RegisterService(m_services->audioManager);

	// ���\�[�X�}�l�[�W���𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	RegisterResourceManager(m_services->textureResourceManager, L"�e�N�X�`��");
	RegisterResourceManager(m_services->geometricPrimitiveResourceManager, L"�W�I���g���b�N�v���~�e�B�u");
	RegisterResourceManager(m_services->modelResourceManager, L"���f��");
	RegisterResourceManager(m_services->soundResourceManager, L"�T�E���h");
	RegisterResourceManager(m_services->bgmResourceManager, L"BGM");
	RegisterResourceManager(m_services->fontResourceManager, L"�t�H���g");
	RegisterResourceManager(m_services->vertexShaderResourceManager, L"���_�V�F�[�_");
	RegisterResourceManager(m_services->geometryShaderResourceManager, L"�W�I���g���V�F�[�_");
	RegisterResourceManager(m_services->pixelShaderResourceManager, L"�s�N�Z���V�F�[�_");

}

/// <summary>
/// �I���������s��
/// </summary>
void CommonServices::Finalize() {
	ServiceLocater<DirectX::CommonStates>::Unregister();
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Unregister();
	ServiceLocater<MouseWrapper>::Unregister();
	ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Unregister();
	ServiceLocater<InputManager>::Unregister();
	ServiceLocater<ShareData>::Unregister();
	ServiceLocater<AudioManager>::Unregister();
	ServiceLocater<ResourceManager<TextureResource>>::Unregister();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Unregister();
	ServiceLocater<ResourceManager<ModelResource>>::Unregister();
	ServiceLocater<ResourceManager<SoundResource>>::Unregister();
	ServiceLocater<ResourceManager<BgmResource>>::Unregister();
	ServiceLocater<ResourceManager<FontResource>>::Unregister();
	ServiceLocater<ResourceManager<VertexShaderResource>>::Unregister();
	ServiceLocater<ResourceManager<GeometryShaderResource>>::Unregister();
	ServiceLocater<ResourceManager<PixelShaderResource>>::Unregister();
}

template<class T, class... Args>
/// <summary>
/// �T�[�r�X�𐶐����ăT�[�r�X���P�[�^�ɓo�^����
/// </summary>
/// <param name="service">�o�^����T�[�r�X</param>
/// <param name="...args">�R���X�g���N�^�̈���</param>
void CommonServices::RegisterService(std::unique_ptr<T>& service, Args&&... args) {
	service = std::make_unique<T>(args...);
	ServiceLocater<T>::Register(service.get());
}

template<class T>
/// <summary>
/// ���\�[�X�}�l�[�W���𐶐����ăT�[�r�X���P�[�^�ɓo�^����
/// </summary>
/// <param name="manager">���\�[�X�}�l�[�W��</param>
/// <param name="kind">���\�[�X�̎��</param>
void CommonServices::RegisterResourceManager(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind) {
	manager = std::make_unique<ResourceManager<T>>(kind);
	ServiceLocater<ResourceManager<T>>::Register(manager.get());
}

