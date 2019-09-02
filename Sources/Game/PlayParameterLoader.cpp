#include "PlayParameterLoader.h"
#include <Utils\LoadDataManager.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayParameterLoader::PlayParameterLoader() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Register(&m_commandParameter);
	manager->Register(&m_aiCommandParameter);
	manager->Register(&m_elementParameter);
	manager->Register(&m_magicParameter);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayParameterLoader::~PlayParameterLoader() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Unregister(&m_commandParameter);
	manager->Unregister(&m_aiCommandParameter);
	manager->Unregister(&m_elementParameter);
	manager->Unregister(&m_magicParameter);
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
}

/// <summary>
/// �p�����[�^���J������
/// </summary>
void PlayParameterLoader::Dispose() {
	LoadDataManager::GetIns()->Dispose(LoadDataID::PlayScene);
}

