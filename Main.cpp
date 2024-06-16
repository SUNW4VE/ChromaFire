#include <iostream>
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "Weapon.h"
using namespace std;

void checkLastBind(int key0, int key1, int key2, int key3, int key4, int key5,
	int& lastBind, int& lastBindCategoryNumber);
int getBind(string input);
void printBind(string categoryName, char standardBind);
void printBind(string categoryName, string customBind);

int main() {

	// set high process priority
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	cout << "Launched ChromaFire ver. 2.0. \n\nPlease set ADS mode to \"Hold\" in-game.\n"
		<< "Detection is on whenever the game window is focused and the toggle is switched to ON.\n"
		<< "Set toggle to OFF and press Ctrl + Q to exit ChromaFire.\n";

	// DETERMINE RESOLUTION & CENTER PIXEL
	if (!SetProcessDPIAware()) {
		cout << "\nERROR (1): SetProcessDPIAware() - Failed to enable DPI scaling consciousness. Exiting ChromaFire.\n";
		system("pause");
		return 1;
	}
	const int CENTER_X = (GetSystemMetrics(SM_CXSCREEN) / 2) - 3;
	const int CENTER_Y = (GetSystemMetrics(SM_CYSCREEN) / 2) - 3;
	cout << "\nAuto-detected Windows resolution: " << (CENTER_X + 3) * 2 << "x" << (CENTER_Y + 3) * 2 << endl; // RESLOG

	char toggleBind = '\0';
	int rThreshold = NULL;
	int gbTolerance = NULL;
	Weapon weapon[6] = { 
		Weapon("PRIMARY"),	// [0]
		Weapon("BACKUP"),	// [1]
		Weapon("MELEE"),	// [2]
		Weapon("SPECIAL"),	// [3]
		Weapon("SNIPER"),	// [4]
		Weapon("HEAVY")		// [5]
	};

	// FETCH WEAPON BINDS
	ifstream infile;
	cout << "\nFetching keybinds from config file...";

	infile.open("CF_Config.txt");
	if (infile.is_open()) {

		// filter out header/info reads
		for (int i = 0; i < 12; i++) {
			string infileDump;
			infile >> infileDump;
		}

		// get toggleBind
		string tempBind;
		infile >> tempBind;
		if (tempBind == "LCLICK" || tempBind == "RCLICK") {
			cout << "\nERROR (3): toggleBind - Do NOT set the toggle bind to a mouse click button. Exiting ChromaFire.\n";
			system("pause");
			return 3;
		}
		else {
			cout << "\n >> Toggle detection: [" << tempBind << "]";
			toggleBind = getBind(tempBind);
		}

		// filter out header/info reads
		for (int i = 0; i < 16; i++) {
			string infileDump;
			infile >> infileDump;
		}
		
		// get weapon binds
		for (int i = 0; i < 6; i++) {

			// filter out weapon category name reads (alternating)
			string infileDump;
			infile >> infileDump;

			string tempBind;
			infile >> tempBind;
			weapon[i].weaponBind = getBind(tempBind);

			// it is normal that uppercase version of the letter isn't printed in terminal,
			// because we are using tempBind instead of loadoutBind[i] as the print argument
			printBind(weapon[i].getCategoryName(), tempBind);
			
		}

		// filter out header/info reads
		for (int i = 0; i < 34; i++) {
			string infileDump;
			infile >> infileDump;
		}

		// save detection intervals
		cout << "\n\nFetching weapon detection intervals...";
		for (int i = 0; i < 6; i++) {

			string infileDump;
			infile >> infileDump;

			int tempInterval;
			infile >> tempInterval;
			if (tempInterval >= 0) {
				weapon[i].setInterval(tempInterval);
			}
			else {
				cout << "\nERROR (4): weapon.setInterval() - Please ensure that all detection intervals "
					<< "are set to 0 or higher. Exiting ChromaFire.\n";
				system("pause");
				return 4;
			}

			cout << "\n - " << weapon[i].getCategoryName() << ": " << weapon[i].getInterval() << " ms";
		}

		// filter out header/info reads
		for (int i = 0; i < 57; i++) {
			string infileDump;
			infile >> infileDump;
		}

		cout << "\n\nFetching thresholds and tolerances...";
		// fetch red tolerance
		infile >> rThreshold;
		cout << "\n - Red Threshold: " << rThreshold << "/255";

		for (int i = 0; i < 2; i++) {
			string infileDump;
			infile >> infileDump;
		}

		// fetch green/blue tolerance
		infile >> gbTolerance;
		cout << "\n - Green/Blue Tolerance: " << gbTolerance << "/255";


		infile.close();
	}
	else {
		cout << "\nERROR (2): inFile.is_open() - Failed to read from CF_Config.txt. Exiting ChromaFire.\n";
		system("pause");
		return 2;
	}

	// now that config fetching is done, get window handle
	cout << "\n\nSearching for running game instance...";
	HWND hwnd = NULL;
	while (!hwnd)
	{
		hwnd = FindWindowA(0, "Pixel Gun 3D");
		Sleep(500);
	}
	cout << "\nFound \"Pixel Gun 3D\" game instance.\n";
	
	// PRINCIPAL SECTION OF PROGRAM - detection + fire loop
	int lastBind = '\0';
	int lastBindCategoryNumber = NULL;
	static HDC crosshair = NULL;
	int r, g, b = NULL;
	bool toggleState = false;
	bool wasPressed = false;
	// int detectionCount = 0; // used for performance measurements

	while (!((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Q') & 0x8000))) {
		
		// split all of these conditions to avoid unnecessary checking
		while (GetForegroundWindow() == hwnd) {

			// keep tracking lastBind to later determine what fire type to call
			checkLastBind(weapon[0].weaponBind, weapon[1].weaponBind,
				weapon[2].weaponBind, weapon[3].weaponBind, weapon[4].weaponBind, weapon[5].weaponBind,
				lastBind, lastBindCategoryNumber);

			// check if toggle bind is currently pressed
			if (GetAsyncKeyState(toggleBind)) {

				// if the key was not pressed before, switch the toggle state
				if (!wasPressed) {

					toggleState = !toggleState;

					// log toggle state switches
					if (toggleState) {
						cout << "\nSet Toggle: ON";
					}
					else {
						cout << "\nSet Toggle: OFF";
					}

					wasPressed = true;

				}
			}
			else {

				// if the key is NOT pressed, reset the wasPressed flag
				wasPressed = false;

			}

			// execute the chromaSearch & fire conditionals if the toggle state is true
			if (toggleState) {
				// if the last bind was the sniper bind
				if (lastBindCategoryNumber == 4) {
					if (weapon[4].chromaSearch(CENTER_X, CENTER_Y, r, g, b, rThreshold, gbTolerance, crosshair)) {
						// cout << "\nzoomFire() Click!";
						// cout << " | chromaFire pass #" << ++detectionCount; // measure the speed of which chromaSearches happen
						weapon[lastBindCategoryNumber].zoomFire();
					}
				}
				// otherwise
				else if (weapon[lastBindCategoryNumber].chromaSearch(CENTER_X, CENTER_Y, r, g, b, rThreshold, gbTolerance, crosshair)) {
					// cout << "\nfire() Click!";
					weapon[lastBindCategoryNumber].fire();
				}
			}
		}

		this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	::ReleaseDC(0, crosshair);
	cout << "\nCtrl + Q was pressed. Exiting ChromaFire.\n";
	system("pause");
	return 0;
}


// set last weapon bind, then return number of weapon
void checkLastBind(int key0, int key1, int key2, int key3, int key4, int key5,
	int& lastBind, int &lastBindCategoryNumber) {

	// primary
	if ((GetAsyncKeyState(key0) & 0x8000)) {
		lastBind = key0;
		lastBindCategoryNumber = 0;
		// cout << "\nSet lastBind to " << key0 << ".";
		return;
	}
	// backup
	if ((GetAsyncKeyState(key1) & 0x8000)) {
		lastBind = key1;
		lastBindCategoryNumber = 1;
		// cout << "\nSet lastBind to " << key1 << ".";
		return;
	}
	// melee
	if ((GetAsyncKeyState(key2) & 0x8000)) {
		lastBind = key2;
		lastBindCategoryNumber = 2;
		// cout << "\nSet lastBind to " << key2 << ".";
		return;
	}
	// special
	if ((GetAsyncKeyState(key3) & 0x8000)) {
		lastBind = key3;
		lastBindCategoryNumber = 3;
		// cout << "\nSet lastBind to " << key3 << ".";
		return;
	}
	// sniper
	if ((GetAsyncKeyState(key4) & 0x8000)) {
		lastBind = key4;
		lastBindCategoryNumber = 4;
		// cout << "\nSet lastBind to SNIPER: " << key4 << ".";
		return;
	}
	// heavy
	if ((GetAsyncKeyState(key5) & 0x8000)) {
		lastBind = key5;
		lastBindCategoryNumber = 5;
		// cout << "\nSet lastBind to " << key5 << ".";
		return;
	}
}

int getBind(string input) {

	// first check for non-alphanumeric inputs

	if (input == "LCLICK")
		return VK_LBUTTON;
	else if (input == "RCLICK")
		return VK_RBUTTON;
	else if (input == "MCLICK")
		return VK_MBUTTON;
	else if (input == "MB4")
		return VK_XBUTTON1;
	else if (input == "MB5")
		return VK_XBUTTON2;
	else if (input == "LSHIFT")
		return VK_LSHIFT;
	else if (input == "RSHIFT")
		return VK_RSHIFT;
	else if (input == "CAPITAL")
		return VK_CAPITAL;
	else if (input == "TAB")
		return VK_TAB;
	else if (input == "LCTRL")
		return VK_LCONTROL;
	else if (input == "RCTRL")
		return VK_RCONTROL;
	else if (input == "LALT")
		return VK_LMENU;
	else if (input == "RALT")
		return VK_RMENU;
	else if (input == "BACKSPACE")
		return VK_BACK;
	else if (input == "ENTER")
		return VK_RETURN;
	else if (input == "LARROW")
		return VK_LEFT;
	else if (input == "RARROW")
		return VK_RIGHT;
	else if (input == "UARROW")
		return VK_UP;
	else if (input == "DARROW")
		return VK_DOWN;
	else if (input == "F1")
		return VK_F1;
	else if (input == "F2")
		return VK_F2;
	else if (input == "F3")
		return VK_F3;
	else if (input == "F4")
		return VK_F4;
	else if (input == "F5")
		return VK_F5;
	else if (input == "F6")
		return VK_F6;
	else if (input == "F7")
		return VK_F7;
	else if (input == "F8")
		return VK_F8;
	else if (input == "F9")
		return VK_F9;
	else if (input == "F10")
		return VK_F10;
	else if (input == "F11")
		return VK_F11;
	else if (input == "F12")
		return VK_F12;

	// auto-capitalize lowercase inputs
	else if (islower(input[0])) {
		input[0] = toupper(input[0]);
	}
	return input[0];
}

void printBind(string bindCategory, char standardKey) {
	cout << "\n - " << bindCategory << ": [" << standardKey << "]";
}

void printBind(string bindCategory, string customKey) {
	cout << "\n - " << bindCategory << ": [" << customKey << "]";
}