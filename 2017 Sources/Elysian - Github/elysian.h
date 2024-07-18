#ifndef ELYSIAN_H
#define ELYSIAN_H
#pragma once

#include "rinstance.h"

#define ELYSIAN_DLL					"elysian.dll"
#define ELYSIAN_NAME				"Elysian"
#define ELYSIAN_NAME_LC				"elysian"
#define ELYSIAN_VERSION				" Test Build 2"
#define ELYSIAN_TITLE				ELYSIAN_NAME " v" ELYSIAN_VERSION

//#define DEBUG_MODE
#define DISABLE_WHITELIST

#define ELYSIAN_CREDITS	\
"Chirality	| sharing the idea for script execution and everything he's taught me\r\n" \
"FaZe	| bug testing, emotional support, and general help\r\n" \

extern RLua* rLua;
//
#endif