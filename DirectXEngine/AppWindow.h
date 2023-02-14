#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"


class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	void update();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	// Keyboard
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	
	// Mouse
	virtual void onMouseMove(const Point& delta_mouse_pos) override;

	virtual void onLeftMouseButtonDown(const Point& mouse_pos) override;
	virtual void onLeftMouseButtonUp(const Point& mouse_pos) override;
	
	virtual void onRightMouseButtonDown(const Point& mouse_pos) override;
	virtual void onRightMouseButtonUp(const Point& mouse_pos) override;


private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;
	IndexBuffer* m_ib;
private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x;
	float m_rot_y;

	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_upward = 0.0f;


	Matrix4x4 m_world_cam;

};

