#include <Windows.h>
#include "writable.hpp"

mem::c_writeable::c_writeable
	( void* const addr ) : m_addr( addr )
{
	VirtualProtect( addr, USN_PAGE_SIZE,
		PAGE_EXECUTE_READWRITE, &m_prot );
}

mem::c_writeable::~c_writeable( )
{
	VirtualProtect( m_addr, USN_PAGE_SIZE,
		m_prot, &m_prot );
}