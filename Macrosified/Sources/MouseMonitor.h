#pragma once

#include <windows.h>
#include <WinUser.h>
#pragma comment(lib, "user32.lib")

class MouseMonitor
{
public:
	int StartThread();

	static LRESULT CALLBACK ProcessAllMouseEvents(int nCode, WPARAM wParam, LPARAM lParam);

	/* Thread procedure can only be static procedure.
	   
	   We can still contain code in member function by using a static member function which 
	   can be passed as function pointer to CreateThread. Run has void* parameter where you 
	   can pass current instance pointer to that static fuction. Cast it correctly and then 
	   call to the member function that you need */
	static DWORD WINAPI Run(LPVOID i_Param)
	{
		return ((MouseMonitor*)i_Param)->_Run();
	}
	
	static void RightClick(long x, long y);

private:
	DWORD _Run();

};