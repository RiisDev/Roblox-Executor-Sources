/*
	HOSTED+
	Hosted+ is the superior address host, that has 200 addresses!
	
	Credits:
	mellonyt#1488 for giving me the addresses weekly, and allowing me to make this service with his addresses.
*/

#include <string>
#include <functional>

#pragma comment(lib, "libcurl_a.lib")

#include "cURL/curl.h"
#include "JSON/json.h"

#define aslr(x) (x - 0x400000 + (DWORD)GetModuleHandle(0))
#define noaslr(x) (x + 0x400000 - (DWORD)GetModuleHandle(0))
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, -10002, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)

int R_LUA_TNIL, 
				R_LUA_TLIGHTUSERDATA, 
				R_LUA_TNUMBER, 
				R_LUA_TBOOLEAN, 
				R_LUA_TSTRING, 
				R_LUA_TTHREAD, 
				R_LUA_TFUNCTION, 
				R_LUA_TTABLE, 
				R_LUA_TUSERDATA, 
				R_LUA_TPROTO, 
				R_LUA_TUPVALUE;

uintptr_t State;
//U can with the format std::function<(typedef),(args)> addrname;

namespace {
    std::size_t callback(
        const char* in,
        std::size_t size,
        std::size_t num,
        std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}

namespace HostedPlus {
	std::string url = "http://hostedplus.ga";
	std::string key = "PUT YOUR KEY HERE";
	Json::Value json;
	struct addresS {
		uintptr_t address;
		std::string callingconv;
		bool retcheck;
		bool isconflict;
		std::string aob;
		std::string mask;
		std::string typedefs;
	};
	
	void Load();
	
	template<class Ret, class... Args>
	std::function<Ret(Args...)> Call(DWORD Address, std::string Convention)
	{
		if (Convention == "__stdcall")
		{
			typedef Ret(__stdcall* Fn_t)(Args...);
			return (Fn_t)Address;
		}
		else if (Convention == "__cdecl")
		{
			typedef Ret(__cdecl* Fn_t)(Args...);
			return (Fn_t)Address;
		}
		else if (Convention == "__fastcall")
		{
			typedef Ret(__fastcall* Fn_t)(Args...);
			return (Fn_t)Address;
		}
		else if (Convention == "__thiscall")
		{
			typedef Ret(__thiscall* Fn_t)(Args...);
			return (Fn_t)Address;
		}
	}
				
	std::string ReadURL(const char* site)
	{
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, site);
		curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		int httpCode(0);
		std::unique_ptr<std::string> httpData(new std::string());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
		curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		curl_easy_cleanup(curl);
		if (httpCode == 200)
		{
			return *httpData;
		}		
		else
		{
			return "";
		}
	}
		
	Json::Value Get(const char* url)
	{
		std::string returned = ReadURL(url);
		if(returned != "")
		{
			const auto rawlength = static_cast<int>((returned).length());
			JSONCPP_STRING err;
			Json::Value Root;
			Json::CharReaderBuilder builder;
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			if (!reader->parse((returned).c_str(), returned.c_str() + rawlength, &Root,&err))
			{
				return EXIT_FAILURE;
			}
			return Root;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
		
	bool run()
	{
		url =  url;
		url += "/grab.php?key=";
		url += key;
		Json::Value got = Get(url.c_str());
		if(got != EXIT_FAILURE)
		{
			HostedPlus::json = got;
			HostedPlus::Load();
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool check()
	{
		if (json <= EXIT_FAILURE)
			return run();
		return true;
	}
		
#include "../Console.h"

	void HostedDumper()
	{
		if (check())
		{
			Json::Value Addresses = json["addresses"];
			for (auto const& id : Addresses.getMemberNames()) {
				std::cout << iyellow << id << ": " << iaqua << "0x" << std::uppercase << std::hex << Addresses[id.c_str()]["addr"].asInt() << " " << iwhite << Addresses[id.c_str()]["cc"].asString() << std::endl;
			}
		}
	}

	template<class Ret, class... Args>
	auto CreateDef(std::string FuncName)
	{//u need to add your own retcheck bypass to use this
		if (check()) {
			if(json["addresses"][FuncName.c_str()]["retcheck"] == true)
			{
				//return Call<Ret, Args...>(unprotect(aslr(json["addresses"][FuncName.c_str()]["addr"].asInt())), JSON["addresses"][FuncName.c_str()]["cc"].asString().c_str());
			}
			else
			{
				//return Call<Ret, Args...>(aslr(json["addresses"][FuncName.c_str()]["addr"].asInt())), json["addresses"][FuncName.c_str()]["cc"].asString().c_str();
			}
		}
	}
		
	auto GetRobloxState()
	{
		if (check())
		{
			volatile uintptr_t StackPad[16]{};
			static uintptr_t DMPad[16]{};
			auto GetDataModel = HostedPlus::Call<uintptr_t, uintptr_t, uintptr_t>(aslr(json["addresses"]["getdatamodel"]["addr"].asInt()), "__thiscall");
			auto GetAddress = HostedPlus::Call<uintptr_t>(aslr(json["addresses"]["getdatamodel2"]["addr"].asInt()), "__cdecl");
			GetDataModel(GetAddress(), (uintptr_t)DMPad);
			uintptr_t DM = DMPad[0];
			DM = DM + 0xC;
			auto FindFirstChildOfClass = HostedPlus::Call<uintptr_t, uintptr_t, uintptr_t, const std::string&>(aslr(json["addresses"]["findfirstchildofclass"]["addr"].asInt()), "__thiscall");
			static DWORD SC;
			FindFirstChildOfClass(DM, (DWORD)&SC, "ScriptContext");
			std::string Symbol = json["luastate"]["extra"]["symbol"].asString();
			uintptr_t StateOffset = json["luastate"]["extra"]["scoffset"].asInt();
			if (Symbol == "^")
			{
				return *(DWORD*)(SC + StateOffset) ^ (SC + StateOffset);
			}
			else if (Symbol == "*")
			{
				return *(DWORD*)(SC + StateOffset) * (SC + StateOffset);
			}
			else if (Symbol == "-")
			{
				return *(DWORD*)(SC + StateOffset) - (SC + StateOffset);
			}
			else if (Symbol == "+")
			{
				return *(DWORD*)(SC + StateOffset) + (SC + StateOffset);
			}
			else if (Symbol == "/")
			{
				return *(DWORD*)(SC + StateOffset) / (SC + StateOffset);
			}
			else
			{
				return 0;
			}
			return 1;
		}
	}
	
	//Credits to mellon for the type scanner
	void GetTypes()
	{
		auto type = HostedPlus::CreateDef<int,uintptr_t,int>("lua_type");
		auto getfield = HostedPlus::CreateDef<int,uintptr_t,int,const char*>("lua_getfield");
		auto settop = HostedPlus::CreateDef<int,uintptr_t,int>("lua_settop");
		auto newthread = HostedPlus::CreateDef<int,uintptr_t>("lua_newthread");
		auto pushstring = HostedPlus::CreateDef<int,uintptr_t,const char*>("lua_pushstring");
		auto pushbool = HostedPlus::CreateDef<int,uintptr_t,int>("lua_pushboolean");
		auto pushlightuserdata = HostedPlus::CreateDef<int,uintptr_t,int>("lua_pushlightuserdata");

		getfield(State, -10002, "print");
		R_LUA_TFUNCTION = type(State, -1);
		settop(State, -(-1) - 1);

		pushstring(State, "Lmao");
		R_LUA_TSTRING = type(State, -1);
		settop(State, -(1) - 1);

		pushboolean(State, true);
		R_LUA_TBOOLEAN = type(State, -1);
		settop(State, -(1) - 1);

		getfield(State, -10002, "game");
		R_LUA_TUSERDATA = type(State, -1);
		settop(State, -(1) - 1);

		getfield(State, -10002, "jfddjdsjfdo.......sdijo");
		R_LUA_TNIL = type(State, -1);
		settop(State, -(1) - 1);

		getfield(State, -10002,"table");
		R_LUA_TTABLE = type(State, -1);
		settop(State, -(1) - 1);

		getfield(State, -10002, "workspace");
		r_lua_getfield(State, -1, "Gravity");
		R_LUA_TNUMBER = type(State, -1);
		settop(State, -(1) - 1);

		newthread(State);
		R_LUA_TTHREAD = type(State, -1);
		settop(State, -(1) - 1);

		pushlightuserdata(State, nullptr);
		R_LUA_TLIGHTUSERDATA = type(State, -1);
		settop(State, -(1) - 1);

		//big meme :kekw:

		R_LUA_TPROTO = 10;
		R_LUA_TUPVALUE = 11;
	}
		
	void Load()
	{
		GetTypes();
		//blablalbalblalalbla
	}
}