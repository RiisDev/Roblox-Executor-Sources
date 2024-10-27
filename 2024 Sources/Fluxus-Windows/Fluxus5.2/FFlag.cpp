#include "FFlag.hpp"

namespace Fluxus {
	namespace FFlag {
		struct SegmentInfo
		{
			DWORD Start;
			DWORD End;
			DWORD Size;
		};

		SegmentInfo Text, RData, Data;
		std::unordered_map<std::string, DWORD> LoadedFlags;

		SegmentInfo GetSegmentInfo(const char* sectionName)
		{
			IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)GetModuleHandle(0);
			IMAGE_NT_HEADERS* e_lfanewPtr = (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);
			IMAGE_SECTION_HEADER* sections = IMAGE_FIRST_SECTION(e_lfanewPtr);

			for (WORD i = 0; i < e_lfanewPtr->FileHeader.NumberOfSections; i++)
			{
				if (!memcmp(&sections[i].Name, (char*)sectionName, 8))
				{
					DWORD Start = (DWORD)((BYTE*)dosHeader + sections[i].VirtualAddress);
					DWORD Size = sections[i].Misc.VirtualSize;
					DWORD End = Start + Size;
					return SegmentInfo({ Start, End, Size });
				}
			}
			return SegmentInfo({ 0, 0, 0 });
		}

		void DefineSegments()
		{
			Text = GetSegmentInfo(".text");
			RData = GetSegmentInfo(".rdata");
			Data = GetSegmentInfo(".data");
		}

		std::vector<DWORD> FindPattern(char* pattern, char* mask, int Amount)
		{
			std::vector<DWORD> PatternsFound;
			DWORD patternLength = (DWORD)strlen(mask);

			for (DWORD i = Text.Start; i < Text.End - patternLength; i++)
			{
				bool found = true;
				for (DWORD j = 0; j < patternLength; j++)
				{
					found &= mask[j] == '?' || pattern[j] == *(char*)(i + j);
				}
				if (found)
				{
					PatternsFound.push_back(i);
					if (PatternsFound.size() == Amount)
					{
						break;
					}
				}
			}
			return PatternsFound;
		}

		void LoadFlags()
		{
			DefineSegments();
			std::vector<DWORD> Locations = FindPattern((char*)"\x6A\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x0C\xA3\x00\x00\x00\x00\xC3", (char*)"x?x????x????x????xxxx????x", 0);
			for (int i = 0; i < Locations.size(); i++)
			{
				LoadedFlags.insert({ std::string((char*)(*(DWORD*)(Locations[i] + 8))),*(DWORD*)(Locations[i] + 3) });
			}
		}

		template <typename T>
		bool SetFlagRaw(std::string FlagName, T To) {
			if (LoadedFlags.size() == 0)
			{
				LoadFlags();
			}
			try {
				DWORD Location = LoadedFlags.at(FlagName);
				*(T*)Location = To;
				return true;
			}
			catch (...)
			{
				return false;
			}
		}

		bool SetFlag(std::string Flag, const char* To) {
			return SetFlagRaw<const char*>(Flag, To);
		}

		const char* GetFlag(const char* Name) {
			if (LoadedFlags.size() == 0)
			{
				LoadFlags();
			}
			try {
				DWORD Location = LoadedFlags.at(Name);
				std::string Name = *(std::string*)Location;
				return Name.c_str();
			}
			catch(...){

			}
			return "";
		}


	}
}