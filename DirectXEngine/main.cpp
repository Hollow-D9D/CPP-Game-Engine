#include "AppWindow.h"


int main()
{
	try
	{
		GraphicsEngine::create();
	}
	catch (...)	{ return -1; }

	AppWindow app;
	if (app.init())
	{
		while (app.isRun())
		{
			app.broadcast();
		}
	}
	
	GraphicsEngine::release();

	return 0;
}