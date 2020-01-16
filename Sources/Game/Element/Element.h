#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED


#include <Framework\StepTimer.h>
#include <Utils\MathUtils.h>
#include <Utils\LoadDataHolder.h>
#include <Game\Object\IObject.h>
#include <Game\Object\Transform.h>
#include <Game\Collision\SphereCollider.h>


enum class ElementID;


/// <summary>
/// �G�������g�N���X
/// </summary>
class Element : public IObject {
public:
	// �����蔻��̔��a
	static constexpr float ELEMENT_RADIUS = 0.75f;
	// ��]���x
	static constexpr float ELEMENT_ROTATE_SPEED = Math::HarfPI;

public:
	Element();
	~Element();

public:
	// �G�������g���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �G�������g���J������
	void Lost() override;
	// �G�������g�𐶐�����
	void Create(ElementID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color);
	// �G�������g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �G�������g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override { return m_world; }
	// �G�������g�̓����蔻����擾����
	const Collider* GetCollider() const override                  { return &m_sphereCollider; }
	// �G�������g�̍��W���擾����
	const DirectX::SimpleMath::Vector3& GetPos() const            { return m_transform.GetLocalPosition(); }
	// �G�������g��ID���擾����
	ElementID GetID() const                                       { return m_id; }
	// �G�������g���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const                                           { return m_isUsed; }
	// �G�������g���g�p���邩�ǂ����ݒ肷��
	void SetUsed(bool isUsed)                                     { m_isUsed = isUsed; }
	// �t�B�[���h�[�Ƃ̋������L������
	void SetBetweenFieldRadius(const DirectX::SimpleMath::Vector3& center, float radius);
	// �t�B�[���h�Ƃ̋�����ۂ�
	void FitField(const DirectX::SimpleMath::Vector3& center, float radius);

private:
	// �G�������gID
	ElementID                                    m_id;
	// �p��
	Transform                                    m_transform;
	// �������蔻��
	SphereCollider                               m_sphereCollider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix                  m_world;
	// �F
	DirectX::SimpleMath::Color                   m_color;
	// �g�p���Ă��邩�ǂ���
	bool                                         m_isUsed;
	// ��������Ă���̎���
	float                                        m_time;
	// �������ꂽ�Ƃ��́A�t�B�[���h�[�܂ł̋���
	float                                        m_betweenFieldEnd;

};


#endif // !ELEMENT_DEFINED


