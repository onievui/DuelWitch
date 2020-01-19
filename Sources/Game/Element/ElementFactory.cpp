#include "ElementFactory.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\ElementParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "Element.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ElementFactory::ElementFactory() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ElementFactory::~ElementFactory() {
}

/// <summary>
/// �G�������g�t�@�N�g��������������
/// </summary>
void ElementFactory::Initialize() {
	m_elements.clear();
	m_elements.resize(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum);

	// �x�[�V�b�N�G�t�F�N�g�𐶐�����
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetTextureEnabled(true);
	// ���̓��C�A�E�g�𐶐�����
	void const* shader_byte_code;
	size_t byte_code_length;

	m_basicEffect->GetVertexShaderBytecode(&shader_byte_code, &byte_code_length);

	DX::ThrowIfFailed(
		device->CreateInputLayout(
			DirectX::GeometricPrimitive::VertexType::InputElements,
			DirectX::GeometricPrimitive::VertexType::InputElementCount,
			shader_byte_code, byte_code_length,
			m_inputLayout.GetAddressOf())
	);

	// �G�������g�𐶐�����
	for (std::vector<std::unique_ptr<Element>>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		(*itr) = std::make_unique<Element>(m_basicEffect.get(), m_inputLayout.Get());
	}
	

}

/// <summary>
/// �G�������g�̐�������
/// </summary>
/// <param name="id">�G�������gID</param>
/// <param name="position">���W</param>
/// <returns>
/// �G�������g
/// </returns>
Element* ElementFactory::Create(ElementID id, const DirectX::SimpleMath::Vector3& position) {
	// �g�p���Ă��Ȃ��I�u�W�F�N�g��T��
	std::vector<std::unique_ptr<Element>>::iterator itr = LamdaUtils::FindIfNot(m_elements, LamdaUtils::GetLamda(&Element::IsUsed));

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == m_elements.end()) {
		return nullptr;
	}
	
	(*itr)->Create(id, position);

	(*itr)->SetUsed(true);

	return itr->get();
}

/// <summary>
/// �����Ɉ�����ő�̃G�������g�����擾����
/// </summary>
/// <returns>
/// �ő吔
/// </returns>
int ElementFactory::GetMaxElementNum() {
	return ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum;
}

