using namespace Wrapper;

#include "ThreadManager.h"
#include <thread>

#define r_setidentity(rL, i) DWORD* identity  = (DWORD*)(rL - 40); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;


static TValue *index2adr(lua_State *L, int idx) {
	if (idx > 0) {
		TValue *o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue *, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure *func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure *func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue *, luaO_nilobject);
	}
	}
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}



void ExecuteScript(std::string Script) {
	Script = "spawn(function() Instance.new'LocalScript'\r\n" + Script + "\r\nend)";
	luaL_dostring(LS, Script.c_str());
}



using Wrapper::RLS;
using Wrapper::LS;

void WrapGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Wrapper::Wrap(rL, L, -1);
	lua_setglobal(L, s);
}



int custoLSoadstring(lua_State* L) {
	if (lua_type(L, -1) != LUA_TSTRING) {
		luaL_error(L, "(Bad Argument) - 'loadstring' only accepts 1 argument (string).");
		return 0;
	}
	if (lua_gettop(L) == 0) {
		luaL_error(L, "'loadstring' needs 1 string argument.");
		return 0;
	}
	ExecuteScript(std::string(lua_tostring(L, -1)));
	return 0;
}


struct i_settings {
	int bufferLength = 8;
} globalSettings;

int setBuffLength(lua_State *L) {
	globalSettings.bufferLength = lua_tonumber(L, -1);
	return 1;
}

using std::string;

#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename);
SOCKET connectToServer(char *szServerName, WORD portNum);
int getHeaderLength(char *content);
char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);


int r_httpget(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 1;

	bool trueorfalse = lua_toboolean(LS, -2);
	if (trueorfalse == true) {
		printf("\nCustomHttpGet: true");
	}
	if (trueorfalse == false) {
		printf("\nCustomHttpGet: false");
		return 1;
	}

	const int bufLen = 1024;
	const char *szUrl = lua_tostring(L, 1);
	long fileSize;
	char *memBuffer, *headerBuffer;
	FILE *fp;
	memBuffer = headerBuffer = NULL;
	if (WSAStartup(0x101, &wsaData) != 0)
		return -1;
	memBuffer = readUrl2((char *)szUrl, fileSize, &headerBuffer);
	lua_pushstring(L, memBuffer);
	if (fileSize != 0)
	{
		fp = fopen("downloaded.file", "wb");
		fwrite(memBuffer, 1, fileSize, fp);
		fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}
	WSACleanup();
	return 1;
}

void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	if (url.substr(0, 8) == "https://")
		url.erase(0, 8);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}

SOCKET connectToServer(char *szServerName, WORD portNum)
{
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}

int getHeaderLength(char *content)
{
	const char *srchStr1 = "\r\n\r\n", *srchStr2 = "\n\r\n\r";
	char *findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}

char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char *tmpResult = NULL, *result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;
	mParseUrl(szUrl, server, filepath, filename);
	conn = connectToServer((char*)server.c_str(), 80);
	sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);
	totalBytesRead = 0;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = getHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char *myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	return(result);
}
//HTTPGET