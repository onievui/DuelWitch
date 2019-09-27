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

	maxNum.resize(root["_MaxNum_Size"].GetNumI());
	for (unsigned int i = 0; i < maxNum.size(); ++i) {
		maxNum[i] = root["MaxNum"][i].GetNumI();
	}
	normalParam.radius            = root["Normal"]["Radius"].GetNumF();
	normalParam.moveSpeed         = root["Normal"]["MoveSpeed"].GetNumF();
	normalParam.lifeTime          = root["Normal"]["LifeTime"].GetNumF();
	fireParam.wayAngle            = Math::Deg2Rad(root["Fire"]["WayAngle_Deg"].GetNumF());
	fireParam.radius              = root["Fire"]["Radius"].GetNumF();
	fireParam.height              = root["Fire"]["Height"].GetNumF();
	fireParam.moveSpeed           = root["Fire"]["MoveSpeed"].GetNumF();
	fireParam.lifeTime            = root["Fire"]["LifeTime"].GetNumF();
	freezeParam.radius            = root["Freeze"]["Radius"].GetNumF();
	freezeParam.lifeTime          = root["Freeze"]["LifeTime"].GetNumF();
	thunderParam.radius           = root["Thunder"]["Radius"].GetNumF();
	thunderParam.colliderRadius   = root["Thunder"]["ColliderRadius"].GetNumF();
	thunderParam.moveSpeed        = root["Thunder"]["MoveSpeed"].GetNumF();
	thunderParam.chaseSpeed       = root["Thunder"]["ChaseSpeed"].GetNumF();
	thunderParam.chaseEndDistance = root["Thunder"]["ChaseEndDistance"].GetNumF();
	thunderParam.chaseLerpSpeed   = root["Thunder"]["ChaseLerpSpeed"].GetNumF();
	thunderParam.chaseEndTime     = root["Thunder"]["ChaseEndTime"].GetNumF();
	thunderParam.lifeTime         = root["Thunder"]["LifeTime"].GetNumF();
	thunderStrikeParam.radius     = root["ThunderStrike"]["Radius"].GetNumF();
	thunderStrikeParam.height     = root["ThunderStrike"]["Height"].GetNumF();
	thunderStrikeParam.appearPosY = root["ThunderStrike"]["AppearPosY"].GetNumF();
	thunderStrikeParam.moveSpeed  = root["ThunderStrike"]["MoveSpeed"].GetNumF();
	thunderStrikeParam.lifeTime   = root["ThunderStrike"]["LifeTime"].GetNumF();

	return true;
}
