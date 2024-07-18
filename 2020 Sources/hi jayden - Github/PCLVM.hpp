#pragma once
#include "Convert.h"


namespace zx3
{
	namespace PCLVM
	{
		class Constants
		{
		public:
			static void ConvertTValue(uintptr_t rL, TValue* V, TValue* R)
			{
				switch (V->tt)
				{
				case LUA_TNIL:
					R->tt = R_LUA_TNIL;
					break;
				case LUA_TNUMBER:
					R->tt = R_LUA_TNUMBER;
					R->value.n = XorDouble(V->value.n);
					break;
				case LUA_TBOOLEAN:
					R->tt = R_LUA_TBOOLEAN;
					R->value.b = V->value.b;
					break;
				case LUA_TSTRING:
				{

					break;
				}


				}
			}
		};
	}
}