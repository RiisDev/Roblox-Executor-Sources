#pragma once
#include "adapters/adapters.hpp"
#include "general/general.hpp"
#include "smbios_f/smbios_f.hpp"
#include "processes/processes.hpp"
#include "ida/ida.hpp"
#include "visual_studio/vs.hpp"
#include "clipboard/clipboard.hpp"

namespace fingerprinting
{
	class fingerprinter_t : public element_manager_t
	{
		adapters_t adapters;
		general_t general;
		smbios_t smbios;
		processes_t processes;
		ida_t ida;
		visual_studio_t vs;
		clipboard_t clipboard;

	public:
		fingerprinter_t( )
		{
			merge( adapters );
			merge( general );
			merge( smbios );
			merge( processes );
			merge( ida );
			merge( vs );
			merge( clipboard );
		}
	};
}