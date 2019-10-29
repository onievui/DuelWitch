#pragma once
#ifndef FIELD_DEFINED
#define FIELD_DEFINED


#include <map>
#include <Framework\StepTimer.h>
#include <Game\Object\Transform.h>
#include "FieldData.h"


class Player;
class FieldShieldEffectEmitter;


/// <summary>
/// �t�B�[���h�N���X
/// </summary>
class Field {
public:
	// �R���X�g���N�^
	Field();
	// �f�X�g���N�^
	~Field();

public:
	// �t�B�[���h���X�V����
	void Update(const DX::StepTimer& timer);
	// �t�B�[���h��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// �t�B�[���h�̔��a���擾����
	float GetRadius() const { return m_radius; }
	// �v���C���[�ƃt�B�[���h�̓����蔻����s��
	void CollisionCheckPlayer(Player& player);

private:
	// �t�B�[���h�Փ˃G�t�F�N�g�𐶐�����
	void CreateEffect(const Player* pPlayer, const DirectX::SimpleMath::Vector3& pos);

private:
	// �t�B�[���h�̃T�C�Y�ύX�p�^�C�}�[
	float                                                                    m_time;
	// �p��
	Transform                                                                m_transform;
	// �t�B�[���h�O��
	std::unique_ptr<DirectX::GeometricPrimitive>                             m_wall;
	// �t�B�[���h���a
	float                                                                    m_radius;
	// �t�B�[���h���
	FieldData                                                                m_fieldData;

	// �t�B�[���h�Փ˃G�t�F�N�g
	FieldShieldEffectEmitter*                                                m_pEffect;
	// �G�t�F�N�g�𔭐������邩�ǂ����̔���p�^�C�}�[
	std::map<const Player*, float>                                           m_effectTimer;
};




#endif // !FIELD_DEFINED
