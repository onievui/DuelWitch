#include "ElementParameter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool ElementParameter::Load() {
	if (LoadData()) {
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		ResourceManager<GeometricPrimitiveResource>* resource_manager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
		resource_manager->AddResource(GeometricPrimitiveID::Element, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateCube(context, radius*2.0f)));

		return true;
	}
	return false;
}

/// <summary>
/// �p�����[�^���ēǂݍ��݂���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool ElementParameter::Reload() {
	if (LoadData()) {
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		ResourceManager<GeometricPrimitiveResource>* resource_manager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
		resource_manager->Replace(GeometricPrimitiveID::Element, DirectX::GeometricPrimitive::CreateCube(context, radius*2.0f));

		return true;
	}
	return false;
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool ElementParameter::Save() {
	// ������
	return false;
}

/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool ElementParameter::LoadData() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/element.json")) {
		return false;
	}

	maxNum = root["MaxNum"].getNumI();
	alphaRate = root["AlphaRate"].getNumF();
	radius = root["Radius"].getNumF();

	return true;
}
