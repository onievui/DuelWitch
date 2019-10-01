#include "ElementManager.h"
#include <Utils\ServiceLocater.h>
#include <Utils\RandMt.h>
#include <Utils\MathUtils.h>
#include <Parameters\ElementParameter.h>
#include "Element.h"
#include "ElementFactory.h"
#include "PlayParameterLoader.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ElementManager::ElementManager()
	: m_elements()
	, m_elementFactory()
	, m_creationTimer() {
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
	m_elements.resize(ElementFactory::GetMaxElementNum(), nullptr);
	m_elementFactory = std::make_unique<ElementFactory>();
	m_elementFactory->Initialize();
	m_creationTimer = 0.0f;
}

/// <summary>
/// �G�������g�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ElementManager::Update(const DX::StepTimer& timer) {
	m_creationTimer += static_cast<float>(timer.GetElapsedSeconds());

	for (std::vector<Element*>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		if (*itr) {
			(*itr)->Update(timer);
			if (!(*itr)->IsUsed()) {
				*itr = nullptr;
			}
		}
	}

	// �G�������g�𐶐�����
	const float creation_interval = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->creationInterval;
	if (m_creationTimer >= creation_interval) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, -60);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3(-3, -3, -2);
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3(3, 3, 2);
		CreateElement(area_start + area_offset, area_end + area_offset, 3);
		area_offset.z = 60.0f;
		CreateElement(area_start + area_offset, area_end + area_offset, 3);
		m_creationTimer -= creation_interval;
	}
}

/// <summary>
/// �G�������g��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ElementManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<Element*>::const_iterator itr = m_elements.cbegin(); itr != m_elements.cend(); ++itr) {
		if (*itr) {
			(*itr)->Render(view, proj);
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
	int rand = RandMt::GetRand(3);
	for (int i = 0; i < num; ++i) {
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(
			Math::Lerp(areaStart.x, areaEnd.x, RandMt::GetRand(1.0f)),
			Math::Lerp(areaStart.y, areaEnd.y, RandMt::GetRand(1.0f)),
			Math::Lerp(areaStart.z, areaEnd.z, RandMt::GetRand(1.0f))
		);
		Element* created_element = m_elementFactory->Create(ElementID(rand), pos);
		// ���g�p�̃I�u�W�F�N�g��T��
		std::vector<Element*>::iterator itr = std::find_if(m_elements.begin(), m_elements.end(), [&](Element* element) {return !element; });
		if (itr != m_elements.end()) {
			*itr = created_element;
		}
		//for (std::vector<Element*>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		//	if (!*itr) {
		//		*itr = created_element;
		//		break;
		//	}
		//}
		rand = (rand + 1) % 3;
	}
}

/// <summary>
/// �G�������g���擾����
/// </summary>
/// <returns>
/// �G�������g�̔z��
/// </returns>
std::vector<Element*>* ElementManager::GetElements() {
	return &m_elements;
}
