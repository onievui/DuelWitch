#pragma once
#ifndef COMMON_SERVICES_DEFINED
#define COMMON_SERVICES_DEFINED


#include <Utils\ServiceLocater.h>


template <class T>
class ResourceManager;


/// <summary>
/// 基本的なサービスをまとめたクラス
/// </summary>
class CommonServices {
public:
	// コンストラクタ
	CommonServices();
	// デストラクタ
	~CommonServices();
	// ムーブコンストラクタ
	CommonServices(CommonServices&& from);
	// ムーブ代入演算子のオーバーロード
	CommonServices& operator=(CommonServices&& from);

public:
	// コモンサービスを初期化する
	void Initialize();
	// 終了処理を行う
	void Finalize();

private:
	template<class T, class... Args>
	// サービスを生成してサービスロケータに登録する
	void RegisterService(std::unique_ptr<T>& service, Args&&... args);

	template<class T>
	// リソースマネージャを生成してサービスロケータに登録する
	void RegisterResourceManager(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind);

private:
	// サービス保持クラス
	class Services;
	std::unique_ptr<Services> m_services;

};


#endif // !COMMON_SERVICES_DEFINED
