#include "MouseWrapper.h"
#include <Framework\DirectX11.h>
#include "ServiceLocater.h"
#include "MathUtils.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="hWnd">ウインドウハンドル</param>
MouseWrapper::MouseWrapper(HWND hWnd)
	: m_sensivity(1.0f)
	, m_pos()
	, m_moved()
	, m_wasChangedToAbsolute(false)
	, m_preAbsolutePos() {
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(hWnd);
	m_tracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

/// <summary>
/// デストラクタ
/// </summary>
MouseWrapper::~MouseWrapper() {
	m_mouse.reset();
	m_tracker.reset();
}

/// <summary>
/// マウスを更新する
/// </summary>
/// <param name="width">ウインドウ幅</param>
/// <param name="height">ウインドウ高</param>
void MouseWrapper::Update(int width, int height) {
	DirectX::Mouse::State state = m_mouse->GetState();
	m_tracker->Update(state);

	// マウスのモードが絶対値参照なら
	if (state.positionMode == DirectX::Mouse::Mode::MODE_ABSOLUTE) {
		// 相対モードから絶対モードに変更した直後は直接変更する
		if (m_wasChangedToAbsolute) {
			m_wasChangedToAbsolute = false;
			SetPos(m_pos);
			m_moved.x = m_moved.y = 0.0f;
		}
		else {
			m_moved.x = state.x - m_pos.x;
			m_moved.y = state.y - m_pos.y;
			m_pos.x = static_cast<float>(state.x);
			m_pos.y = static_cast<float>(state.y);
			m_preAbsolutePos = m_pos;
		}
	}
	// マウスのモードが相対値参照なら
	else {
		m_moved.x = state.x*m_sensivity;
		m_moved.y = state.y*m_sensivity;
		// マウスの位置をウインドウ内に収める
		m_pos = Math::Clamp(m_pos + m_moved, DirectX::SimpleMath::Vector2::Zero,
			DirectX::SimpleMath::Vector2(static_cast<float>(width), static_cast<float>(height)));
	}
}

/// <summary>
/// マウスの状態をリセットする
/// </summary>
void MouseWrapper::Reset() {
	m_tracker->Reset();
}

/// <summary>
/// マウスを取得する
/// </summary>
/// <returns>
/// マウスへのポインタ
/// </returns>
DirectX::Mouse* MouseWrapper::GetMouse() {
	return m_mouse.get();
}

/// <summary>
/// トラッカーを取得する
/// </summary>
/// <returns>
/// トラッカーへのポインタ
/// </returns>
DirectX::Mouse::ButtonStateTracker* MouseWrapper::GetTracker() {
	return m_tracker.get();
}

/// <summary>
/// マウス感度を設定する
/// </summary>
/// <param name="sensivity">感度</param>
void MouseWrapper::SetSensivity(float sensivity) {
	m_sensivity = sensivity;
}

/// <summary>
/// マウス感度を取得する
/// </summary>
/// <returns>
/// 感度
/// </returns>
float MouseWrapper::GetSensivity() const {
	return m_sensivity;
}

/// <summary>
/// マウスの座標を取得する
/// </summary>
/// <returns>
/// マウスの座標
/// </returns>
const DirectX::SimpleMath::Vector2& MouseWrapper::GetPos() const {
	return m_pos;
}

/// <summary>
/// マウスの座標を設定する
/// </summary>
/// <param name="pos"></param>
void MouseWrapper::SetPos(const DirectX::SimpleMath::Vector2& pos) {
	if (m_mouse->GetState().positionMode == DirectX::Mouse::Mode::MODE_ABSOLUTE) {
		// 実際のマウスの座標を調べる
		POINT mouse_pos;
		mouse_pos.x = 0, mouse_pos.y = 0;
		GetCursorPos(&mouse_pos);
		
		// 指定した座標までの差分を加えて反映する
		mouse_pos.x += static_cast<LONG>(pos.x - m_preAbsolutePos.x);
		mouse_pos.y += static_cast<LONG>(pos.y - m_preAbsolutePos.y);
		SetCursorPos(mouse_pos.x, mouse_pos.y);
	}
	m_pos = pos;
}

/// <summary>
/// マウスの移動量を取得する
/// </summary>
/// <returns>
/// マウスの移動量
/// </returns>
const DirectX::SimpleMath::Vector2& MouseWrapper::GetMoved() const {
	return m_moved;
}

/// <summary>
/// マウスの座標モードを設定する
/// </summary>
/// <param name="mode">設定するモード</param>
void MouseWrapper::SetMode(DirectX::Mouse::Mode mode) {
	// 相対モードから絶対モードに変更したかどうか
	if (m_mouse->GetState().positionMode == DirectX::Mouse::Mode::MODE_RELATIVE && mode == DirectX::Mouse::Mode::MODE_ABSOLUTE) {
		m_wasChangedToAbsolute = true;
	}
	m_mouse->SetMode(mode);
}
