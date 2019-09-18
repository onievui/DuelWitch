#pragma once
#ifndef COMMON_SERVICES_DEFINED
#define COMMON_SERVICES_DEFINED


#include <Utils\ServiceLocater.h>


class MouseWrapper;
template <class T>
class ResourceManager;
class TextureResource;
class GeometricPrimitiveResource;
class ModelResource;
class FontResource;


/// <summary>
/// 基本的なサービスをまとめたクラス
/// </summary>
class CommonServices {
public:
	// コモンサービスを初期化する
	void Initialize();
	// 終了処理を行う
	void Finalize();

private:
	template<class T, class... Args>
	// サービスを生成してサービスロケータに登録する
	void RegisterService(std::unique_ptr<T>& service, Args&&... args) {
		service = std::make_unique<T>(args...);
		ServiceLocater<T>::Register(service.get());
	}

	template<class T>
	// リソースマネージャを生成してサービスロケータに登録する
	void RegisterResourceManager(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind) {
		manager = std::make_unique<ResourceManager<T>>(kind);
		ServiceLocater<ResourceManager<T>>::Register(manager.get());
	};

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                         m_commonStates;
	// キートラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>       m_keyboardStateTracker;
	// マウスラッパ―
	std::unique_ptr<MouseWrapper>                                  m_mouseWrapper;

	// リソースマネージャ
	std::unique_ptr<ResourceManager<TextureResource>>              m_textureResourceManager;
	std::unique_ptr<ResourceManager<GeometricPrimitiveResource>>   m_geometricPrimitiveResourceManager;
	std::unique_ptr<ResourceManager<ModelResource>>                m_modelResourceManager;
	std::unique_ptr<ResourceManager<FontResource>>                 m_fontResourceManager;
};


#endif // !COMMON_SERVICES_DEFINED
