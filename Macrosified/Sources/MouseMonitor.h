#pragma once

#include <windows.h>
#include <WinUser.h>
#include <string>
#include <iostream>
#include "FileHandler.h"
#pragma comment(lib, "user32.lib")

using namespace std;

class MouseMonitor
{
public:
	enum ELogTo
	{
		eLogToConsole,
		eLogToFile,
		eLogToFileAndConsole
	};

	// Ensure one instance
	static MouseMonitor& GetInstance();

	int StartThread();
	void ChangeLogTo(ELogTo i_eLogTo = eLogToConsole);
	void Log(string i_LogText);

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
	MouseMonitor();
	~MouseMonitor();

	// Singleton
	static MouseMonitor* m_Instance;

	// Other attributes
	ELogTo m_eLogTo;
	FileHandler m_MouseEventLogFile;

	// Helpers
	DWORD _Run();

};