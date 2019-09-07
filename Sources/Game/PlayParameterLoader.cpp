#include "PlayParameterLoader.h"
#include <Utils\LoadDataManager.h>
#include <Utils\LoadDataHolder.h>

#include <Parameters\CommandParameter.h>
#include <Parameters\AICommandParameter.h>
#include <Parameters\ElementParameter.h>
#include <Parameters\MagicParameter.h>


/// <summary>
/// �w�b�_�t�@�C���ł̃C���N���[�h�����炷���߂̃N���X
/// </summary>
class PlayParameterLoader::Impl {
public:
	// �R���X�g���N�^
	Impl();
	// �f�X�g���N�^
	~Impl();

public:
	// �ǂݍ��݃f�[�^
	LoadDataHolder<CommandParameter, LoadDataID::PlayScene>   commandParameter;
	LoadDataHolder<AICommandParameter, LoadDataID::PlayScene> aiCommandParameter;
	LoadDataHolder<ElementParameter, LoadDataID::PlayScene>   elementParameter;
	LoadDataHolder<MagicParameter, LoadDataID::PlayScene>     magicParameter;
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayParameterLoader::Impl::Impl() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Register(&commandParameter);
	manager->Register(&aiCommandParameter);
	manager->Register(&elementParameter);
	manager->Register(&magicParameter);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayParameterLoader::Impl::~Impl(){
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Unregister(&commandParameter);
	manager->Unregister(&aiCommandParameter);
	manager->Unregister(&elementParameter);
	manager->Unregister(&magicParameter);
}



/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayParameterLoader::PlayParameterLoader() {
	m_impl = std::make_unique<PlayParameterLoader::Impl>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayParameterLoader::~PlayParameterLoader() {
	m_impl.reset();
}

/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
void PlayParameterLoader::Load() {
	LoadDataManager::GetIns()->Load(LoadDataID::PlayScene);
}

/// <summary>
/// �p�����[�^���ēǂݍ��݂���
/// </summary>
void PlayParameterLoader::Reload() {
	LoadDataManager::GetIns()->Reload(LoadDataID::PlayScene);
}

/// <summary>
/// �p�����[�^���J������
/// </summary>
void PlayParameterLoader::Dispose() {
	LoadDataManager::GetIns()->Dispose(LoadDataID::PlayScene);
}

/// <summary>
/// �p�����[�^���擾����
/// </summary>
/// <returns>
/// �p�����[�^�ւ̃|�C���^
/// </returns>
const CommandParameter* PlayParameterLoader::GetCommandParameter() {
	return m_impl->commandParameter.Get();
}

/// <summary>
/// �p�����[�^���擾����
/// </summary>
/// <returns>
/// �p�����[�^�ւ̃|�C���^
/// </returns>
const AICommandParameter* PlayParameterLoader::GetAICommandParameter() {
	return m_impl->aiCommandParameter.Get();
}

/// <summary>
/// �p�����[�^���擾����
/// </summary>
/// <returns>
/// �p�����[�^�ւ̃|�C���^
/// </returns>
const ElementParameter* PlayParameterLoader::GetElementParameter() {
	return m_impl->elementParameter.Get();
}

/// <summary>
/// �p�����[�^���擾����
/// </summary>
/// <returns>
/// �p�����[�^�ւ̃|�C���^
/// </returns>
const MagicParameter* PlayParameterLoader::GetMagicParameter() {
	return m_impl->magicParameter.Get();
}

