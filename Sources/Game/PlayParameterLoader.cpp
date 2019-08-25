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
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayParameterLoader::~PlayParameterLoader() {
	LoadDataManager* manager = LoadDataManager::GetIns();
	manager->Unregister(&m_commandParameter);
	manager->Unregister(&m_aiCommandParameter);
	manager->Unregister(&m_elementParameter);
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

