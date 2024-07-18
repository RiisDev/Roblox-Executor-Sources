#pragma once
#include <Windows.h>
#include "RBXClasses.h"

struct Globals {
	static HANDLE Handle;
	static DWORD BaseAddress;
	
	static HWND RobloxHWND;
	static HWND OverlayHWND;
	static HWND GuiHWND;

	static Players PlayersService;
	static Player LocalPlayer;
	static VisualEngine VisualEngine;

	static bool ESPBoxEnabled;
	static bool ESPTracerEnabled;
	static bool ESPNameEnabled;
	static bool ESPDistanceEnabled;
	static bool ESPHeadDotEnabled;

	static int TracerStartLocation;

	static bool AimbotEnabled;
	static bool TeamCheck;
	static bool Crosshair;
	static bool ViewFOVCircle;

	static int Target;
	static int AimbotBind;

	static float CrosshairScale;
	static float CrosshairThickness;
	static float FOVSize;
	static float FOVCircleThickness;
	static float AimbotSmoothness;
	static float ESPBoxThickness;
	static float TracerThickness;

	static float CrosshairColor[4];
	static float FOVCircleColor[4];
	static float ESPBoxColor[4];
	static float TracerColor[4];
	static float NameColor[4];
	static float DistanceColor[4];
	static float HeadDotColor[4];
};