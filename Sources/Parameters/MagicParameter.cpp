#include "MagicParameter.h"
#include <Framework\DirectX11.h>
#include <Utils\JsonWrapper.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool MagicParameter::Load() {
	if (LoadData()) {
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		ResourceManager<GeometricPrimitiveResource>* resource_manager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
		resource_manager->AddResource(GeometricPrimitiveID::NormalMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateSphere(context, normalParam.radius*2.0f)));
		resource_manager->AddResource(GeometricPrimitiveID::FireMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateCone(context, fireParam.radius*2.0f, fireParam.height)));
		resource_manager->AddResource(GeometricPrimitiveID::FreezeMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateSphere(context, freezeParam.radius*2.0f)));
		resource_manager->AddResource(GeometricPrimitiveID::ThunderMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateSphere(context, thunderParam.radius*2.0f)));
		resource_manager->AddResource(GeometricPrimitiveID::ThunderStrikeMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateCylinder(context, thunderStrikeParam.height, thunderStrikeParam.radius*2.0f)));
		return true;
	}
	return false;
}

/// <summary>
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool MagicParameter::Reload() {
	if (LoadData()) {
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		ResourceManager<GeometricPrimitiveResource>* resource_manager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
		resource_manager->Replace(GeometricPrimitiveID::NormalMagic,
			DirectX::GeometricPrimitive::CreateSphere(context, normalParam.radius*2.0f));
		resource_manager->Replace(GeometricPrimitiveID::FireMagic,
			DirectX::GeometricPrimitive::CreateCone(context, fireParam.radius*2.0f, fireParam.height));
		resource_manager->Replace(GeometricPrimitiveID::FreezeMagic,
			DirectX::GeometricPrimitive::CreateSphere(context, freezeParam.radius*2.0f));
		resource_manager->Replace(GeometricPrimitiveID::ThunderMagic,
			DirectX::GeometricPrimitive::CreateSphere(context, thunderParam.radius*2.0f));
		resource_manager->Replace(GeometricPrimitiveID::ThunderStrikeMagic,
			DirectX::GeometricPrimitive::CreateCylinder(context, thunderStrikeParam.height, thunderStrikeParam.radius*2.0f));
		return true;
	}
	return false;
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool MagicParameter::Save() {
	// 未実装
	return false;
}

/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool MagicParameter::LoadData() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/magic.json")) {
		return false;
	}

	maxNum.resize(root["_MaxNum_Size"].getNumI());
	for (unsigned int i = 0; i < maxNum.size(); ++i) {
		maxNum[i] = root["MaxNum"][i].getNumI();
	}
	normalParam.radius            = root["Normal"]["Radius"].getNumF();
	normalParam.moveSpeed         = root["Normal"]["MoveSpeed"].getNumF();
	normalParam.lifeTime          = root["Normal"]["LifeTime"].getNumF();
	fireParam.wayAngle            = Math::Deg2Rad(root["Fire"]["WayAngle_Deg"].getNumF());
	fireParam.radius              = root["Fire"]["Radius"].getNumF();
	fireParam.height              = root["Fire"]["Height"].getNumF();
	fireParam.moveSpeed           = root["Fire"]["MoveSpeed"].getNumF();
	fireParam.lifeTime            = root["Fire"]["LifeTime"].getNumF();
	freezeParam.radius            = root["Freeze"]["Radius"].getNumF();
	freezeParam.lifeTime          = root["Freeze"]["LifeTime"].getNumF();
	thunderParam.radius           = root["Thunder"]["Radius"].getNumF();
	thunderParam.colliderRadius   = root["Thunder"]["ColliderRadius"].getNumF();
	thunderParam.moveSpeed        = root["Thunder"]["MoveSpeed"].getNumF();
	thunderParam.chaseSpeed       = root["Thunder"]["ChaseSpeed"].getNumF();
	thunderParam.chaseEndDistance = root["Thunder"]["ChaseEndDistance"].getNumF();
	thunderParam.chaseLerpSpeed   = root["Thunder"]["ChaseLerpSpeed"].getNumF();
	thunderParam.chaseEndTime     = root["Thunder"]["ChaseEndTime"].getNumF();
	thunderParam.lifeTime         = root["Thunder"]["LifeTime"].getNumF();
	thunderStrikeParam.radius     = root["ThunderStrike"]["Radius"].getNumF();
	thunderStrikeParam.height     = root["ThunderStrike"]["Height"].getNumF();
	thunderStrikeParam.appearPosY = root["ThunderStrike"]["AppearPosY"].getNumF();
	thunderStrikeParam.moveSpeed  = root["ThunderStrike"]["MoveSpeed"].getNumF();
	thunderStrikeParam.lifeTime   = root["ThunderStrike"]["LifeTime"].getNumF();

	return true;
}
