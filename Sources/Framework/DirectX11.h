#pragma once
#ifndef DIRECTX11_DEFINED
#define DIRECTX11_DEFINED
//#include <memory>
//#include <algorithm>
//#include <exception>
//#include <stdexcept>
//#include <d3d11_1.h>
//#include <DirectXMath.h>
//#include <DirectXColors.h>
//#include <windows.h>
//#include <wrl.h>
//#include <wrl/client.h>
//
//#include <SimpleMath.h>

#include "StepTimer.h"
#include <Utils\NonCopyable.h>


// DirectX11クラス
class DirectX11 : NonCopyable {
public:
	// ウィンドウ幅を取得する
	int GetWidth() const       { return m_width; }
	// ウィンドウ幅を設定する
	void SetWidth(int width)   { m_width = width; }
	// ウィンドウ高を取得する
	int GetHeight() const      { return m_height; }
	// ウィンドウ高を設定する
	void SetHeight(int height) { m_height = height; }
	// ウィンドウハンドルを取得する
	HWND GetHWnd() const       { return m_hWnd; }
	// ウィンドウハンドルを設定する
	void SetHWnd(HWND hWnd)    { m_hWnd = hWnd; }

	// デバイスを取得する
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const                     { return m_device; }
	// デバイスコンテキストを取得する
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const             { return m_context; }
	// スワップチェインを取得する
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const                { return m_swapChain; }
	// スワップチェインを設定する
	void SetSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain)        { m_swapChain = swapChain; }
	// レンダーターゲットビューを取得する
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const { return m_renderTargetView; }
	// デプスステンシルビューを取得する
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const { return m_depthStencilView; }

	// デバイスを生成する
	void CreateDevice();
	// リソースを生成する
	void CreateResources();
	// デバイスロストが発生した場合
	void OnDeviceLost();

private:
	// 幅
	int                                            m_width;
	// 高さ
	int                                            m_height;
	// ウィンドウハンドル
	HWND                                           m_hWnd;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
	Microsoft::WRL::ComPtr<ID3D11Device1>          m_device1;
	// デバイスコンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	   m_context;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>   m_context1;
	// 機能レベル
	D3D_FEATURE_LEVEL                              m_featureLevel;
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>        m_swapChain1;
	// レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	// デプスステンシルビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
};

#endif	// DIRECTX11_DEFINED

