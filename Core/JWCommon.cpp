#include "JWCommon.h"

wchar_t JWCommon::ms_AppDir[MAX_PATH_LEN];

void JWCommon::SetApplicationDir()
{
	GetCurrentDirectoryW(MAX_PATH_LEN, ms_AppDir);
}