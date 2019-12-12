#pragma once
#ifndef EFFECT_FACTORY_DEFINED
#define EFFECT_FACTORY_DEFINED


class IEffectEmitter;
class EffectManager;
enum class EffectID;


/// <summary>
/// �G�t�F�N�g�t�@�N�g���N���X
/// </summary>
class EffectFactory {
public:
	// �R���X�g���N�^
	EffectFactory();
	// �f�X�g���N�^
	~EffectFactory();

public:
	// �G�t�F�N�g�t�@�N�g��������������
	void Initialize(EffectManager* effectManager);
	// �G�t�F�N�g�𐶐����ĕԂ�
	IEffectEmitter* Create(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// �S�G�t�F�N�g�̍ő�o�������擾����
	int GetEffectMaxNum();

private:
	template <class T, class... Args>
	// �e�G�t�F�N�g������������
	void InitializeEffect(EffectID id, Args&&... args);

private:
	// �Ǘ�����G�t�F�N�g
	std::vector<std::unique_ptr<IEffectEmitter>> m_effects;
	// �G�t�F�N�g�̍ő�o����
	std::vector<int>                             m_maxNum;
	// �z��̃C���f�b�N�X�����ʒu
	std::vector<int>                             m_beginIndex;
};


#endif // !EFFECT_FACTORY_DEFINED

