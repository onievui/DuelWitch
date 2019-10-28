#pragma once
#ifndef ELEMENT_MANAGER_DEFINED
#define ELEMENT_MANAGER_DEFINED


#include <Framework\StepTimer.h>


class Element;
class ElementFactory;


/// <summary>
/// �G�������g�}�l�[�W���N���X
/// </summary>
class ElementManager {
public:
	// �R���X�g���N�^
	ElementManager();
	// �f�X�g���N�^
	~ElementManager();

public:
	// �G�������g�}�l�[�W��������������
	void Initialize();
	// �G�������g�}�l�[�W�����X�V����
	void Update(const DX::StepTimer& timer);
	// �G�������g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// �G�������g�o���G���A�̔��a��ݒ肷��
	void SetRadius(float radius) { m_radius = radius; }
	// �G�������g���擾����
	std::vector<Element*>* GetElements();

private:
	// �G�������g�𐶐�����
	void CreateElement(float radius, int groupNum, int num);

private:
	// �����ς݃G�������g
	std::vector<Element*>           m_elements;
	// �G�������g�t�@�N�g��
	std::unique_ptr<ElementFactory> m_elementFactory;
	// �����p�^�C�}�[
	float                           m_creationTimer;
	// �G�������g�o���G���A���a
	float                           m_radius;
};


#endif // !ELEMENT_MANAGER_DEFINED
