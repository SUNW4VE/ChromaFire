#include <Windows.h>
#include "Weapon.h"
#include <chrono>
#include <thread>

// default constructor
Weapon::Weapon() {
	detectionInterval = 0;
	weaponBind = '\0';
	categoryName = "";
}

// overloaded constructors
Weapon::Weapon(const std::string& categoryName) {
	this->detectionInterval = 0;
	this->weaponBind = 0;
	this->categoryName = categoryName;
}
Weapon::Weapon(int weaponBind, char detectionInterval) {
	this->detectionInterval = detectionInterval;
	this->weaponBind = weaponBind;
	this->categoryName = "";
}

// getters
int Weapon::getInterval() const {
	return detectionInterval;
}
int Weapon::getBind() const {
	return weaponBind;
}
std::string Weapon::getCategoryName() const {
	return categoryName;
}

// setters
void Weapon::setInterval(int newInterval) {
	detectionInterval = newInterval;
}
void Weapon::setCategoryName(const std::string& newName) {
	categoryName = newName;
}

// miscellaneous:

// pass by reference to reduce overhead
bool Weapon::chromaSearch(const int& CENTER_X, const int& CENTER_Y, int& r, int& g, int& b, 
	const int& RTHRESHOLD, const int& GBTOLERANCE, static HDC crosshair) {
	crosshair = ::GetDC(0);
	COLORREF color = GetPixel(crosshair, CENTER_X, CENTER_Y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);
	::ReleaseDC(0, crosshair);
	
	// if close enough to shade of red
	if (r > RTHRESHOLD && g < GBTOLERANCE && b < GBTOLERANCE)
		return true;
	else
		return false;
}

void Weapon::fire() const {
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(detectionInterval - 10));
}

void Weapon::zoomFire() const {
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(detectionInterval - 10));
}

// destructor
Weapon::~Weapon() {
}