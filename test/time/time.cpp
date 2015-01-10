// time.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"


int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	printf ("%d:%d:%d:%d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	return 0;
}

