#pragma once
#ifndef FIELD_DEFINED
#define FIELD_DEFINED

//#include "DirectX11.h"
//#include "Model.h"
//#include "CommonStates.h"
//#include "SimpleMath.h"

/// <summary>
/// フィールドクラス
/// </summary>
class Field {
public:
	Field();
	~Field();

public:
	void Update();
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	void DrawTurn(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// スカイドーム
	std::unique_ptr<DirectX::Model> m_skydome;
	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// Uターン画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_turnTexture;

};




#endif // !FIELD_DEFINED
