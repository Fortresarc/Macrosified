// Macrosified.cpp : main project file.

#include "stdafx.h"
#include "MouseMonitor.h"

int main(int argc, char** argv)
{
	MouseMonitor::GetInstance().ChangeLogTo(MouseMonitor::eLogToFileAndConsole);
	MouseMonitor::GetInstance().StartThread();

	return 0;
}
