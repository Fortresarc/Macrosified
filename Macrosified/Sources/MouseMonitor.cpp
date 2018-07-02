#include "stdafx.h"
#include "MouseMonitor.h"
#include <Windowsx.h>
#include <assert.h>

// Global static members
HHOOK hMouseHook;

// Mouse LButton drag
static BOOL m_LBDragged;
static long m_LBDragStartX;
static long m_LBDragStartY;

// Singleton
MouseMonitor* MouseMonitor::m_Instance = nullptr;

MouseMonitor::MouseMonitor()
{
	m_Instance = nullptr;
	
	if (!m_MouseEventLogFile.CreateNewFile("RecordedMacro"))
	{
		Log("Error!!!\n");
	}
}

MouseMonitor::~MouseMonitor()
{
	if (m_Instance)
	{
		delete m_Instance;
	}

	m_MouseEventLogFile.Close();
}

MouseMonitor& MouseMonitor::GetInstance()
{
	if (nullptr == m_Instance)
	{
		// Init static variables
		m_LBDragStartX = 0;
		m_LBDragStartY = 0;
		m_LBDragged = FALSE;
		
		m_Instance = new MouseMonitor();
		assert(nullptr != m_Instance);
	}
	return *m_Instance;
}

int MouseMonitor::StartThread()
{
	HANDLE hThread;
	DWORD dwThread;

	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MouseMonitor::Run, 0, NULL, &dwThread);
	if (hThread)
	{
		return WaitForSingleObject(hThread, INFINITE);
	}
	else
	{
		return 1;
	}
}

LRESULT CALLBACK MouseMonitor::ProcessAllMouseEvents(
	int nCode,
	WPARAM wParam,
	LPARAM lParam)
{
	MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
	if (pMouseStruct != nullptr) 
	{
		string logStr("");
		switch (wParam)
		{
			case WM_LBUTTONDOWN:
			{
				// Drag handling
				m_LBDragged = TRUE;
				m_LBDragStartX = pMouseStruct->pt.x;
				m_LBDragStartY = pMouseStruct->pt.y;

				// Log
				logStr.clear();
				logStr = ("Clicked = ")
					+ to_string(m_LBDragStartX)
					+ ", "
					+ to_string(m_LBDragStartY)
					+ "\n";
				MouseMonitor::GetInstance().Log(logStr);				

				// TODO remove this test code
				//RightClick(m_LastClickX + 50, m_LastClickY);
				break;
			}
			case WM_LBUTTONUP:
			{
				// Drag handling
				if (m_LBDragged)
				{
					GetInstance().Log(string("Drag stopped.\n"));
				}
				m_LBDragged = FALSE;
				break;
			}
			case WM_MOUSEMOVE:
			{
				// Drag handling
				if (m_LBDragged)
				{
					// Log
					logStr.clear();
					logStr = ("Dragged = ") 
						+ to_string(pMouseStruct->pt.x)
						+ ", "
						+ to_string(pMouseStruct->pt.y)
						+ "\n";
					GetInstance().Log(logStr);
				}
				break;
			}

			default:
			{
				break;
			}
		}
	}

	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

DWORD MouseMonitor::_Run()
{
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, ProcessAllMouseEvents, NULL, /*GetCurrentThreadId()*/0);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(hMouseHook);
	return 0;
}

void MouseMonitor::RightClick(
	long x,
	long y)
{
	/* Finds the size of the user's screen in pixels and converts that into pointer coordinates.
	   An annoying—but understandable—convention requires deals with screen locations in terms of 
	   absolute points; yet deals with pointer locations in terms of screen resolution*/

	// get system information regarding screen size / resolution
	double screenResolution_Width = GetSystemMetrics(SM_CXSCREEN) - 1;
	double screenResolution_Height = GetSystemMetrics(SM_CYSCREEN) - 1;
	// scale the function inputs 'x' and 'y' appropriately by a factor of 65535
	double dx = x*(65535.0 / screenResolution_Width);
	double dy = y*(65535.0 / screenResolution_Height);

	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = static_cast<LONG>(dx);
	input.mi.dy = static_cast<LONG>(dy);
	input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP);
	SendInput(1, &input, sizeof(INPUT));
}

void MouseMonitor::ChangeLogTo(ELogTo i_eLogTo /*= eLogToConsole*/)
{
	m_eLogTo = i_eLogTo;
}

void MouseMonitor::Log(string i_LogText)
{
	switch (m_eLogTo)
	{
		case eLogToConsole:
		{
			cout << i_LogText;
			break;
		}
		case eLogToFile:
		{
			m_MouseEventLogFile.Log(i_LogText);
			break;
		}
		case eLogToFileAndConsole:
		{
			m_MouseEventLogFile.Log(i_LogText);
			cout << i_LogText;
			break;
		}
		default:
		{
			break;
		}
	}
}