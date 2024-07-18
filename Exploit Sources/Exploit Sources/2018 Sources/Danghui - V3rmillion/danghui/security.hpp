#pragma once

#include "globals.hpp"
#include <istream>
#include <ostream>
#include <sstream>
#include "Libraries/VMP/VMProtectSDK.h"
#include "Libraries/xxTea/xxtea.h"

extern DWORD danghui_security_scriptcontextvf;
typedef char* STRING;

#define DH_XXTEAKEY_CLIENT_KEY "Nlg6LJwaKDY3jgUvwOD735DWJnLrzEQp"
#define DH_XXTEAKEY_SERVER_KEY "C912EUvwFCMWcudf0L4UWPXgDSCFfWmp"
#define DH_HOSTURL (VMProtectDecryptStringA("http://louka.io/danghui/hst.php"))
#define DH_HEADER "!DANGHUI!"

struct PACKED_STRING
{
	STRING String;
	size_t StringLength;
};