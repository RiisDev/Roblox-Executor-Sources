#include "fps.h"

double get_fps() {
	return double(1) / functions::rbx_getscheduler()->fps;
}
void set_fps(double fps) {
	functions::rbx_getscheduler()->fps = 1 / (double)fps;
}