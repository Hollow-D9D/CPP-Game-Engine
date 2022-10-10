#pragma once
#include <d3d11.h>

class SwapChain
{
public:
	SwapChain();
	//Initialize swapchain for a window
	bool init(HWND &hwnd, UINT width, UINT height);
	//Release the swapchain
	bool release();

	~SwapChain();
private:
	IDXGISwapChain* m_swap_chain;
};

