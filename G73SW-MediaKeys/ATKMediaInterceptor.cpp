#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>

#define WM_APPCOMMAND                     0x0319
#define APPCOMMAND_MEDIA_NEXTTRACK        11
#define APPCOMMAND_MEDIA_PREVIOUSTRACK    12
#define APPCOMMAND_MEDIA_STOP             13
#define APPCOMMAND_MEDIA_PLAY_PAUSE       14

#define ATKMEDIA_MESSAGE				  0x0917
#define ATKMEDIA_PLAY                     0x0002
#define ATKMEDIA_STOP                     0x0003
#define ATKMEDIA_PREV                     0x0005
#define ATKMEDIA_NEXT                     0x0004
#define ATKMEDIA_CALC                     0x002B

HINSTANCE instance;
TCHAR title[] = "ATKMEDIA";
TCHAR windowClass[] = "ATKMEDIA";

WORD registerClass(HINSTANCE currentInstance);
BOOL initInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR calc_name[] = "\\system32\\calc.exe";
TCHAR full_calc_name[MAX_PATH + sizeof(calc_name)];


int APIENTRY WinMain(HINSTANCE currentInstance, HINSTANCE prevInstance, LPTSTR cmdLine, int cmdShow)
{
	MSG msg;

	registerClass(currentInstance);

	if (!initInstance(currentInstance, cmdShow))
	{
		return FALSE;
	}

	UINT path_len = GetWindowsDirectory(full_calc_name, MAX_PATH);
	_tcscpy(full_calc_name+path_len,calc_name);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

WORD registerClass(HINSTANCE currentInstance)
{
	WNDCLASSEX classEx;

	classEx.cbSize = sizeof(WNDCLASSEX);

	classEx.style			= CS_HREDRAW | CS_VREDRAW;
	classEx.lpfnWndProc   	= WndProc;
	classEx.cbClsExtra		= 0;
	classEx.cbWndExtra		= 0;
	classEx.hInstance		= currentInstance;
	classEx.hIcon			= 0;
	classEx.hCursor		    = LoadCursor(NULL, IDC_ARROW);
	classEx.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	classEx.lpszMenuName	= 0;
	classEx.lpszClassName	= windowClass;
	classEx.hIconSm		    = 0;

	return RegisterClassEx(&classEx);
}

BOOL initInstance(HINSTANCE currentInstance, int cmdShow)
{
   HWND hWnd;

   instance = currentInstance;

   hWnd = CreateWindow(windowClass, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, currentInstance, NULL);

   if(!hWnd)
   {
      return FALSE;
   }

   return TRUE;
}

void popup_calculator()
{
	   HWND hdlCalc = NULL;
	   hdlCalc = FindWindow(TEXT("CalcFrame"), TEXT("Calculator"));
	   if(hdlCalc)
	   {
			ShowWindow(hdlCalc,SW_RESTORE);
	   }
	   else
	   {
			ShellExecute(NULL, TEXT("open"), full_calc_name, TEXT(""),NULL,SW_SHOWNORMAL); 	   
	   }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.dwFlags = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	bool send_command = false;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

		case ATKMEDIA_MESSAGE:
			switch(wmEvent)
			{
			case ATKMEDIA_PLAY:
				input.ki.wVk = VK_MEDIA_PLAY_PAUSE;
				send_command = true;
				break;
			case ATKMEDIA_STOP:
				input.ki.wVk = VK_MEDIA_STOP;
				send_command = true;
				break;
			case ATKMEDIA_NEXT:
				input.ki.wVk = VK_MEDIA_NEXT_TRACK;
				send_command = true;
				break;
			case ATKMEDIA_PREV:
				input.ki.wVk = VK_MEDIA_PREV_TRACK;
				send_command = true;
				break;
			case ATKMEDIA_CALC:
				popup_calculator();
				break;
			}
			
			if(send_command){
				SendInput(1, &input, sizeof(INPUT));
			}
			//intentional fall-through!!!!
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}