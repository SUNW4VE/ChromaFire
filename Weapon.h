#ifndef WEAPON_H
#define WEAPON_H

#include <Windows.h>
#include <string>

class Weapon {

public:

	// default constructor
	Weapon();

	// overloaded constructors
	Weapon(const std::string& categoryName);
	Weapon(int weaponBind, char detectionInterval);

	// getters
	int getInterval() const;
	int getBind() const;
	std::string getCategoryName() const;


	// setters
	void setInterval(int newInterval);
	void setCategoryName(const std::string& newName);

	// miscellaneous
	bool chromaSearch(const int& CENTER_X, const int& CENTER_Y, int& r, int& g, int& b, 
		const int& RTHRESHOLD, const int& GBTOLERANCE, static HDC crosshair);
	void fire() const;
	void zoomFire() const;

	// destructor
	~Weapon();

	// weaponBind and detectionInterval must be made public to reduce unnecessary overhead
	int weaponBind;

private:

	int detectionInterval;
	std::string categoryName;

};


#endif
