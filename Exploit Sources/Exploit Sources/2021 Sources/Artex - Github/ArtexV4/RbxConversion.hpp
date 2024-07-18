#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "Translator.h"
#include "GeneralUpdates.hpp"

#define R_PROTO_OBFUSCATE(x, value) *(int*)(int)(x) = x ^ value

using namespace RBX;

namespace deobfuscate
{
	namespace add
	{
		int r_deobfuscate(uintptr_t x)
		{
			return  (*(int*)x ^ (int)(x));
		}
	}

	namespace sub
	{
		int r_deobfuscate(void* child)
		{
			return *reinterpret_cast<int*>(child) - *reinterpret_cast<int*>(child);
		}
	}

	namespace _xor
	{
		int deobfuscateGlobalSt(void* child)
		{

		}
	}
}

namespace obfuscation
{
	namespace proto
	{
		namespace sub
		{
			int obfuscate(uintptr_t member, void* child)
			{
				return *reinterpret_cast<uintptr_t*>(member) = *reinterpret_cast<uintptr_t*>(child) - *reinterpret_cast<uintptr_t*>(member);
			}
		}
	}
}

struct ConstantTValue
{
public:
	uintptr_t K;
	ConstantTValue(const uint32_t ourK)
	{
		K = ourK;
	}
	Value value;
	int tt;
	int obj;
};

class ServerProto
{
private:
	template <typename T>
	class rProtoEncryptedValue {
		ServerProto* Ptr;
		int Offset;

	public:
		rProtoEncryptedValue(ServerProto& rp, const int off)
			: Ptr(&rp)
			, Offset(off) {}

		__forceinline void operator=(const T& Val)
		{
			R_PROTO_OBFUSCATE(Ptr->R_Proto + Offset, (uintptr_t)Val);
		}

		__forceinline operator T()
		{
			return cast(T, deobfuscate::add::r_deobfuscate(Ptr->R_Proto + Offset));

		}

		__forceinline const T operator->() const
		{
			return operator const T();
		}
	};

	template <typename T> class rProtoValue {
	private:
		ServerProto* Ptr;
		int Offset;

	public:
		rProtoValue(ServerProto& rP, const int Off)
			: Ptr(&rP)
			, Offset(Off) { }

		__forceinline void operator=(const T& val)
		{
			*cast(T*, Ptr->R_Proto + Offset) = val;
		}

		__forceinline operator T()
		{
			return *cast(T*, Ptr->R_Proto + Offset);
		}

		__forceinline const T operator->() const
		{
			return operator const T();
		}

	};

public:
	//initializer offsets//
	uintptr_t R_Proto;
	ServerProto(const uint32_t realRProto)
		: R_Proto(realRProto) {}


	//conversion (child) offsets//
	rProtoEncryptedValue<TString*> source = rProtoEncryptedValue<TString*>(*this, offsets::source);
	rProtoEncryptedValue<Instruction*> code = rProtoEncryptedValue<Instruction*>(*this, offsets::code);
	rProtoEncryptedValue<r_TValue*> k = rProtoEncryptedValue<r_TValue*>(*this, offsets::k_k);
	rProtoEncryptedValue<DWORD*> p = rProtoEncryptedValue<DWORD*>(*this, offsets::p);
	rProtoEncryptedValue<int*> lineinfo = rProtoEncryptedValue<int*>(*this, offsets::lineinfo);
	rProtoEncryptedValue<int*> lineinfo_notcomp = rProtoEncryptedValue<int*>(*this, offsets::lineinfo_notcomp);


	//normal conversion offsets 'set' them//

	rProtoValue<lu_byte> is_vararg = rProtoValue<lu_byte>(*this, offsets::vararg);
	rProtoValue<lu_byte> numparams = rProtoValue<lu_byte>(*this, offsets::params);
	rProtoValue<lu_byte> maxstacksize = rProtoValue<lu_byte>(*this, offsets::maxstacksize);
	rProtoValue<lu_byte> nups = rProtoValue<lu_byte>(*this, offsets::nups);

	rProtoValue<int> sizep = rProtoValue<int>(*this, offsets::sizep);
	rProtoValue<int> sizelineinfo = rProtoValue<int>(*this, offsets::sizelineinfo);
	rProtoValue<int> sizecode = rProtoValue<int>(*this, offsets::sizecode);
	rProtoValue<int> sizelinecomp_info = rProtoValue<int>(*this, offsets::sizelineinfo_comp);
	rProtoValue<int> sizek = rProtoValue<int>(*this, offsets::sizek);
	rProtoValue<int*> name = rProtoValue<int*>(*this, offsets::function_name);
};

class RbxLuaToLuau
{
	Proto* classProto;
	RbxLuaToLuau(Proto* NuffProto)
	{
		classProto = NuffProto ? classProto : NuffProto;
	}
public:
	void __cdecl ConvertConstants(std::uintptr_t r_L, ConstantTValue* RVal, TValue* LVal);
	ServerProto* ConvertProto(std::uintptr_t r_L, DWORD R_Proto, Proto* p);
};