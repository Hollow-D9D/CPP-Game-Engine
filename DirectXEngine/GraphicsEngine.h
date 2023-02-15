#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all resources loaded
	bool release();
	
	RenderSystem* getRenderSystem();

	~GraphicsEngine();

	static GraphicsEngine* get();
private:
	RenderSystem* m_render_system = nullptr;


};

