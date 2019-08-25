#include "CommonServices.h"
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>


/// <summary>
/// コモンサービスを初期化する
/// </summary>
void CommonServices::Initialize() {
	// サービスを生成してサービスロケータに登録する
	RegisterService(m_keyboardStateTracker);
	RegisterService(m_mouseStateTracker);
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
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Remove();
	ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Remove();
	ServiceLocater<ResourceManager<TextureResource>>::Remove();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Remove();
	ServiceLocater<ResourceManager<ModelResource>>::Remove();
	ServiceLocater<ResourceManager<FontResource>>::Remove();
}
