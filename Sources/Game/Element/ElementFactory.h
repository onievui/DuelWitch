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

	Num
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
	std::vector<std::unique_ptr<Element>>     m_elements;
	// �G�������g�p�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect>     m_basicEffect;
	// �G�������g�p���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

};


#endif // !ELEMENT_FACTORY_DEFINED
