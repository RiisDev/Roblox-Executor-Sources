#pragma once
#include <Windows.h>
#include "eyestep.h"

namespace EyeStep
{
	extern const char* convs[5];

	enum : uint8_t
	{
		c_cdecl,
		c_stdcall,
		c_fastcall,
		c_thiscall,
		c_auto
	};

	struct function_arg
	{
		uint32_t ebp_offset;
		uint8_t bits;
		uint8_t isCharPointer;
		uint32_t location;
	};

	struct function_info
	{
		function_info();

		uint32_t start_address;
		uint8_t convention;
		uint8_t return_bits;
		uint16_t stack_cleanup;
		uint32_t function_size;
		std::vector<function_arg> args;

		char psuedocode[256];

		void analyze(uint32_t func);
	};

	namespace util
	{
		extern DWORD setPageProtect(uint32_t address, DWORD protect, size_t size = 0x3FF);
		extern DWORD getPageProtect(uint32_t address);

		extern void writeByte(uint32_t address, uint8_t value);
		extern void writeBytes(uint32_t address, void* value, size_t count);
		extern void writeShort(uint32_t address, uint16_t value);
		extern void writeInt(uint32_t address, uint32_t value);
		extern void writeFloat(uint32_t address, float value);
		extern void writeQword(uint32_t address, uint64_t value);
		extern void writeDouble(uint32_t address, double value);

		extern uint8_t readByte(uint32_t address);
		extern uint8_t* readBytes(uint32_t address, size_t count);
		extern uint16_t readShort(uint32_t address);
		extern uint32_t readInt(uint32_t address);
		extern float readFloat(uint32_t address);
		extern uint64_t readQword(uint32_t address);
		extern double readDouble(uint32_t address);

		extern void freeBytes(uint8_t* bytes);
		extern void placeJmp(uint32_t from, uint32_t to);
		extern void placeCall(uint32_t from, uint32_t to);
		extern void placeTrampoline(uint32_t from, uint32_t to, size_t length);

		extern uint32_t rebase(uint32_t address);
		extern uint32_t aslr(uint32_t address);
		extern uint32_t raslr(uint32_t address);
		extern bool isRel(uint32_t address);
		extern bool isCall(uint32_t address);
		extern bool isPrologue(uint32_t address);
		extern bool isEpilogue(uint32_t address);
		extern bool isValidCode(uint32_t address);
		extern uint32_t getRel(uint32_t address);
		extern uint16_t getRetn(uint32_t func);
		extern uint32_t getPrologue(uint32_t func);
		extern uint32_t getEpilogue(uint32_t func);
		extern uint32_t nextPrologue(uint32_t address);
		extern uint32_t nextCall(uint32_t address, bool location = false, bool prologue = false);
		extern uint32_t prevCall(uint32_t address, bool location = false, bool prologue = false);
		extern uint32_t nextRef(uint32_t start, uint32_t func_search, bool prologue = true);
		extern uint32_t prevRef(uint32_t start, uint32_t func_search, bool prologue = true);
		extern uint32_t nextPointer(uint32_t start, uint32_t ptr_search, bool prologue = true);
		extern uint32_t prevPointer(uint32_t start, uint32_t ptr_search, bool prologue = true);

		extern std::vector<uint32_t> getCalls(uint32_t address);
		extern std::vector<uint32_t> getPointers(uint32_t address);

		extern uint8_t getConvention(uint32_t func, size_t n_expected_args); // method to grab calling conventions with 100% accuracy
		extern uint8_t getConvention(uint32_t func); // primary 'guessing' method
		extern uint32_t createRoutine(uint32_t func, uint8_t n_args);
		extern std::string getAnalysis(uint32_t func);
		extern void disableFunction(uint32_t func);

		extern std::vector<uint32_t> debug_r32(uint32_t address, uint8_t r32, uint32_t start_offset, size_t count = 1);
		extern std::string generate_sig(uint32_t func, size_t instructions);
	}

	namespace io
	{
		extern std::string readFile(const char* filename);
		extern void writeFile(const char* filename, void* data, size_t size);
		extern void appendFile(const char* filename, void* data, size_t size);
		extern inline bool checkFileExists(const char* filename);
		extern std::string readRawHttp(const char* sUrl);
	}

	namespace remote
	{
		// reserved for executables
		class f_remote
		{
		public:
			/*
			f_remote();
			~f_remote();

			void init();
			void finish();
			void run();
			*/
		};
	}

	namespace scanner
	{
		typedef std::vector<uint32_t> scan_results;

		enum
		{
			byte_equal,
			word_equal,
			int_equal,
			byte_notequal,
			word_notequal,
			int_notequal
		};

		struct scan_check
		{
			int type;
			int offset;
			void* value;
		};

		extern bool compare_bytes(const uint8_t* location, const uint8_t* aob, const char* mask);
		extern scan_results scan(const char* aob, bool code = true, int align = 1, int endresult = 0, std::vector<scan_check>checks = std::vector<scan_check>());
		extern scan_results scan_xrefs(const char* str, int nresult = 0);
		extern scan_results scan_xrefs(uint32_t func);
		extern std::string aobstring(const char* str); // converts a string to a scan-able AOB
		extern std::string ptrstring(uint32_t ptr); // converts a pointer to a scan-able AOB
	}
}
