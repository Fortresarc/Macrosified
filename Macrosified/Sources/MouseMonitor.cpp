#include "stdafx.h"
#include "MouseMonitor.h"
#include "Windowsx.h"

using namespace System;

// Global static members
HHOOK hMouseHook;
static long m_LastClickX;
static long m_LastClickY;
static BOOL m_LBDragged;

int MouseMonitor::StartThread()
{
	HANDLE hThread;
	DWORD dwThread;

	// Init
	m_LastClickX = 0;
	m_LastClickY = 0;
	m_LBDragged = FALSE;

	hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MouseMonitor::Run, 0, NULL, &dwThread);
	if (hThread)
		return WaitForSingleObject(hThread, INFINITE);
	else
		return 1;
}

LRESULT CALLBACK MouseMonitor::ProcessAllMouseEvents(
	int nCode,
	WPARAM wParam,
	LPARAM lParam)
{
	MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
	if (pMouseStruct != NULL) 
	{
		switch (wParam)
		{
			case WM_LBUTTONDOWN:
			{
				m_LastClickX = pMouseStruct->pt.x;
				m_LastClickY = pMouseStruct->pt.y;
				Console::WriteLine("Clicked X = {0}  Mouse Position Y = {1}\n", m_LastClickX, m_LastClickY);

				m_LBDragged = TRUE;

				// TODO remove this test code
				//RightClick(m_LastClickX + 50, m_LastClickY);
			}
			break;
			case WM_LBUTTONUP:
			{
				if (m_LBDragged)
				{
					Console::WriteLine("Drag stopped.");
				}
				m_LBDragged = FALSE;
			}
			break;
			case WM_MOUSEMOVE:
			{
				if (m_LBDragged)
				{
					Console::WriteLine("Dragged... {0}, {1}\n", pMouseStruct->pt.x, pMouseStruct->pt.y);
				}
			}
			break;

			default:
			{
				//Console::WriteLine("Mouse position X = {0}  Mouse Position Y = {1}\n", pMouseStruct->pt.x, pMouseStruct->pt.y);
			}
			break;
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
