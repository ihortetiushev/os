#pragma once

#include <string>
#include <vector>
#include <windows.h>

class MouseTrack
{
public:
	__declspec(dllexport) std::vector<POINT> DoMouseTrack();
	__declspec(dllexport) void SetTrackingTime(int time_in_milliseconds);
	//delay between points measurements. the less number we have here the more amount of bata we have to deal with
	__declspec(dllexport) void SetMeasurementResolution(int delay_in_milliseconds);

};
