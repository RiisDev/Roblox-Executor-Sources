#pragma once

#include "TypeDefs.h"
#include <vector>

#define WaveHeight_PropDescriptor				(DWORD*)0x16A97F4 // 7
#define DisplayDistanceType_PropDescriptor		(DWORD*)0x16A04FC // 7
#define FogColor_PropDescriptor					(DWORD*)0x16A18B8 // 7
#define FogStart_PropDescriptor					(DWORD*)0x16A18EC // 7
#define FogEnd_PropDescriptor					(DWORD*)0x16A1920 // 7
#define WaterColor_PropDescriptor               (DWORD*)0x16A958C // 7
#define IntValSetVal_PropDescriptor             (DWORD*)0x16A26C0 // 7
#define RobloxLocked_PropDescriptor             (DWORD*)0x1674108 // 7
#define UserId_PropDescriptor                   (DWORD*)0x166E5F0 // 1
#define StringValSetVal_PropDescriptor			(DWORD*)0x16A2414 // 7
#define BoolValSetVal_PropDescriptor			(DWORD*)0x16A2728 // 7
#define NumberValSetVal_PropDescriptor			(DWORD*)0x16A2790 // 7

#define IntVal_ZeroVal							(DWORD*)0x1676F40 // 7

#define SetParent								0x44A010 // 7
#define ReplicateProperty						0x4EC480 // 7
#define SetWalkSpeed							0x69A8A0 // 7
#define SetMxHealth								0x699A20 // 7
#define SetHealthA                              0x699480 // 7
#define GetMxHealth								0x6929F0 // 7
#define SetJmpPower								0x699640 // 7	
#define AutoRotate                              0x698EB0 // 7
#define ClearAll                                0xE7DF00 // doesnt work dont update
#define PSR                                     0xA91520 // 7
#define Time                                    0x6AB6D0 // 7
#define SetJump                                 0x699560 // 7
#define SetSit                                  0x69A120 // 7
#define PlatformS                               0x699BF0 // 7
#define SendChatA								0xAA3C10 // 7
#define CameraSubjectA							0x673220 // 7
#define LoadAsset								0x7DFEE0 // 7

#define BinType									0x677E90 // 7

//#define SetStringValA							0x6D6190 // 4
#define SetNumberValA							0x6C6A50 // 5

//Part Addresses
#define SetTrans                                0x6DD7A0 // 7
#define Locked                                  0x6DCE60 // 7
#define SetRelfec								0x6DD2D0 // 7
#define SetMat									0x6DD300 // 7
#define CanCollideA								0x6D2D12 // 7
#define AnchorAddr                              0x6DBC50 // 7

#define SetCFrameA								0x6DC030 // 7
#define GetCFrameA								0x6D6F30 // 7


//haxs
#define SetTextLabelText                        0x909E80 // 7
#define GetTextLabelText						0x967760 // 7

#define SetGuiVisibleA							0x88A2C0 // 7
#define SetGuiPosition                          0x889AF0 // 7
#define SetGuiSizeA								0x889F30 // 7

//and thats when roblox knew.... they fucked up.
#define ClearContentsA							0x54C510 // 5
#define ReportAbuseA							0xAB3320 // 6
#define DebrisCleanupA							0x7D49A0 // 6

#define SetObjectValueA							0x6C46F0 // 7
#define GetObjectValueA							0x6C2E40 // 7

#define SetPitchA								0x62CDF0 // 7
#define SetVolumeA								0x62D5C0 // 7
#define IsPlayingA								0x62AD70 // 7
#define IsPausedA								0x62ACC0 // 7
#define PlaySoundA								0x62C160 // 7
#define SoundIdA								0x62CF80 // 7
#define StopSoundA								0x62D8F0 // 7

namespace Rosin {
	namespace Roblox {

		typedef int(__cdecl* opnew)(int);
		typedef void(__thiscall* insNew)(int, int, int);

		static opnew OperNew = (opnew)0xC604AA; // 7

		static insNew InsNewFunc = (insNew)0x4F6190; // 7

		static haexs LookupFunc = (haexs)0x735360;

		//static numVal SetNumberValue = (numVal)SetNumberValA;
		static s SetGuiVisible = (s)SetGuiVisibleA;
		static s CameraSubject = (s)CameraSubjectA;
		static clr DebrisCleanup = (clr)DebrisCleanupA;
		static ReportP ReportAbuse = (ReportP)ReportAbuseA;
		static sfs SetRelfectance = (sfs)SetRelfec;
		static sfs SetJumpPower = (sfs)SetJmpPower;
		static sfs SetWS = (sfs)SetWalkSpeed;
		static sfs SetMaxHealth = (sfs)SetMxHealth;
		static getPlrChar GetMaxHealth = (getPlrChar)GetMxHealth;
		static sfs AutoR = (sfs)AutoRotate;
		static r Replicate = (r)ReplicateProperty;
		static s rSetParent = (s)SetParent;
		static clr ClearA = (clr)ClearAll;
		static PSRR PSRRR = (PSRR)PSR;
		static settime SetMinutes = (settime)Time;
		static sfs SetPartTrans = (sfs)SetTrans;
		static s CanCollide = (s)CanCollideA;
		static setjump Jump = (setjump)SetJump;
		static setjump PlatformStand = (setjump)PlatformS;
		static s SetAnchored = (s)AnchorAddr;
		static sfs SetHealth = (sfs)SetHealthA;
		static s SetMaterial = (s)SetMat;
		static SetCFrameD SetCFrame = (SetCFrameD)SetCFrameA;
		static GetCFrameD GetCFrame = (GetCFrameD)GetCFrameA;
		static sfs SetPartLocked = (sfs)Locked;
		static setjump Sit = (setjump)SetSit;
		static getPlrChar GetObjectValue = (getPlrChar)GetObjectValueA;
		static s SetObjectValue = (s)SetObjectValueA;
		static s SetBinType = (s)BinType;

		static clrContent ClearContent = (clrContent)ClearContentsA;

		static SoundIdd SetTLText = (SoundIdd)SetTextLabelText;
		static setUdim2 SetGuiPos = (setUdim2)SetGuiPosition;
		static setUdim2 SetGuiSize = (setUdim2)SetGuiSizeA;

		static SoundIdd SoundId = (SoundIdd)SoundIdA;
		static startSound playSound = (startSound)PlaySoundA;
		static stopSound stopMusic = (stopSound)StopSoundA;
		static sfs SetPitch = (sfs)SetPitchA;
		static sfs SetVolume = (sfs)SetVolumeA;
		static spl IsPlaying = (spl)IsPlayingA;
		static spl IsPaused = (spl)IsPausedA;

		//static SoundIdd SetStringValue = (SoundIdd)SetStringValA;

		static s CameraMode = (s)0xA90330;//7
		static SoundIdd SendChat = (SoundIdd)SendChatA;
		static clr ClearTerrain = (clr)0x7BB5A0;//7
		static SetColorD SetMeshScale = (SetColorD)0x8B5C80;//7
		static s SetPlrChar = (s)0xA903B0;//7
		static setIName SetName = (setIName)0x4ED3D0; //7
		static setIName SetCharApp = (setIName)0xA90850; //7
		//static cloneDef CloneIns = (cloneDef)0x4E7850;//6
		//static cloneAft CloneInsAft = (cloneAft)0x6BAD70; // 6

		static s SetArchivable = (s)0x4E655A;//7

		static loadCharDef LoadChar = (loadCharDef)0xAA4260; // 1

		static SoundIdd ExecuteS = (SoundIdd)0x837B90; // 7

		static clr LoadCoreScripts = (clr)0x583E40;

		static s SetAccountAge = (s)0xA8FD00; // 7

		static s SetMembershipType = (s)0xA913C0; // 7

		static ripchar CanLoadCharacterAppearance = (ripchar)0xA90380;//7
		static BreakJ BreakJoints = (BreakJ)0x680A10; //7

		static setIName SkyboxBk = (setIName)0x8F5B00; //7
		static setIName SkyboxDn = (setIName)0x8F5B50; //7
		static setIName SkyboxFt = (setIName)0x8F5BA0; //7
		static setIName SkyboxLf = (setIName)0x8F5C00; //7
		static setIName SkyboxRt = (setIName)0x8F5C40; //7
		static setIName SkyboxUp = (setIName)0x8F5C80; //7
		static s SetStarCount = (s)0x8F5AA0;//7
	}
}