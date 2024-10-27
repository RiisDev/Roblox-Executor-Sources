#include "./ExceptionHandler.hpp"

ExceptionManager::EHSettings ExceptionManager::g_ehsettings;
char ExceptionManager::g_ehreportbuffer [ 16384 ];

std::string ExceptionManager::getBack( const std::string& s, char delim ) {
	std::stringstream ss( s );
	std::string item;

	while ( std::getline( ss, item, delim ) );

	return item;
}

std::string ExceptionManager::StackWalkReport( PEXCEPTION_POINTERS pExceptionRecord )
{
	std::stringstream StackWalkReport;

	STACKFRAME stackFrame;
	memset( &stackFrame, 0, sizeof( STACKFRAME ) );

#if (INTPTR_MAX == INT32_MAX)
	stackFrame.AddrPC.Offset = pExceptionRecord->ContextRecord->Eip;
	stackFrame.AddrStack.Offset = pExceptionRecord->ContextRecord->Esp;
	stackFrame.AddrFrame.Offset = pExceptionRecord->ContextRecord->Ebp;

	DWORD Eax = pExceptionRecord->ContextRecord->Eax,
		Ebx = pExceptionRecord->ContextRecord->Ebx,
		Ecx = pExceptionRecord->ContextRecord->Ecx,
		Edx = pExceptionRecord->ContextRecord->Edx,
		Esi = pExceptionRecord->ContextRecord->Esi,
		Edi = pExceptionRecord->ContextRecord->Edi;
#elif (INTPTR_MAX == INT64_MAX)
	stackFrame.AddrPC.Offset = pExceptionRecord->ContextRecord->Rip;
	stackFrame.AddrStack.Offset = pExceptionRecord->ContextRecord->Rsp;
	stackFrame.AddrFrame.Offset = pExceptionRecord->ContextRecord->Rbp;

	DWORD Rax = pExceptionRecord->ContextRecord->Rax,
		Rbx = pExceptionRecord->ContextRecord->Rbx,
		Rcx = pExceptionRecord->ContextRecord->Rcx,
		Rdx = pExceptionRecord->ContextRecord->Rdx,
		Rsi = pExceptionRecord->ContextRecord->Rsi,
		Rdi = pExceptionRecord->ContextRecord->Rdi;
#endif

	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrStack.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Mode = AddrModeFlat;

	SymInitialize( GetCurrentProcess( ), NULL, TRUE );

	std::vector<std::vector<std::string>> reportStringParseCache {};

	size_t fileInfoStrMax = 0, fileInfoStrTemp = 0;
	size_t moduleInfoStrMax = 0, moduleInfoStrTemp = 0;
	size_t lineNumberStrMax = 0, lineNumberStrTemp = 0;
	size_t dynAddrStrMax = 0, dynAddrStrTemp = 0;
	size_t constAddrStrMax = 0, constAddrStrTemp = 0;
	size_t baseAddrStrMax = 0, baseAddrStrTemp = 0;
	/* 0: file info
	   1: module info
	   2: line number or ""
	   3: dyn or static addr
	   4: base addr or 0      */

#if (INTPTR_MAX == INT32_MAX)
	while ( StackWalk( IMAGE_FILE_MACHINE_I386, GetCurrentProcess( ), GetCurrentThread( ), &stackFrame, pExceptionRecord->ContextRecord, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL ) )
#elif (INTPTR_MAX == INT64_MAX)
	while ( StackWalk64( IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess( ), GetCurrentThread( ), &stackFrame, pExceptionRecord->ContextRecord, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL ) )
#endif
	{
		CHAR moduleName [ MAX_PATH ];
		PCHAR symbolName;
		PCHAR fileName;
		DWORD lineNumber;

		HMODULE moduleBase = ( HMODULE )SymGetModuleBase( GetCurrentProcess( ), stackFrame.AddrPC.Offset );
		if ( moduleBase )
		{
			GetModuleFileNameA( moduleBase, moduleName, MAX_PATH );
		}
		else
		{
			sprintf_s( moduleName, "UnkMod" );
		}

		CHAR symbolBuf [ sizeof( IMAGEHLP_SYMBOL ) + 255 ];
		PIMAGEHLP_SYMBOL symbol = ( PIMAGEHLP_SYMBOL )symbolBuf;
		symbol->SizeOfStruct = sizeof symbolBuf;
		symbol->MaxNameLength = 254;

		DWORD32 offset;
#if (INTPTR_MAX == INT32_MAX)
		DWORD disp;
#elif (INTPTR_MAX == INT64_MAX)
		DWORD64 disp;
#endif
		symbolName = SymGetSymFromAddr( GetCurrentProcess( ), stackFrame.AddrPC.Offset, &disp, symbol )
			? symbol->Name : PCHAR( "UnkSym" );

		IMAGEHLP_LINE line;
		line.SizeOfStruct = sizeof line;

		if ( SymGetLineFromAddr( GetCurrentProcess( ), stackFrame.AddrPC.Offset, ( PDWORD )&offset, &line ) )
		{
			fileName = line.FileName;
			lineNumber = line.LineNumber;
		}
		else
		{
			fileName = PCHAR( "UnkFile" );
			lineNumber = 0;
		}

		if ( moduleBase == 0 )
		{
			if ( stackFrame.AddrPC.Offset > ( std::uintptr_t )g_ehsettings.prog_base && stackFrame.AddrPC.Offset < ( std::uintptr_t )g_ehsettings.prog_base + g_ehsettings.prog_size )
			{
				if ( !GetModuleFileNameA( ( HMODULE )g_ehsettings.prog_base, moduleName, sizeof moduleName ) )
				{
					sprintf_s( moduleName, g_ehsettings.prog_name.value( ).c_str( ) );
					moduleBase = ( HMODULE )g_ehsettings.prog_base;
				}
			}
		}
#if (INTPTR_MAX == INT32_MAX)
		std::uintptr_t normalizedAddress = stackFrame.AddrPC.Offset - ( DWORD32 )moduleBase;
#elif (INTPTR_MAX == INT64_MAX)
		std::uintptr_t normalizedAddress = stackFrame.AddrPC.Offset - ( DWORD64 )moduleBase;
#endif

		CHAR tempFileInfoString [ 192 ];
		CHAR tempModuleInfoString [ MAX_PATH ];
		CHAR tempLineInfoString [ 48 ];
		CHAR tempDynAddrString [ 64 ];
		CHAR tempConstAddrString [ 64 ];
		CHAR tempBaseAddrString [ 64 ];

		sprintf_s( tempFileInfoString, "[File: %s]", getBack( fileName, '\\' ).c_str( ) );
		sprintf_s( tempModuleInfoString, sizeof tempModuleInfoString, moduleBase != 0 ? "[Module: %s]" : "[Module: Unknown]", getBack( moduleName, '\\' ).c_str( ) );
		sprintf_s( tempLineInfoString, lineNumber != 0 ? "[Line: %i]" : "", lineNumber );
#if (INTPTR_MAX == INT32_MAX)
		sprintf_s( tempDynAddrString, "[DynamicAddr: 0x%08X]", normalizedAddress );
		sprintf_s( tempConstAddrString, "[RebaseAddr: 0x%08X]", normalizedAddress );
		sprintf_s( tempBaseAddrString, "[BaseAddr: 0x%08X]", ( std::uintptr_t )moduleBase );
#elif (INTPTR_MAX == INT64_MAX)
		sprintf_s( tempDynAddrString, "[DynamicAddr: 0x%016llX]", normalizedAddress );
		sprintf_s( tempConstAddrString, "[RebaseAddr: 0x%016llX]", normalizedAddress );
		sprintf_s( tempBaseAddrString, "[BaseAddr: 0x%016llX]", ( std::uintptr_t )moduleBase );
#endif

		fileInfoStrMax = fileInfoStrMax < ( fileInfoStrTemp = strlen( tempFileInfoString ) ) ? fileInfoStrTemp : fileInfoStrMax;
		moduleInfoStrMax = moduleInfoStrMax < ( moduleInfoStrTemp = strlen( tempModuleInfoString ) ) ? moduleInfoStrTemp : moduleInfoStrMax;
		lineNumberStrMax = lineNumberStrMax < ( lineNumberStrTemp = strlen( tempLineInfoString ) ) ? lineNumberStrTemp : lineNumberStrMax;
		dynAddrStrMax = dynAddrStrMax < ( dynAddrStrTemp = strlen( tempDynAddrString ) ) ? dynAddrStrTemp : dynAddrStrMax;
		constAddrStrMax = constAddrStrMax < ( constAddrStrTemp = strlen( tempConstAddrString ) ) ? constAddrStrTemp : constAddrStrMax;
		baseAddrStrMax = baseAddrStrMax < ( baseAddrStrTemp = strlen( tempBaseAddrString ) ) ? baseAddrStrTemp : baseAddrStrMax;

		std::vector<std::string> stringParseVector {
			tempFileInfoString,
			tempModuleInfoString,
			lineNumber != 0 ? tempLineInfoString : "",
			moduleBase == 0 ? tempDynAddrString : tempConstAddrString,
			moduleBase != 0 ? tempBaseAddrString : "" // if empty, omit
		};

		reportStringParseCache.push_back( stringParseVector );
	}

	SymCleanup( GetCurrentProcess( ) );

	CHAR stackWalkReportLine [ 480 ];

	for ( std::vector<std::string>& stackFrameReportLine : reportStringParseCache )
	{
		memset( stackWalkReportLine, 0, sizeof stackWalkReportLine );
		/* 0: file info
		   1: module info
		   2: line number or ""
		   3: dyn or static addr
		   4: base addr or 0      */

		const std::string& fileInfo = stackFrameReportLine [ 0 ];
		const std::string& moduleInfo = stackFrameReportLine [ 1 ];
		const std::string& lineNumber = stackFrameReportLine [ 2 ];
		const std::string& addrInfo = stackFrameReportLine [ 3 ];
		const std::string& baseAddrInfo = stackFrameReportLine [ 4 ];

		std::string fmtFileInfo( fileInfoStrMax, ' ' );
		std::string fmtModuleInfo( moduleInfoStrMax, ' ' );
		std::string fmtLineNumber( lineNumberStrMax, ' ' );
		std::string fmtAddrInfo( dynAddrStrMax > constAddrStrMax ? dynAddrStrMax : constAddrStrMax, ' ' );
		std::string fmtBaseAddrInfo( baseAddrStrMax, ' ' );

		memcpy( ( void* )fmtFileInfo.c_str( ), fileInfo.c_str( ), fileInfo.size( ) );
		memcpy( ( void* )fmtModuleInfo.c_str( ), moduleInfo.c_str( ), moduleInfo.size( ) );
		memcpy( ( void* )fmtLineNumber.c_str( ), lineNumber.c_str( ), lineNumber.size( ) );
		memcpy( ( void* )fmtAddrInfo.c_str( ), addrInfo.c_str( ), addrInfo.size( ) );
		memcpy( ( void* )fmtBaseAddrInfo.c_str( ), baseAddrInfo.c_str( ), baseAddrInfo.size( ) );

		sprintf_s(
			stackWalkReportLine,
			"%s %s %s %s %s"
			, fmtFileInfo.c_str( )
			, fmtModuleInfo.c_str( )
			, fmtLineNumber.c_str( )
			, fmtAddrInfo.c_str( )
			, fmtBaseAddrInfo.c_str( )
		);

		StackWalkReport << stackWalkReportLine << "\n";
	}

	return StackWalkReport.str( );
}

std::string ExceptionManager::ResolveModuleFromAddress( DWORD Address )
{
	std::string result( "UnkMod" );

	HMODULE hMods [ 1024 ];
	HANDLE hProcess;
	DWORD cbNeeded;

	auto processID = GetCurrentProcessId( );

	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );

	if ( EnumProcessModules( hProcess, hMods, sizeof( hMods ), &cbNeeded ) )
	{
		for ( unsigned m = 0; m < ( cbNeeded / sizeof( HMODULE ) ); m++ )
		{
			MODULEINFO modInfo;
			GetModuleInformation( hProcess, hMods [ m ], &modInfo, sizeof( MODULEINFO ) );
			auto BaseAddress = std::uintptr_t( modInfo.lpBaseOfDll );
			if ( Address > BaseAddress && Address < ( BaseAddress + modInfo.SizeOfImage ) )
			{
				char ModuleName [ MAX_PATH ];
				GetModuleFileNameExA( hProcess, hMods [ m ], ModuleName, MAX_PATH );

				result = getBack( ModuleName, '\\' );
				break;
			}
		}
	}

	CloseHandle( hProcess );

	return result;
}

PCHAR ExceptionManager::GetExceptionSymbol( PEXCEPTION_POINTERS pExceptionRecord )
{
#if (INTPTR_MAX == INT32_MAX)
	PDWORD L0 = ( PDWORD )pExceptionRecord->ExceptionRecord->ExceptionInformation [ 2 ];
	if ( L0 != NULL )
	{
		PDWORD L1 = ( PDWORD )L0 [ 3 ];
		if ( L1 != NULL )
		{
			PDWORD L2 = ( PDWORD )L1 [ 1 ];
			if ( L2 != NULL )
			{
				PCHAR Symbol = ( PCHAR )( L2 [ 1 ] + 8 );
				return Symbol;
			}
		}
	}
	return NULL;
#elif (INTPTR_MAX == INT64_MAX)
	return NULL;
#endif
}

PCHAR ExceptionManager::GetExceptionMessage( PEXCEPTION_POINTERS pExceptionRecord )
{
#if (INTPTR_MAX == INT32_MAX)
	PCHAR Message = NULL;

	PDWORD L0 = ( PDWORD )pExceptionRecord->ExceptionRecord->ExceptionInformation [ 1 ];
	if ( L0 != NULL )
	{
		Message = ( PCHAR )L0 [ 1 ];
		if ( Message == NULL )
		{
			Message = ( PCHAR )L0 [ 3 ];
		}
	}
	return Message;
#elif (INTPTR_MAX == INT64_MAX)
	return NULL;
#endif
}

BOOL ExceptionManager::ExceptionNotify( bool isVEH, PEXCEPTION_POINTERS pExceptionRecord )
{
	auto ExceptionAddress = pExceptionRecord->ExceptionRecord->ExceptionAddress;
	auto ExceptionCode = pExceptionRecord->ExceptionRecord->ExceptionCode;

	PCHAR Symbol = NULL;
	PCHAR Message = NULL;
	CHAR DecodedSymbol [ 512 ];

	if ( ExceptionCode == 0xE06D7363 ) /* C++ exception not caught in top level */
	{
		Symbol = GetExceptionSymbol( pExceptionRecord );
		if ( Symbol != NULL )
		{
#if (INTPTR_MAX == INT32_MAX)
			UnDecorateSymbolName( Symbol + 1, DecodedSymbol, sizeof DecodedSymbol, UNDNAME_NO_ARGUMENTS | UNDNAME_32_BIT_DECODE );
#elif (INTPTR_MAX == INT64_MAX)
			UnDecorateSymbolName( Symbol + 1, DecodedSymbol, sizeof DecodedSymbol, UNDNAME_NO_ARGUMENTS );
#endif
			Symbol = DecodedSymbol;
			if ( std::find( std::begin( g_ehsettings.blacklist_sym ), std::end( g_ehsettings.blacklist_sym ), std::string( Symbol ) ) != std::end( g_ehsettings.blacklist_sym ) )
			{
				return TRUE;
			}
		}
		Message = GetExceptionMessage( pExceptionRecord );
	}
	sprintf_s(
		g_ehsettings.report_dst,
		g_ehsettings.report_dst_size,
		"[Code = 0x%08X]"      "\n"
		"[ExceptSymbol = %s]"  "\n"
		"[Message = %s]"       "\n"
#if (INTPTR_MAX == INT32_MAX)
		"[Eip = 0x%08X]"       "\n"
		"[Eax = 0x%08X]"       "\n"
		"[Ebx = 0x%08X]"       "\n"
		"[Ecx = 0x%08X]"       "\n"
		"[Edx = 0x%08X]"       "\n"
		"[Esi = 0x%08X]"       "\n"
		"[Edi = 0x%08X]"       "\n"
		"[Ebp = 0x%08X]"       "\n"
		"[Esp = 0x%08X]"       "\n"

		"[Dll Base = 0x%08x]"  "\n"
#elif (INTPTR_MAX == INT64_MAX)
		"[Rip = 0x%016llX]"       "\n"
		"[Rax = 0x%016llX]"       "\n"
		"[Rbx = 0x%016llX]"       "\n"
		"[Rcx = 0x%016llX]"       "\n"
		"[Rdx = 0x%016llX]"       "\n"
		"[Rsi = 0x%016llX]"       "\n"
		"[Rdi = 0x%016llX]"       "\n"
		"[Rbp = 0x%016llX]"       "\n"
		"[Rsp = 0x%016llX]"       "\n"

		"[Dll Base = 0x%016llX]"  "\n"
#endif
		"\n"
		"%s"
		, ExceptionCode
		, Symbol ? Symbol : "NoSym"
		, Message ? Message : "NoMsg"
#if (INTPTR_MAX == INT32_MAX)
		, pExceptionRecord->ContextRecord->Eip
		, pExceptionRecord->ContextRecord->Eax
		, pExceptionRecord->ContextRecord->Ebx
		, pExceptionRecord->ContextRecord->Ecx
		, pExceptionRecord->ContextRecord->Edx
		, pExceptionRecord->ContextRecord->Esi
		, pExceptionRecord->ContextRecord->Edi
		, pExceptionRecord->ContextRecord->Ebp
		, pExceptionRecord->ContextRecord->Esp
		, ( DWORD32 )g_ehsettings.prog_base
#elif (INTPTR_MAX == INT64_MAX)
		, pExceptionRecord->ContextRecord->Rip
		, pExceptionRecord->ContextRecord->Rax
		, pExceptionRecord->ContextRecord->Rbx
		, pExceptionRecord->ContextRecord->Rcx
		, pExceptionRecord->ContextRecord->Rdx
		, pExceptionRecord->ContextRecord->Rsi
		, pExceptionRecord->ContextRecord->Rdi
		, pExceptionRecord->ContextRecord->Rbp
		, pExceptionRecord->ContextRecord->Rsp
		, ( DWORD64 )g_ehsettings.prog_base
#endif
		, StackWalkReport( pExceptionRecord ).c_str( )
	);

	g_ehsettings.callback( { g_ehreportbuffer, strlen( g_ehreportbuffer ), strlen( g_ehreportbuffer ) == EH_REPORTSIZE } );

	return FALSE;
}

LONG WINAPI ExceptionManager::TopLevelExceptionHandler( PEXCEPTION_POINTERS pExceptionRecord )
{
	if ( std::find( std::begin( g_ehsettings.blacklist_code ), std::end( g_ehsettings.blacklist_code ), pExceptionRecord->ExceptionRecord->ExceptionCode ) != std::end( g_ehsettings.blacklist_code ) )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	if ( ExceptionNotify( false, pExceptionRecord ) == TRUE ) return EXCEPTION_CONTINUE_SEARCH; // Upon further analysis, this is a non-fatal exception and should be skipped

	exit( 1 );

	return EXCEPTION_NONCONTINUABLE_EXCEPTION;
}

LONG WINAPI ExceptionManager::VectoredExceptionHandler( PEXCEPTION_POINTERS pExceptionRecord )
{
	if ( std::find( std::begin( g_ehsettings.blacklist_code ), std::end( g_ehsettings.blacklist_code ), pExceptionRecord->ExceptionRecord->ExceptionCode ) != std::end( g_ehsettings.blacklist_code ) )
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	if ( ExceptionNotify( true, pExceptionRecord ) == TRUE ) return EXCEPTION_CONTINUE_SEARCH; // Upon further analysis, this is a non-fatal exception and should be skipped

	exit( 1 );

	return EXCEPTION_NONCONTINUABLE_EXCEPTION;
}

void ExceptionManager::Init( EHSettings* settings )
{
	g_ehsettings = *settings;
	if ( g_ehsettings.report_dst == NULL ) g_ehsettings.report_dst = g_ehreportbuffer;
	if ( g_ehsettings.report_dst_size == NULL ) g_ehsettings.report_dst_size = EH_REPORTSIZE;
	if ( g_ehsettings.prog_base != NULL && g_ehsettings.prog_size == NULL )
	{
		/* todo: look into whether MODULEINFO::lpBaseOfDll actually means it has to be a DLL base? */
		MODULEINFO mi;
		GetModuleInformation( GetCurrentProcess( ), ( HMODULE )g_ehsettings.prog_base, &mi, sizeof( MODULEINFO ) );
		g_ehsettings.prog_size = mi.SizeOfImage;
	}
	if ( g_ehsettings.use_seh == true )
	{
		typedef VOID( WINAPI* TopLevelException )( LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter );
		TopLevelException RtlSetUnhandledExceptionFilter = ( TopLevelException )GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "RtlSetUnhandledExceptionFilter" );
		RtlSetUnhandledExceptionFilter( TopLevelExceptionHandler );
	}
	if ( g_ehsettings.use_veh == true )
	{
		typedef VOID( WINAPI* VectoredException )( ULONG First, PVECTORED_EXCEPTION_HANDLER lpVectorExceptionFilter );
		VectoredException RtlAddVectoredExceptionHandler = ( VectoredException )GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "RtlAddVectoredExceptionHandler" );
		RtlAddVectoredExceptionHandler( 0, VectoredExceptionHandler );
	}

	return;
}