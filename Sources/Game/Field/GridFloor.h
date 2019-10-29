#pragma once
#ifndef GRIDFLOOR_DEFINED
#define GRIDFLOOR_DEFINED


class GridFloor {
public:
	// コンストラクタ
	GridFloor(DirectX::CommonStates* states, float size, int divs);
	// デストラクタ
	~GridFloor();

public:
	// 描画する
	void Render(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& proj, DirectX::GXMVECTOR color = DirectX::Colors::Gray);

private:
	// エフェクト
	std::unique_ptr<DirectX::BasicEffect>                                  m_basicEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                              m_pInputLayout;
	// コモンステートへのポインタ
	DirectX::CommonStates*                                                 m_states;
	// 床の一辺のサイズ
	float                                                                  m_size;
	// 分割数
	int                                                                    m_divs;
};

#endif	// GRIDFLOOR_DEFINED


