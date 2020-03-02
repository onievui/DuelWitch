#include "CommonServices.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\Resource.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\InputManager.h>
#include <Game\Scene\ShareData\ShareData.h>


/// <summary>
/// サービス保持クラス
/// </summary>
class CommonServices::Services {
public:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                         commonStates;
	// キートラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>       keyboardStateTracker;
	// マウスラッパ―
	std::unique_ptr<MouseWrapper>                                  mouseWrapper;
	// パッドトラッカー
	std::unique_ptr<DirectX::GamePad::ButtonStateTracker>          padStateTracker;
	// 入力マネージャ
	std::unique_ptr<InputManager>                                  inputManager;

	// シーン間で共有するデータ
	std::unique_ptr<ShareData>                                     shareData;

	// リソースマネージャ
	std::unique_ptr<ResourceManager<TextureResource>>              textureResourceManager;
	std::unique_ptr<ResourceManager<GeometricPrimitiveResource>>   geometricPrimitiveResourceManager;
	std::unique_ptr<ResourceManager<ModelResource>>                modelResourceManager;
	std::unique_ptr<ResourceManager<SoundResource>>                soundResourceManager;
	std::unique_ptr<ResourceManager<BgmResource>>                  bgmResourceManager;
	std::unique_ptr<ResourceManager<FontResource>>                 fontResourceManager;
	std::unique_ptr<ResourceManager<VertexShaderResource>>         vertexShaderResourceManager;
	std::unique_ptr<ResourceManager<GeometryShaderResource>>       geometryShaderResourceManager;
	std::unique_ptr<ResourceManager<PixelShaderResource>>          pixelShaderResourceManager;

	// オーディオマネージャ
	std::unique_ptr<AudioManager>                                  audioManager;
};


/// <summary>
/// コンストラクタ
/// </summary>
CommonServices::CommonServices() {
	m_services = std::make_unique<Services>();
}

/// <summary>
/// デストラクタ
/// </summary>
CommonServices::~CommonServices() {

}

/// <summary>
/// ムーブコンストラクタ
/// </summary>
/// <param name="from">ムーブ元</param>
CommonServices::CommonServices(CommonServices&& from) = default;

/// <summary>
/// ムーブ代入演算子のオーバーロード
/// </summary>
/// <param name="from">ムーブ元</param>
/// <returns>
/// 代入先
/// </returns>
CommonServices& CommonServices::operator=(CommonServices&& from) = default;

/// <summary>
/// コモンサービスを初期化する
/// </summary>
void CommonServices::Initialize() {
	// サービスを生成してサービスロケータに登録する
	RegisterService(m_services->commonStates, ServiceLocater<DirectX11>::Get()->GetDevice().Get());
	RegisterService(m_services->keyboardStateTracker);
	RegisterService(m_services->mouseWrapper, ServiceLocater<DirectX11>::Get()->GetHWnd());
	RegisterService(m_services->padStateTracker);
	RegisterService(m_services->inputManager);
	RegisterService(m_services->shareData);
	RegisterService(m_services->audioManager);

	// リソースマネージャを生成してサービスロケータに登録する
	RegisterResourceManager(m_services->textureResourceManager, L"テクスチャ");
	RegisterResourceManager(m_services->geometricPrimitiveResourceManager, L"ジオメトリックプリミティブ");
	RegisterResourceManager(m_services->modelResourceManager, L"モデル");
	RegisterResourceManager(m_services->soundResourceManager, L"サウンド");
	RegisterResourceManager(m_services->bgmResourceManager, L"BGM");
	RegisterResourceManager(m_services->fontResourceManager, L"フォント");
	RegisterResourceManager(m_services->vertexShaderResourceManager, L"頂点シェーダ");
	RegisterResourceManager(m_services->geometryShaderResourceManager, L"ジオメトリシェーダ");
	RegisterResourceManager(m_services->pixelShaderResourceManager, L"ピクセルシェーダ");

}

/// <summary>
/// 終了処理を行う
/// </summary>
void CommonServices::Finalize() {
	ServiceLocater<DirectX::CommonStates>::Unregister();
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Unregister();
	ServiceLocater<MouseWrapper>::Unregister();
	ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Unregister();
	ServiceLocater<InputManager>::Unregister();
	ServiceLocater<ShareData>::Unregister();
	ServiceLocater<AudioManager>::Unregister();
	ServiceLocater<ResourceManager<TextureResource>>::Unregister();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Unregister();
	ServiceLocater<ResourceManager<ModelResource>>::Unregister();
	ServiceLocater<ResourceManager<SoundResource>>::Unregister();
	ServiceLocater<ResourceManager<BgmResource>>::Unregister();
	ServiceLocater<ResourceManager<FontResource>>::Unregister();
	ServiceLocater<ResourceManager<VertexShaderResource>>::Unregister();
	ServiceLocater<ResourceManager<GeometryShaderResource>>::Unregister();
	ServiceLocater<ResourceManager<PixelShaderResource>>::Unregister();
}

template<class T, class... Args>
/// <summary>
/// サービスを生成してサービスロケータに登録する
/// </summary>
/// <param name="service">登録するサービス</param>
/// <param name="...args">コンストラクタの引数</param>
void CommonServices::RegisterService(std::unique_ptr<T>& service, Args&&... args) {
	service = std::make_unique<T>(args...);
	ServiceLocater<T>::Register(service.get());
}

template<class T>
/// <summary>
/// リソースマネージャを生成してサービスロケータに登録する
/// </summary>
/// <param name="manager">リソースマネージャ</param>
/// <param name="kind">リソースの種類</param>
void CommonServices::RegisterResourceManager(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind) {
	manager = std::make_unique<ResourceManager<T>>(kind);
	ServiceLocater<ResourceManager<T>>::Register(manager.get());
}

