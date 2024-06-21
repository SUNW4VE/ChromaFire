/*

Need to:
	Block the input of only the trigger bind and nothing else (only check for the case of the trigger bind)
	- Trigger bind can be a mouse input, an alphanumeric keyboard input, or a Windows keyboard input (ctrl, alt, caps)
	Integrate this entire program into a singular function that returns TRUE if the trigger bind is HELD down

*/

#include <iostream>
#include "Windows.h"
using namespace std;

// global var's
HHOOK _mouseHook;
HHOOK _keyboardHook;

MSLLHOOKSTRUCT msStruct;
KBDLLHOOKSTRUCT kbdStruct;

DWORD vkCode;

const int triggerBind = 'D';
bool keyBlocked;

// mouse callback function
LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {

	// block out certain characters that can't be processed
	if (nCode >= 0) {

		// converts lParam to an actually usable state
		msStruct = *((MSLLHOOKSTRUCT*)lParam);

		switch (wParam) {

		case WM_LBUTTONDOWN:
			printf("LBUTTON was pressed and blocked\n");
			return -1;
		case WM_RBUTTONDOWN:
			printf("RBUTTON was pressed and blocked\n");
			return -1;
		case WM_MBUTTONDOWN:
			printf("MBUTTON was pressed and blocked\n");
			return -1;
		case WM_XBUTTONDOWN:

			if (HIWORD(msStruct.mouseData) == XBUTTON1) {
				printf("X button 1 (MB4) was pressed and blocked\n");
				return -1;
			}
			else if (HIWORD(msStruct.mouseData) == XBUTTON2) {
				printf("X button 2 (MB5) was pressed and blocked\n");
				return -1;
			}
		break;
		}
	}

	// pass hook information to next hook procedure
	return CallNextHookEx(_mouseHook, nCode, wParam, lParam);
}

// keyboard callback function 
LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {

	// block out certain characters that can't be processed
	if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {

		kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		vkCode = kbdStruct.vkCode;

		// Check for non-alphanumeric keys to block
		switch (vkCode) {

		case triggerBind:
			cout << (char)vkCode << " key was pressed and blocked\n";
			keyBlocked = true;
			return -1;
		default:
			cout << (char)vkCode << " key was pressed\n";
			break;
		}

	}
	return CallNextHookEx(_keyboardHook, nCode, wParam, lParam);
}


int main() {

	// SetWindowsHookEx() creates the hook
	// WH_MOUSE_LL is hook name

	_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);
	if (_mouseHook == NULL) {
		printf("ERROR (1): SetWindowsHookEx() - Failed to set mouse hook.\n");
		return 1;
	}

	_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0);
	if (_keyboardHook == NULL) {
		printf("ERROR (2): SetWindowsHookEx() - Failed to set keyboard hook.\n");
		return 2;
	}

	MSG msg;
	// first "0" is argument for HWND - use retrieved "hwnd" for main prog
	while (GetMessage(&msg, 0, 0, 0)) {

		// cout function does not go through?
		cout << "Test\n";
		PeekMessage(&msg, 0, 0, 0, 0x0001);


	}

	// global hooks should automatically unhook upon program termination, but this is still good practice
	UnhookWindowsHookEx(_mouseHook);
	UnhookWindowsHookEx(_keyboardHook);
	return 0;
}
