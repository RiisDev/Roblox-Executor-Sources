#ifndef WINLUA_UTIL_INCLUDED
#define WINLUA_UTIL_INCLUDED
#include <Windows.h>
#include <string>
#include <vector>

int base_number = 1000;
int event_base_number = base_number;

std::vector<int> keys, event_keys;

int gen_random_form_key() {
	int gen = base_number + 1;
	base_number++;

	for (int i = 0; i < keys.size(); i++)
		if (i == gen)
			return gen_random_form_key();

	keys.push_back(gen);
	return gen;
}

int gen_random_event_key() {
	int gen = event_base_number + 1;
	event_base_number++;

	for (int i = 0; i < event_keys.size(); i++)
		if (i == gen)
			return gen_random_event_key();

	event_keys.push_back(gen);
	return gen;
}

/*
  I just added, because rand() never worked and returned the same thing for me.
  You can do whatever you want here with this, I was just bored and didn't have time.
*/
#endif
