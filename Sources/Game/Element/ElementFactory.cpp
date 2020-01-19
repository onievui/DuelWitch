#include "ElementFactory.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\ElementParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "Element.h"


/// <summary>
/// コンストラクタ
/// </summary>
ElementFactory::ElementFactory() {
}

/// <summary>
/// デストラクタ
/// </summary>
ElementFactory::~ElementFactory() {
}

/// <summary>
/// エレメントファクトリを初期化する
/// </summary>
void ElementFactory::Initialize() {
	m_elements.clear();
	m_elements.resize(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum);

	// ベーシックエフェクトを生成する
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
	m_basicEffect->SetTextureEnabled(true);
	// 入力レイアウトを生成する
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

	// エレメントを生成する
	for (std::vector<std::unique_ptr<Element>>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		(*itr) = std::make_unique<Element>(m_basicEffect.get(), m_inputLayout.Get());
	}
	

}

/// <summary>
/// エレメントの生成処理
/// </summary>
/// <param name="id">エレメントID</param>
/// <param name="position">座標</param>
/// <returns>
/// エレメント
/// </returns>
Element* ElementFactory::Create(ElementID id, const DirectX::SimpleMath::Vector3& position) {
	// 使用していないオブジェクトを探す
	std::vector<std::unique_ptr<Element>>::iterator itr = LamdaUtils::FindIfNot(m_elements, LamdaUtils::GetLamda(&Element::IsUsed));

	// これ以上生成できないならnullptrを返す
	if (itr == m_elements.end()) {
		return nullptr;
	}
	
	(*itr)->Create(id, position);

	(*itr)->SetUsed(true);

	return itr->get();
}

/// <summary>
/// 同時に扱える最大のエレメント数を取得する
/// </summary>
/// <returns>
/// 最大数
/// </returns>
int ElementFactory::GetMaxElementNum() {
	return ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum;
}

