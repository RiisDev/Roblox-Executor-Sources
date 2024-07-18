#pragma once

#define M_PI       3.14159265358979323846   // pi
#include <cmath>
#include <cstdint>
#include <tuple>
#include "../sdk/vec.h"


float* rainbow_color(float time, float speed) noexcept
{
	float* arr = new float[3];
	arr[0] = std::sin(speed * time) * 0.5f + 0.5f;
	arr[1] = std::sin(speed * time + static_cast<float>(2 * M_PI / 3)) * 0.5f + 0.5f;
	arr[2] = std::sin(speed * time + static_cast<float>(4 * M_PI / 3)) * 0.5f + 0.5f;
	return arr;
}
