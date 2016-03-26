#include "Log.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include "Game.h"

using namespace std;
using namespace Log;

bool Log::StopOnErrors = true;;

Handler Log::Error(std::cerr, "ERROR");
Handler Log::Warning(std::cerr, "WARNING");
Handler Log::Info(std::cout, "INFO");
Handler Log::Debug(std::cerr, "DEBUG");


/*
Prints an [hh:mm:ss.ms] time stamp. Surely there is an easier way to do this. :(
*/
string Log::GetTimeStamp()
{
	const float totalSeconds = Game::Instance().Time.TotalSeconds();
	
	int ms = (int)truncf(totalSeconds * 1000.f) % 1000;

	int ss = (int)floor(totalSeconds);

	int sec = ss % 60;
	int hours = ss / 3600;
	int minutes = (ss % 3600) / 60;

	stringstream str;

	str << "[" << hours << ":";

	str.width(2);
	str.fill('0');

	str << minutes << ":";
	
	str.width(2);
	str << sec << ".";

	str.width(3);

	str << ms << "] ";

	return str.str();

}



