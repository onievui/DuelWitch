#pragma once
#ifndef MOUSE_WRAPPER_DEFINED
#define MOUSE_WRAPPER_DEFINED


/// <summary>
/// マウスラッパークラス
/// </summary>
class MouseWrapper {
public:
	// コンストラクタ
	MouseWrapper(HWND hWnd);
	// デストラクタ
	~MouseWrapper();

public:
	// マウスを更新する
	void Update(int width, int height);
	// マウスの状態をリセットする
	void Reset();

public:
	// マウスを取得する
	DirectX::Mouse* GetMouse();
	// トラッカーを取得する
	DirectX::Mouse::ButtonStateTracker* GetTracker();
	// マウス感度を設定する
	void SetSensivity(float sensivity);
	// マウス感度を取得する
	float GetSensivity() const;
	// マウスの座標を取得する
	const DirectX::SimpleMath::Vector2& GetPos() const;
	// マウスの座標を設定する
	void SetPos(const DirectX::SimpleMath::Vector2& pos);
	// マウスの移動量を取得する
	const DirectX::SimpleMath::Vector2& GetMoved() const;
	// マウスの座標モードを設定する
	void SetMode(DirectX::Mouse::Mode mode);

private:
	// マウス本体
	std::unique_ptr<DirectX::Mouse>                     m_mouse;
	// マウストラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_tracker;
	// マウス感度
	float                                               m_sensivity;
	// マウス座標
	DirectX::SimpleMath::Vector2                        m_pos;
	// マウス移動量
	DirectX::SimpleMath::Vector2                        m_moved;
	// マウスの座標モード
	DirectX::Mouse::Mode                                m_mode;
	// マウスを相対モードから絶対モードに変更したかのフラグ
	bool                                                m_wasChangedToAbsolute;
	// 絶対モードでのマウスの最終位置
	DirectX::SimpleMath::Vector2                        m_preAbsolutePos;
};


#endif // !MOUSE_WRAPPER_DEFINED
