#pragma once

#include <string>
#include <vector>


extern __declspec(dllexport) std::vector<POINT> mouse_track();
extern __declspec(dllexport) void set_track_time(int time_in_milliseconds);
//delay between points measurements. the less munber we have here the more amount of bata we have to deal with
extern __declspec(dllexport) void set_measurement_resolution(int delay_in_milliseconds);