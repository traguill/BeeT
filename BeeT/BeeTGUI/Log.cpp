#include "Log.h"
#include "OutputLog.h"

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

void log(const char file[], int line, int level, const char* format, ...)
{
	if (level >= DEBUG_LEVEL)
	{
		static char buf[4096];
		static char buf2[4096];
		static char buf3[4096];
		static va_list ap;

		va_start(ap, format);
		vsprintf_s(buf, 4096, format, ap);
		va_end(ap);
		switch (level)
		{
		case 0:
			sprintf_s(buf2, 4096, "%s(%d) [INFO]: %s\n", file, line, buf);
			sprintf_s(buf3, 4096, "[INFO]: %s\n", buf);
			break;
		case 1:
			sprintf_s(buf2, 4096, "%s(%d) [WARNING]: %s\n", file, line, buf);
			sprintf_s(buf3, 4096, "[WARNING]: %s\n", buf);
			break;
		case 2:
			sprintf_s(buf2, 4096, "%s(%d) [ERROR]: %s\n", file, line, buf);
			sprintf_s(buf3, 4096, "[ERROR]: %s\n", buf);
			break;
		}
		OutputDebugString(buf2);
		if (g_output)
			g_output->Write(buf3);
	}
}