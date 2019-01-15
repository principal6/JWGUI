#include "JWGUI.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	using namespace JW_GUI;
	static JWGUI myGUI;

	if (JW_OK(myGUI.Create(L"장원 GUI 시험용")))
	{
		myGUI.Run();
	}

	return 0;
}