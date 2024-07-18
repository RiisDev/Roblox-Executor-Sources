#pragma once

#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <Psapi.h>

#include "consolecolors.h"
#include "functions.h"
#include "addresses.h"

LONG WINAPI TaaprWareExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo);
void unpatch_SetUnhandledExceptionFilter();