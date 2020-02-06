#pragma once
#ifndef PLAY_PARAMETER_LOADER_DEFINED
#define PLAY_PARAMETER_LOADER_DEFINED


#include "IParameterLoader.h"


class CommandParameter;
class CharaStatusParameter;
class FieldParameter;
class ElementParameter;
class MagicParameter;
class EffectParameter;
class FuzzyParameter;
class OptionParameter;


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
	// �p�����[�^���擾����
	const CommandParameter*     GetCommandParameter();
	const CharaStatusParameter* GetCharaStatusParameter();
	const FieldParameter*       GetFieldParameter();
	const ElementParameter*     GetElementParameter();
	const MagicParameter*       GetMagicParameter();
	const EffectParameter*      GetEffectParameter();
	const FuzzyParameter*       GetFuzzyParameter();
	const OptionParameter*      GetOptionParameter();

private:
	// �w�b�_�t�@�C���ł̃C���N���[�h�����炷���߂̃N���X
	class Impl;
	std::unique_ptr<PlayParameterLoader::Impl> m_impl;

};

#endif // !PLAY_PARAMETER_LOADER_DEFINED
