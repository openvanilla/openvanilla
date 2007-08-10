#ifndef PROFILEFETCHER_H
#define PROFILEFETCHER_H

#include "Profile.h"
#include "FileHandler.h"

class ProfileFetcher
{
	public:
		ProfileFetcher()
		{}

		Profile* fetch(std::wstring id);

	private:
		FileHandler fileCache;

};

#endif