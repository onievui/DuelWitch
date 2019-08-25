#pragma once
#ifndef PLAY_PARAMETER_LOADER_DEFINED
#define PLAY_PARAMETER_LOADER_DEFINED


#include "IParameterLoader.h"
#include <Utils\LoadDataHolder.h>

#include <Parameters\CommandParameter.h>
#include <Parameters\AICommandParameter.h>
#include <Parameters\ElementParameter.h>


/// <summary>
/// �v���C�V�[���p�p�����[�^�ǂݍ��݃N���X
/// </summary>
class PlayParameterLoader : public IParameterLoader {
public:
	// �R���X�g���N�^
	PlayParameterLoader();
	// �f�X�g���N�^
	~PlayParameterLoader();

public:
	// �p�����[�^��ǂݍ���
	void Load() override;
	// �p�����[�^���ēǂݍ��݂���
	void Reload() override;
	// �p�����[�^���J������
	void Dispose() override;

public:
	const CommandParameter*   GetCommandParameter()   { return m_commandParameter.Get(); }
	const AICommandParameter* GetAICommandParameter() { return m_aiCommandParameter.Get(); }
	const ElementParameter*   GetElementParameter()   { return m_elementParameter.Get(); }

private:
	LoadDataHolder<CommandParameter, LoadDataID::PlayScene>   m_commandParameter;
	LoadDataHolder<AICommandParameter, LoadDataID::PlayScene> m_aiCommandParameter;
	LoadDataHolder<ElementParameter, LoadDataID::PlayScene>   m_elementParameter;

};

#endif // !PLAY_PARAMETER_LOADER_DEFINED
