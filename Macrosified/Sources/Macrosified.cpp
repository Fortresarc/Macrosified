// Macrosified.cpp : main project file.

#include "stdafx.h"
#include "MouseMonitor.h"

using namespace System;

int main(int argc, char** argv)
{
	MouseMonitor mouseMonitor;
	mouseMonitor.StartThread();
	return 0;
}
