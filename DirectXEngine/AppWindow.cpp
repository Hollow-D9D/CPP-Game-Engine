#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"


struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;
	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;
	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(1.0f,1.0f,0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5, 0), Vector3D(1.5, 1.5, 0), m_delta_pos));

	//cc.m_world *= temp;

	/*
	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;
	*/
	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * .3f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * .3f);
	new_pos = new_pos + world_cam.getYDirection() * (m_upward * .3f);

	world_cam.setTranslation(new_pos);
	m_world_cam = world_cam;

	world_cam.inverse();

	cc.m_view = world_cam;
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	
	/*
	cc.m_proj.setOrthoLH
	(
		width / 300.0f,
		height / 300.0f,
		-4.0f,
		4.0f
	);
	*/


	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), .1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));
	vertex vertex_list[] =
	{
		//Front face
		{ Vector3D(-0.5f,-0.5f,-0.5f),	Vector3D(0,0,0),	Vector3D(0,1,0) },
		{ Vector3D(-0.5f,0.5f,-0.5f),	Vector3D(1,1,0),	Vector3D(0,1,1) },
		{ Vector3D(0.5f,0.5f,-0.5f),	Vector3D(0,0,1),	Vector3D(1,0,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),	Vector3D(1,1,1),	Vector3D(0,0,1) },
		//Back face
		{ Vector3D(0.5f,-0.5f,0.5f),	Vector3D(1,1,1),	Vector3D(0,0,1) },
		{ Vector3D(0.5f,0.5f,0.5f),		Vector3D(1,1,1),	Vector3D(0,0,1) },
		{ Vector3D(-0.5f,0.5f,0.5f),	Vector3D(1,1,1),	Vector3D(0,0,1) },
		{ Vector3D(-0.5f,-0.5f,0.5f),	Vector3D(1,1,1),	Vector3D(0,0,1) },


	};
	UINT size_list = ARRAYSIZE(vertex_list);
	
	unsigned int index_list[] =
	{
		//Front side
		0,1,2,
		2,3,0,
		//Back side
		4,5,6,
		6,7,4,
		//Top side
		1,6,5,
		5,2,1,
		//Bottom side
		7,0,3,
		3,4,7,
		//Right side
		3,2,5,
		5,4,3,
		//Left side
		7,6,1,
		1,0,7
	};
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	//VERTEX SHADER
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//PIXEL SHADER
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();


	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	//CLEAR RENDER TARGET
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.7f, 0.7f, 1);
	
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	//DRAW THE VERTICES
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);

}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);

}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		m_forward = -1.0f;
	}
	else if (key == 'D')
	{
		m_rightward = 1.0f;
	}
	else if (key == 'A')
	{
		m_rightward = -1.0f;
	}
	else if (key == 'E')
	{
		m_upward = 1.0f;
	}
	else if (key == 'Q')
	{
		m_upward = -1.0f;
	}

}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
	m_upward = 0.0f;

}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * .1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * .1f;

	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onLeftMouseButtonDown(const Point& mouse_pos)
{
	m_scale_cube -= .1f;
}

void AppWindow::onLeftMouseButtonUp(const Point& mouse_pos)
{
}

void AppWindow::onRightMouseButtonDown(const Point& mouse_pos)
{
	m_scale_cube += .1f;

}

void AppWindow::onRightMouseButtonUp(const Point& mouse_pos)
{
}

