#include "CommonServices.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// コモンサービスを初期化する
/// </summary>
void CommonServices::Initialize() {
	// サービスを生成してサービスロケータに登録する
	RegisterService(m_commonStates, ServiceLocater<DirectX11>::Get()->GetDevice().Get());
	RegisterService(m_keyboardStateTracker);
	RegisterService(m_mouseWrapper, ServiceLocater<DirectX11>::Get()->GetHWnd());
	// リソースマネージャを生成してサービスロケータに登録する
	RegisterResourceManager(m_textureResourceManager, L"テクスチャ");
	RegisterResourceManager(m_geometricPrimitiveResourceManager, L"ジオメトリックプリミティブ");
	RegisterResourceManager(m_modelResourceManager, L"モデル");
	RegisterResourceManager(m_fontResourceManager, L"フォント");

}

/// <summary>
/// 終了処理を行う
/// </summary>
void CommonServices::Finalize() {
	ServiceLocater<DirectX::CommonStates>::Unregister();
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Unregister();
	ServiceLocater<MouseWrapper>::Unregister();
	ServiceLocater<ResourceManager<TextureResource>>::Unregister();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Unregister();
	ServiceLocater<ResourceManager<ModelResource>>::Unregister();
	ServiceLocater<ResourceManager<FontResource>>::Unregister();
}
