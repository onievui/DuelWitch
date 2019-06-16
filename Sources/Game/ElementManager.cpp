#include "ElementManager.h"
#include <Framework\RandMt.h>
#include "Element.h"
#include "ElementFactory.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ElementManager::ElementManager()
	: m_elements()
	, m_elementFactory() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ElementManager::~ElementManager() {
}

/// <summary>
/// �G�������g�}�l�[�W��������������
/// </summary>
void ElementManager::Initialize() {
	m_elements.clear();
	m_elements.resize(ElementFactory::ELEMENT_MAX_NUM, nullptr);
	m_elementFactory = std::make_unique<ElementFactory>();
	m_elementFactory->Initialize();
}

/// <summary>
/// �G�������g�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ElementManager::Update(const DX::StepTimer& timer) {
	timer;
	for (auto& element : m_elements) {
		if (element) {
			element->Update(timer);
		}
	}
}

/// <summary>
/// �G�������g��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ElementManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (const auto& element : m_elements) {
		if (element) {
			element->Render(view, proj);
		}
	}
}

/// <summary>
/// �G�������g�𐶐�����
/// </summary>
/// <param name="areaStart">�����͈͂̎n�_</param>
/// <param name="areaEnd">�����͈͂̏I�_</param>
/// <param name="num">������</param>
void ElementManager::CreateElement(const DirectX::SimpleMath::Vector3& areaStart, const DirectX::SimpleMath::Vector3& areaEnd, const int num) {
	for (int i = 0; i < num; ++i) {
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(
			areaStart.x + (areaEnd.x - areaStart.x)*RandMt::GetRand(10001) / 10000.0f,
			areaStart.y + (areaEnd.y - areaStart.y)*RandMt::GetRand(10001) / 10000.0f,
			areaStart.z + (areaEnd.z - areaStart.z)*RandMt::GetRand(10001) / 10000.0f
		);
		Element* createdElement = m_elementFactory->Create(ElementFactory::ElementID::Fire, pos);
		for (auto& element : m_elements) {
			if (!element) {
				element = createdElement;
				break;
			}
		}
	}
}
