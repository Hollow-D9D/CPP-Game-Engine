#pragma once
#include <windows.h>
#include <iostream>

class Window
{
	public:
		Window();
		//Initialize the window
		bool init();
		bool broadcast();
		//Release the window
		bool release();

		bool isRun();

		//EVENTS
		virtual void onCreate();
		virtual void onUpdate() = 0;
		virtual void onDestroy();


		~Window();
	protected:
		HWND m_hwnd;
		bool m_is_run;
};

