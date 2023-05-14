#include "pch.h"
#include "MouseTrack.h"
#include <chrono>
#include <thread>


int total_track_time = 60000; //by default 60000 ms = 1 min
int delay_between_points = 20; //by default 20ms

std::vector<POINT> MouseTrack::DoMouseTrack()
{
	std::vector<POINT> points;
	
	auto begin = std::chrono::high_resolution_clock::now();
	while (true)
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = end - begin;
		auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		POINT p;
		GetCursorPos(&p);
		points.push_back(p);
		if (durationInMs >= total_track_time)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_between_points));
	}
	return points;
}

void MouseTrack::SetTrackingTime(int time_in_milliseconds)
{
	total_track_time = time_in_milliseconds;
}

void MouseTrack::SetMeasurementResolution(int delay_in_milliseconds)
{
	delay_between_points = delay_in_milliseconds;
}

