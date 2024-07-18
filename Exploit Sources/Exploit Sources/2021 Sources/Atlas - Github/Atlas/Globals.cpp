#include <Windows.h>
#include "Globals.h"

HANDLE Globals::Handle = NULL;
DWORD Globals::BaseAddress = NULL;

HWND Globals::RobloxHWND = NULL;
HWND Globals::OverlayHWND = NULL;
HWND Globals::GuiHWND = NULL;

Players Globals::PlayersService = NULL;
Player Globals::LocalPlayer = NULL;
VisualEngine Globals::VisualEngine = NULL;

bool Globals::ESPBoxEnabled = true;
bool Globals::ESPTracerEnabled = true;
bool Globals::ESPNameEnabled = true;
bool Globals::ESPDistanceEnabled = true;
bool Globals::ESPHeadDotEnabled = true;

int Globals::TracerStartLocation = 0;

bool Globals::AimbotEnabled = true;
bool Globals::TeamCheck = false;
bool Globals::Crosshair = true;
bool Globals::ViewFOVCircle = true;

int Globals::Target = 2;
int Globals::AimbotBind = 0;

float Globals::CrosshairScale = 1;
float Globals::CrosshairThickness = 2;
float Globals::FOVSize = 100;
float Globals::FOVCircleThickness = 3;
float Globals::AimbotSmoothness = 8;
float Globals::ESPBoxThickness = 2;
float Globals::TracerThickness = 2;

// Format is RGBA.
float Globals::CrosshairColor[4] = { 1, 0, 0, 1 };
float Globals::FOVCircleColor[4] = { 1, 0, 0, 1 };
float Globals::ESPBoxColor[4] = { 1, 0, 0, 1 };
float Globals::TracerColor[4] = { 1, 0, 0, 1 };
float Globals::NameColor[4] = { 1, 1, 1, 1 };
float Globals::DistanceColor[4] = { 1, 1, 1, 1 };
float Globals::HeadDotColor[4] = { 1, 0, 0, 0.45 };