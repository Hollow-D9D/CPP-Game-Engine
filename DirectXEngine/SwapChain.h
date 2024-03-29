#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	//Initialize swapchain for a window
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* m_system);

	bool present(bool vsync);

	//Release the swapchain
	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;
	RenderSystem* m_system = nullptr;

private:
	friend class DeviceContext;
};

