#include "ElementManager.h"
#include <unordered_map>
#include <Utils\ServiceLocater.h>
#include <Utils\RandMt.h>
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Parameters\ElementParameter.h>
#include "Element.h"
#include "ElementFactory.h"
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Field\FieldData.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
ElementManager::ElementManager()
	: m_elements()
	, m_elementFactory()
	, m_creationTimer()
	, m_center()
	, m_radius() {
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

	// �t�B�[���h���ɃG�������g��o�^����
	ServiceLocater<FieldData>::Get()->pElements = &m_elements;
}

/// <summary>
/// �G�������g�}�l�[�W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ElementManager::Update(const DX::StepTimer& timer) {
	m_creationTimer += static_cast<float>(timer.GetElapsedSeconds());

	// �t�B�[���h�����擾����
	const FieldData* fied_data = ServiceLocater<FieldData>::Get();

	// �G�������g���X�V����
	for (IfIterator<std::vector<Element*>> itr(m_elements, LamdaUtils::NotNull()); itr != m_elements.end(); ++itr) {
		(*itr)->Update(timer);
		// �t�B�[���h�Ƃ̈ʒu�֌W��ۂ�
		(*itr)->FitField(fied_data->fieldCenter, fied_data->fieldRadius);
		// ���g�p�̃G�������g��null�ɂ���
		if (!(*itr)->IsUsed()) {
			*itr = nullptr;
		}
	}

	// �G�������g�𐶐�����
	const float creation_interval = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->creationInterval;
	// �C���^�[�o���𒴂����琶������
	if (m_creationTimer >= creation_interval) {
		// 2������3����������
		CreateElement(m_radius, 2, 3);
		m_creationTimer -= creation_interval;
	}
}

/// <summary>
/// �G�������g��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ElementManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {

	// �L���ȃG�������g�̔z��𐶐�����
	std::list<Element*> valid_elements;
	// �|�C���^�Ɛ[�x�l�̃}�b�v�𐶐�����
	std::unordered_map<Element*, float> sort_keys;
	for (std::vector<Element*>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		if (*itr) {
			const DirectX::SimpleMath::Vector3& pos = (*itr)->GetPos();
			// ��r�p�[�x�l���v�Z����
			float key = view._13*pos.x + view._23*pos.y + view._33*pos.z;
			// �[�x�l�ƃ|�C���^��R�Â���
			sort_keys.emplace((*itr), key);
			// �L���ȃG�������g�̔z��ɒǉ�����
			valid_elements.push_back((*itr));
		}
	}

	// Z�\�[�g���s��
	valid_elements.sort([&](Element* left, Element* right) { return sort_keys[left] < sort_keys[right]; });

	// �L���ȃG�������g��`�悷��
	for (std::list<Element*>::const_iterator itr = valid_elements.cbegin(); itr != valid_elements.cend(); ++itr) {
		(*itr)->Render(view, proj);
	}
}

/// <summary>
/// �G�������g�𐶐�����
/// </summary>
/// <param name="radius">�t�B�[���h���a</param>
/// <param name="groupNum">�G�������g�O���[�v��</param>
/// <param name="num">�O���[�v���̌�</param>
void ElementManager::CreateElement(float radius, int groupNum, int num) {
	// �t�B�[���h�����擾����
	const FieldData* fied_data = ServiceLocater<FieldData>::Get();

	// �����_���ɕ��������߂�i��360���c+-30���j
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::UnitZ;
	dir = DirectX::SimpleMath::Vector3::Transform(dir,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitX, RandMt::GetRange(-Math::PI / 6, Math::PI / 6)));
	dir = DirectX::SimpleMath::Vector3::Transform(dir,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, RandMt::GetRand(Math::PI2)));
	//float angle = RandMt::GetRand(Math::PI2);
	//DirectX::SimpleMath::Vector3 dir(std::cosf(angle), RandMt::GetRange(-1.0f,1.0f)/std::sqrtf(3), std::sinf(angle));
	//dir.Normalize();

	// ��]���𐶐�����
	DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
	DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
		Math::CreateQuaternionFromVector3(xz_dir, dir));
	// ��]���쐬����
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, Math::PI2 / groupNum);
	
	// �G�������g�𐶐�����
	for (int i = 0; i < groupNum; ++i) {
		// �G�������g�������_���Ɍ��߂�
		int rand = RandMt::GetRand(static_cast<int>(ElementID::Num));
		for (int j = 0; j < num; j++) {
			// �o���ʒu�������_���Ō��߂�
			DirectX::SimpleMath::Vector3 pos = m_center + DirectX::SimpleMath::Vector3(
				dir.x*(radius-10.0f)+RandMt::GetRange(-3.0f,3.0f),
				dir.y*(radius-10.0f)+RandMt::GetRange(-2.0f,2.0f),
				dir.z*(radius-10.0f)+RandMt::GetRange(-3.0f,3.0f)
			);
			Element* created_element = m_elementFactory->Create(static_cast<ElementID>(rand), pos);
			// ���g�p�̃I�u�W�F�N�g��T��
			std::vector<Element*>::iterator itr = LamdaUtils::FindIf(m_elements, LamdaUtils::IsNull());
			if (itr != m_elements.end()) {
				*itr = created_element;
				// �t�B�[���h�[�Ƃ̋������L������
				(*itr)->SetBetweenFieldRadius(fied_data->fieldCenter, fied_data->fieldRadius);
			}
			// �G�������g��ύX����
			rand = (rand + 1) % static_cast<int>(ElementID::Num);
		}
		dir = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
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
