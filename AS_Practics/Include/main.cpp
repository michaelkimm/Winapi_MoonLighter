
#include <windows.h>
#include "Core\Core.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!CCore::Instance()->Init(hInstance))
	{
		CCore::DestroyInst();
		return 0;
	}

	int iRev = CCore::Instance()->Run();
	
	CCore::DestroyInst();

	return iRev;
}