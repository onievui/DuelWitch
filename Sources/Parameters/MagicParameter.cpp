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

	{
		JsonWrapper::value normal(root["Normal"]);
		normalParam.power     = normal["Power"].GetNumF();
		normalParam.radius    = normal["Radius"].GetNumF();
		normalParam.moveSpeed = normal["MoveSpeed"].GetNumF();
		normalParam.lifeTime  = normal["LifeTime"].GetNumF();
	}
	{
		JsonWrapper::value fire(root["Fire"]);
		fireParam.power    = fire["Power"].GetNumF();
		fireParam.wayAngle  = Math::Deg2Rad(fire["WayAngle_Deg"].GetNumF());
		fireParam.radius    = fire["Radius"].GetNumF();
		fireParam.height    = fire["Height"].GetNumF();
		fireParam.moveSpeed = fire["MoveSpeed"].GetNumF();
		fireParam.lifeTime  = fire["LifeTime"].GetNumF();
	}
	{
		JsonWrapper::value freeze(root["Freeze"]);
		freezeParam.power   = freeze["Power"].GetNumF();
		freezeParam.radius   = freeze["Radius"].GetNumF();
		freezeParam.lifeTime = freeze["LifeTime"].GetNumF();
	}
	{
		JsonWrapper::value thunder(root["Thunder"]);
		thunderParam.power            = thunder["Power"].GetNumF();
		thunderParam.radius           = thunder["Radius"].GetNumF();
		thunderParam.colliderRadius   = thunder["ColliderRadius"].GetNumF();
		thunderParam.moveSpeed        = thunder["MoveSpeed"].GetNumF();
		thunderParam.chaseSpeed       = thunder["ChaseSpeed"].GetNumF();
		thunderParam.chaseEndDistance = thunder["ChaseEndDistance"].GetNumF();
		thunderParam.chaseLerpSpeed   = thunder["ChaseLerpSpeed"].GetNumF();
		thunderParam.chaseEndTime     = thunder["ChaseEndTime"].GetNumF();
		thunderParam.lifeTime         = thunder["LifeTime"].GetNumF();
	}
	{
		JsonWrapper::value thunder_strike(root["ThunderStrike"]);
		thunderStrikeParam.power      = thunder_strike["Power"].GetNumF();
		thunderStrikeParam.radius     = thunder_strike["Radius"].GetNumF();
		thunderStrikeParam.height     = thunder_strike["Height"].GetNumF();
		thunderStrikeParam.appearPosY = thunder_strike["AppearPosY"].GetNumF();
		thunderStrikeParam.moveSpeed  = thunder_strike["MoveSpeed"].GetNumF();
		thunderStrikeParam.lifeTime   = thunder_strike["LifeTime"].GetNumF();
	}	

	return true;
}
