#pragma once
#ifndef ELEMENT_FACTORY_DEFINED
#define ELEMENT_FACTORY_DEFINED


#include <Utils\LoadDataHolder.h>


class Element;


// �G�������g��ID
enum class ElementID {
	Fire,       // ���G�������g
	Thunder,	// ���G�������g
	Freeze,     // �X�G�������g
};


/// <summary>
/// �G�������g�t�@�N�g���N���X
/// </summary>
class ElementFactory {
public:
	// �R���X�g���N�^
	ElementFactory();
	// �f�X�g���N�^
	~ElementFactory();

public:
	// �G�������g�t�@�N�g��������������
	void Initialize();
	// �G�������g�𐶐�����
	Element* Create(ElementID id, const DirectX::SimpleMath::Vector3& position);

public:
	// �����Ɉ�����ő�̃G�������g�����擾����
	static int GetMaxElementNum();

private:
	// �Ǘ�����G�������g
	std::vector<std::unique_ptr<Element>> m_elements;

};


#endif // !ELEMENT_FACTORY_DEFINED
