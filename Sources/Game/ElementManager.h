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
	// �G�������g�𐶐�����
	void CreateElement(const DirectX::SimpleMath::Vector3& areaStart, const DirectX::SimpleMath::Vector3& areaEnd, const int num);
	// �G�������g���擾����
	std::vector<Element*>* GetElements();

private:
	// �����ς݃G�������g
	std::vector<Element*> m_elements;
	// �G�������g�t�@�N�g��
	std::unique_ptr<ElementFactory> m_elementFactory;

};


#endif // !ELEMENT_MANAGER_DEFINED
