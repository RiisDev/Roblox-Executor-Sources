#ifndef _MEM_
#define _MEM_

namespace mem
{
	class c_writeable
	{
		void* const m_addr;
		DWORD m_prot;

	public:
		c_writeable( void* const addr );
		~c_writeable( );
	};
}

#endif