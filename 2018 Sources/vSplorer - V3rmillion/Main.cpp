#include <Windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <algorithm>
#include <istream>
#include <sstream>
#include <windowsx.h>
#include <WinINet.h>
#include "memwatch.h"
#include <Uxtheme.h>
#include <dirent.h>
#include <cctype>
#include <ostream>
#include <istream>
#include <fstream>
#include <streambuf>

#pragma comment(lib, "UxTheme")
#pragma comment(lib, "comctl32")
#pragma comment(lib, "Wininet.lib")


#define format(x) (x - 0x400000 + (DWORD)GetModuleHandle(NULL))


DWORD WINAPI ThreadProc();
HWND treeviewi = 0;
HWND listviewi = 0;
HTREEITEM g_root = 0;
HIMAGELIST hImageList;
HBITMAP hBitMap;
HBITMAP hBitMap2;
HMENU m_hMenu = CreatePopupMenu();
HMENU CallMethodMenu = CreateMenu();
HWND refHwnd = 0;
HWND IDDMAINHWND = 0;

HIMAGELIST eehiml;  // handle to image list 
HBITMAP eehbmp;     // handle to bitmap 
HBITMAP bgBitMap;
#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

#define DLG_MAIN                                100
#define IDR_MENU1                               106
#define IDD_DIALOG1                             107
#define IDD_DIALOG2                             108
#define IDD_DIALOG3                             109


HINSTANCE hInst;
#define IDM_DESTROY 200
#define IDM_CLONE 201
#define IDM_CUT 202
#define IDM_PASTE 203
#define IDM_INSERTOBJ 204
#define IDM_REFRESH 205
#define IDM_CALLMETHOD 206
#define IDM_TPTO 207
#define IDM_CALLREMOTE 208


BOOL CALLBACK IDDMAIN(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK IDD2MAIN(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int rstate = 0;
std::vector<int> InstanceKEY;


#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8
#define rbxttisstring(o) (ttype(o) == RLUA_TSTRING)


#define adr_ScriptContext 0x100c93c
#define addr_lua_getfield 0x72a510
#define addr_lua_setfield 0x72c270
#define addr_luaV_settable 0x7298e0
#define addr_lua_pushvalue 0x72B970
#define addr_lua_pcall 0x72b2f0
#define addr_lua_resume 0x72c030
#define addr_lua_rawget 0x72bad0
#define addr_lua_pushstring 0x72b8b0
#define addr_lua_remove 0x72be80
#define addr_lua_setfenv 0x72c160
#define addr_lua_pushboolean 0x72b3c0
#define addr_luaL_where 0x725f90
#define addr_lua_touserdata 0x72cca0
#define addr_lua_topointer 0x72cbf0
#define addr_lua_tolstring 0x72c890
#define addr_lua_getmetatable 0x72a7e0
#define addr_luaL_getmetafield 0x725810
#define addr_lua_setmetatable 0x72c420
#define addr_lua_pushnumber 0x72b820
#define addr_lua_settop 0x72c650
#define addr_lua_type 0x72ccd0
#define addr_luaS_newlstr 0x7266a0
#define addr_lua_checkstack 0x729f10
#define addr_luaL_ref 0x725CB0
#define addr_lua_rawgeti 0x72bb80
#define addr_lua_objlen 0x72b200
#define addr_lua_tonumber 0x72cb70
#define addr_lua_toboolean 0x72c7d0
#define addr_lua_createtable 0x72A130
#define addr_lua_next 0x72b170
#define addr_lua_gc 0x72A2E0



#define addr_luaL_error 0x750310 //not updated

#define addr_lua_newthread 0x72AFC0 //updated
#define addr_lua_pushcclosure 0x72B440 //updated

#define addr_luaD_growstack 0x74D470




typedef const char*(__cdecl *rbx_tls)(int a1, int a2, int a3);
rbx_tls rlua_tolstring = (rbx_tls)format(addr_lua_tolstring); //

typedef int(__cdecl *gf)(DWORD a, DWORD b, const char* c);
gf rlua_getfield = (gf)format(addr_lua_getfield); //


typedef int(__cdecl *pca)(DWORD a, DWORD b, DWORD c, DWORD d);
pca rlua_pcall = (pca)format(addr_lua_pcall); //

typedef int(__cdecl *ps)(DWORD st, const char* str);
ps rlua_pushstring = (ps)format(addr_lua_pushstring); //

typedef int(__cdecl *pn)(DWORD st, double n);
pn rlua_pushnumber = (pn)format(addr_lua_pushnumber);

typedef int(__cdecl *pb)(DWORD st, int n);
pb rlua_pushboolean = (pb)format(addr_lua_pushboolean);

typedef int(__cdecl *rsetfield)(int a, int idx, const char* field);
rsetfield rlua_setfield = (rsetfield)format(addr_lua_setfield);

typedef int(__cdecl *pv)(int st, int idx);
pv rlua_pushvalue = (pv)format(addr_lua_pushvalue);

typedef int(__cdecl *rty)(int st, signed int idx);
rty rlua_type = (rty)format(addr_lua_type);


typedef int(__cdecl *smt)(int st, int idx);
smt rlua_setmetatable = (smt)format(addr_lua_setmetatable);

typedef int(__cdecl *stt)(int st, int idx);
stt rlua_settop = (stt)format(addr_lua_settop);

typedef int(__cdecl *rlnth)(int st);
rlnth rlua_newthread = (rlnth)format(addr_lua_newthread);

typedef int(__cdecl *rtud)(int st, int idx);
rtud rlua_touserdata = (rtud)format(addr_lua_touserdata);

typedef double(__cdecl *rtnn)(int st, int idx);
rtnn rlua_tonumber = (rtnn)format(addr_lua_tonumber);

typedef double(__cdecl *cstk)(int st, int idx);
cstk rlua_checkstack = (cstk)format(addr_lua_checkstack);

typedef bool(__cdecl *tbol)(int st, int idx);
tbol rlua_toboolean = (tbol)format(addr_lua_toboolean);

typedef int(__cdecl *rgmt)(int st, int idx);
rgmt rlua_getmetatable = (rgmt)format(addr_lua_getmetatable);

typedef int(__cdecl *luadgs)(int st, int idx);
luadgs rluaD_growstack = (luadgs)format(addr_luaD_growstack);

int rw_checkstack(int st, int sz){
//if (*(DWORD *)(st + 32) - *(DWORD *)(st + 16) <= 16 * sz)
//return rluaD_growstack(st, sz);
return 1;
}





HTREEITEM InsertRootNode(HWND hWnd, LPSTR txt, int picidx){
TV_INSERTSTRUCT tvins;
TV_ITEM tvi;
if (picidx > -1){
tvi.mask = TVIF_TEXT | TVIF_IMAGE;
tvi.pszText = txt;
tvi.iImage = picidx;
}
else{
tvi.mask = TVIF_TEXT;
tvi.pszText = txt;
tvi.iImage = picidx;
tvi.iSelectedImage = picidx;
}

tvins.hParent = TVI_ROOT;
tvins.hInsertAfter = TVI_FIRST;
tvins.item = tvi;

return(TreeView_InsertItem(hWnd, &tvins));
}


int g_nBird, g_nTree;

#define CX_ICON  32 
#define CY_ICON  32 
#define NUM_ICONS 3 

std::vector<std::string> instname; //might be multiple of the same vector but i got lazy
//std::vector<int> instid

;
HIMAGELIST AddIconsToImageList(HINSTANCE hinst)
{
HIMAGELIST himlIcons;  // handle to new image list 
HICON hicon;           // handle to icon 

// Ensure that the common control DLL is loaded. 
InitCommonControls();

// Create a masked image list large enough to hold the icons. 
himlIcons = ImageList_Create(CX_ICON, CY_ICON, ILC_MASK, NUM_ICONS, 0);

// Load the icon resources, and add the icons to the image list. 

hicon = LoadIcon(hinst, MAKEINTRESOURCE("BasePart.ico"));
g_nBird = ImageList_AddIcon(himlIcons, hicon);

return himlIcons;
}


template<typename Out>
void split(const std::string &s, char delim, Out result) {
std::stringstream ss;
ss.str(s);
std::string item;
while (std::getline(ss, item, delim)) {
*(result++) = item;
}
}


std::vector<std::string> split(const std::string &s, char delim) {
std::vector<std::string> elems;
split(s, delim, std::back_inserter(elems));
return elems;
}

std::string dirPath() {
char buffer[2048];
GetModuleFileNameA(GetModuleHandle("vSplorer.dll"), buffer, 2048);
std::string::size_type pos = std::string(buffer).find_last_of("\\/");
return std::string(buffer).substr(0, pos);
}

namespace imgs{
std::vector<int> imgid;
std::vector<std::string> imgname;

int addImage(std::string name){
std::string theimg = dirPath() + "\\bin\\" + name;

HBITMAP hbmp = (HBITMAP)LoadImage(0, theimg.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
int result = ImageList_Add(eehiml, hbmp, (HBITMAP)NULL);
DeleteObject(hbmp);

imgname.push_back(name);
imgid.push_back(result);
return result;
}

void LoadAllImages(){
std::string loaddir = dirPath() + "\\bin\\";
DIR *dir;  //gets all files  - skidded from stackoverflow
struct dirent *ent;
if ((dir = opendir(loaddir.c_str())) != NULL) {
/* print all the files and directories within directory */
while ((ent = readdir(dir)) != NULL) {
std::string hec = ent->d_name;
if (strlen(hec.c_str()) > 3){
//MessageBoxA(0, hec.c_str(), "hec", 0);
addImage(hec);
}
}
closedir(dir);
}
else {
/* could not open directory */
perror("");
//return EXIT_FAILURE;
}
}

int partid;
int noneid;
int workspaceid;
int scriptid;
int humanoidid;
int attachmentid;
int terrainid;
int toolid;
int modelid;
int valueid;
int shirtid;
int pantsid;
int soundid;
int instanceid;
}


HTREEITEM InsertChildNode(HWND hWnd, LPSTR txt, HTREEITEM par, int imgidx){
TV_INSERTSTRUCT tvins;
TV_ITEM tvi;

if (imgidx > -1){
tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
tvi.pszText = txt;
tvi.iImage = imgidx;
tvi.iSelectedImage = imgidx;
}
else{
tvi.mask = TVIF_TEXT;
tvi.pszText = txt;
tvi.iImage = imgidx;
tvi.iSelectedImage = imgidx;
}

tvins.hParent = par;
tvins.hInsertAfter = TVI_FIRST;
tvins.item = tvi;


return(TreeView_InsertItem(hWnd, &tvins));
}




int ScriptContext = 0;
int Workspace = 0;

class Instance{
public:
std::string* getName(int Instance) {
if (*(DWORD*)(Instance)){
if (*(int*)(Instance + 0x28)){
return (std::string*)(*(int*)(Instance + 0x28));
}
else{
//MessageBoxA(0, "error with while getting name", "Error", 0);
}
}
else{
printf("Error occured with getname\n");
return (std::string*)("Error->Instance");
}
}

std::string* getClass(int Self) {
return (std::string*)(*(int(**)(void))(*(int*)Self + 16))();
}

};

namespace Roblox{

std::vector<int> vecidxitm;

std::vector<int> GetChildren(int instance){
std::vector<int> result;
if (*(DWORD*)instance){
if (*(DWORD*)instance + 0x2C){
DWORD StartOfChildren = *(DWORD*)(instance + 0x2C); //TODO: Add Stabability
DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
{
result.push_back(*(int*)i);
}
}
else{
MessageBoxA(0, "No Children detected", "Warning", MB_ICONWARNING);
}
}
return result;
}

HTREEITEM treeret = g_root;
int idxitm = 0;
bool refresh_ready = true;
int getAll(int instance, HTREEITEM trit){
if (instance){
std::vector<int> Children = GetChildren(instance);
Instance cls_inst;
refresh_ready = false;
for (int i = 0; i < Children.size(); i++){
Sleep(1);
int inst = Children[i];
printf("Retrieving Name\n");
const char* iname = cls_inst.getName(inst)->c_str();
printf("got Name\n");
const char* clname = cls_inst.getClass(inst)->c_str();
printf("got ClassName\n");
std::string siname = iname;
std::string sclname = clname;
std::string cmbname = siname; //+ "                                                      [#" + std::to_string(idxitm) + "]";
printf("Pushing %d into vecidxitm\n", idxitm);
vecidxitm.push_back(idxitm);
printf("Pushing %s into instname\n", siname.c_str());
instname.push_back(siname);
printf("Pushing Instance\n");
InstanceKEY.push_back(inst);


idxitm += 1;
HTREEITEM chres;
///0 = part
//1 = none
//2 = script
if (std::find(imgs::imgname.begin(), imgs::imgname.end(), sclname + ".bmp") != imgs::imgname.end())
{
int res = 0;
for (int i = 0; i < imgs::imgname.size(); i++){
std::string sres = imgs::imgname[i];
if (sres == sclname + ".bmp"){
res = imgs::imgid[i];
}
}
chres = InsertChildNode(treeviewi, (LPSTR)cmbname.c_str(), trit, res);
// Element in vector.
}
else if (sclname.find("Value") != std::string::npos){
chres = InsertChildNode(treeviewi, (LPSTR)cmbname.c_str(), trit, imgs::valueid);
}
else{
chres = InsertChildNode(treeviewi, (LPSTR)cmbname.c_str(), trit, imgs::instanceid);
}




if (*(DWORD*)(inst)){
if (*(DWORD*)(inst + 0x2C)){
printf("has children\n");
treeret = TreeView_GetChild(treeviewi, chres);;
//printf("Found Children for Instance: %s\n", iname);
getAll(inst, chres);
}
else{

}
}

}
}
return 0;
}


}


namespace Memorys {
bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
{
for (; *szMask; ++szMask, ++pData, ++bMask)
if (*szMask == 'x' && *pData != *bMask) return 0;
return (*szMask) == NULL;
}


DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask){
for (DWORD i = 0; i<dwLen; i++)
if (Compare((BYTE*)(dwAddress + i), bMask, szMask))  return (DWORD)(dwAddress + i);
return 0;
}

int Scan(DWORD mode, char* content, char* mask)
{
DWORD PageSize;
SYSTEM_INFO si;
GetSystemInfo(&si);
PageSize = si.dwPageSize;
MEMORY_BASIC_INFORMATION mi;
for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
{
DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
if (mi.Type == MEM_MAPPED) continue;
if (mi.Protect == mode)
{
int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
if (addr != 0)
{
return addr;
}
}
}
}


}

namespace parsing{
std::string Replace(std::string subject, const std::string& search,
const std::string& replace) {
size_t pos = 0;
while ((pos = subject.find(search, pos)) != std::string::npos) {
subject.replace(pos, search.length(), replace);
pos += replace.length();
}
return subject;
}


std::string DownloadString(std::string URL) {

HINTERNET interwebs = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
HINTERNET urlFile;
std::string rtn;
if (interwebs) {
urlFile = InternetOpenUrl(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
if (urlFile) {
char buffer[2000];
DWORD bytesRead;
do {
InternetReadFile(urlFile, buffer, 2000, &bytesRead);
rtn.append(buffer, bytesRead);
memset(buffer, 0, 2000);
} while (bytesRead);
InternetCloseHandle(interwebs);
InternetCloseHandle(urlFile);
std::string p = Replace(rtn, "|n", "\r\n");
return p;
}
}
}

std::string get_str_between_two_str(const std::string &s,    //skidded off stackoverflow
const std::string &start_delim,
const std::string &stop_delim)
{
unsigned first_delim_pos = s.find(start_delim);
unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
unsigned last_delim_pos = s.find(stop_delim);

return s.substr(end_pos_of_first_delim,
last_delim_pos - end_pos_of_first_delim);
}


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
if (from.empty())
return;
size_t start_pos = 0;
while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
str.replace(start_pos, from.length(), to);
start_pos += to.length();
}
}

std::string parsePropName(std::string src){
src = get_str_between_two_str(src, "SectionProperties\">", "sectionFunctions\">");
std::string result = "";
std::istringstream f(src);
std::string line;

while (std::getline(f, line)) {
if (line.find("col-name") != std::string::npos){
line = get_str_between_two_str(line, "\">", "</a>");
line = get_str_between_two_str(line, "#member", "\">");
if (line.find(">") == std::string::npos){
result += line + "\n";
}
}
}
return result;
}

bool firstUpper(const std::string& word) { return word.size() && isupper(word[0]); }

std::string argtype = "";
std::string parseFuncName(std::string src){
src = get_str_between_two_str(src, "sectionFunctions", "sectionYieldFunctions");
std::string result = "";
std::istringstream f(src);
std::string line;

while (std::getline(f, line)) {

if (line.find("col-return-type") != std::string::npos){
argtype = get_str_between_two_str(line, "/api/type/", ".html");
}
if (line.find("col-name") != std::string::npos && argtype == "void"){
std::string argcheck = get_str_between_two_str(line, "api-arguments\">", "</span>");
if (argcheck == "( )"){
line = get_str_between_two_str(line, "\">", "</a>");
line = get_str_between_two_str(line, "#member", "\">");
if (line.find(">") == std::string::npos && firstUpper(line)){
result += line + "\n";
}
}
}
}
return result;
}
}



#define adr_Workspace 0xFF57B4

void NewListItem(LPSTR val);
void AddObject(HWND listView, const char* Name, const char* Value, bool readOnly);




std::vector<std::string> proplistidstr;
std::vector<int> proplistid;
int globali = 0;


void reverseVectorProp(){
std::vector<std::string> n_v(proplistidstr.rbegin(), proplistidstr.rend());
proplistidstr.clear();
for (int i = 0; i < n_v.size(); i++){
printf("Indexing item: %s", n_v[i].c_str());
proplistidstr.push_back(n_v[i]);
}
}

int CheckProperty(int state, std::string prop){
MemoryWatch_Start();
rlua_getfield(state, -10002, "g_obj");
try{
rlua_getfield(state, -1, prop.c_str());
int typ = rlua_type(state, -1);
printf("Getting type of: %d\n", typ);
switch (typ){
case RLUA_TSTRING:{
std::string str = rlua_tolstring(state, -1, NULL);
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), str.c_str(), false);
globali += 1;
//NewListItem(0, 0, (LPSTR)prop.c_str(), (LPSTR)str.c_str());
break;
}
case RLUA_TNUMBER:{
int num = rlua_tonumber(state, -1);
std::string str = std::to_string(num);
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), str.c_str(), false);
globali += 1;
//NewListItem(0, 0, (LPSTR)prop.c_str(), (LPSTR)str.c_str());
break;
}

case RLUA_TBOOLEAN:{
std::string str;
bool num = rlua_toboolean(state, -1);
if (num == false){
str = "false";
}
else{
str = "true";
}
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), str.c_str(), false);
globali += 1;
//NewListItem(0, 0, (LPSTR)prop.c_str(), (LPSTR)str.c_str());
break;
}

case RLUA_TUSERDATA: {
rlua_getfield(rstate, -10002, "typeof");
rlua_pushvalue(rstate, -2);
rlua_pcall(rstate, 1, 1, 0);
std::string typof = rlua_tolstring(rstate, -1, NULL);
if (typof == "Vector3"){

rlua_getfield(rstate, -10002, "g_obj"); //X
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "X");
double numx = rlua_tonumber(rstate, -1);

rlua_getfield(rstate, -10002, "g_obj"); //Y
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "Y");
double numy = rlua_tonumber(rstate, -1);

rlua_getfield(rstate, -10002, "g_obj"); //Z
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "Z");
double numz = rlua_tonumber(rstate, -1);

std::string res = std::to_string(numx) + "," + std::to_string(numy) + "," + std::to_string(numz);
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), res.c_str(), false);
globali += 1;

}

else if (typof == "CFrame"){

rlua_getfield(rstate, -10002, "g_obj"); //X
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "X");
double numx = rlua_tonumber(rstate, -1);

rlua_getfield(rstate, -10002, "g_obj"); //Y
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "Y");
double numy = rlua_tonumber(rstate, -1);

rlua_getfield(rstate, -10002, "g_obj"); //Z
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "Z");
double numz = rlua_tonumber(rstate, -1);

std::string res = std::to_string(numx) + "," + std::to_string(numy) + "," + std::to_string(numz);
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), res.c_str(), false);
globali += 1;

}


else if (typof == "BrickColor" || typof == "EnumItem"){
rlua_getfield(rstate, -10002, "g_obj");
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "Name");
std::string res = rlua_tolstring(rstate, -1, NULL);
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), res.c_str(), false);
globali += 1;

}

else if (typof == "Instance"){
rlua_getfield(rstate, -10002, "g_obj"); //X
rlua_getfield(rstate, -1, prop.c_str());
rlua_getfield(rstate, -1, "Name");
std::string res = rlua_tolstring(rstate, -1, NULL);
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), res.c_str(), true);
globali += 1;

}

break;
}

case RLUA_TNIL: //if its NIL
printf("Property is NIL\n");
break;


default:
printf("Not implemented\n");
std::string str = "N/A";
proplistidstr.push_back(prop);
proplistid.push_back(globali);
AddObject(listviewi, prop.c_str(), str.c_str(), false);
globali += 1;
break;
}
}
catch (std::exception e){
printf("Error: %s\n", e.what());
}

MemoryWatch_End();
return 1;
}


DWORD GetLuaState(DWORD SC){
int ls = *(DWORD *)(SC + 56 * 1 + 164) - (SC + 56 * 1 + 164);
return ls;
}




#define rgetglobal(s,i) (rlua_getfield(s, -10002, i))

#define f_index(s,i) (rlua_getfield(s, -1, i))

void InstanceNew(int state, const char* instance, bool ret){
if (ret == true){
MemoryWatch_Start();
}
rgetglobal(state, "Instance");
f_index(state, "new");
rlua_pushstring(state, instance);
rlua_pushvalue(state, -4);
rlua_pcall(state, 2, 1, 0);
if (ret == true){
MemoryWatch_End();
}
}



void Scan(){
DWORD ScriptContextVFTable = format(adr_ScriptContext);
//DWORD Workspaceaddr = format(adr_Workspace);
ScriptContext = Memorys::Scan(PAGE_READWRITE, (char*)&ScriptContextVFTable, "xxxx");
//Workspace = Memorys::Scan(PAGE_READWRITE, (char*)&Workspaceaddr, "xxxx");
Workspace = *(DWORD*)(ScriptContext + 52); //DataModel, but whatever same

Memory_Watch();
MemoryWatch_Start();
rstate = ((GetLuaState(ScriptContext)));

rlua_getfield(rstate, -10002, "game");
f_index(rstate, "Workspace");
f_index(rstate, "CurrentCamera");
InstanceNew(rstate, "SelectionBox", false);
rlua_settop(rstate, 0);
MemoryWatch_End();

printf("Scan complete!\n\n");
}



void Starter(){
Roblox::refresh_ready = false;
Roblox::getAll(Workspace, g_root);
Roblox::refresh_ready = true;
}

std::string txt = "Empty";
std::vector<int> funcmenuid;
std::vector<std::string> funcmenustr;

void CheckFuncMenu(){
do{
if (txt != "Empty"){
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "g_obj");
rlua_getfield(rstate, -1, txt.c_str());
rlua_pushvalue(rstate, -2);
rlua_pcall(rstate, 1, 0, 0);
MemoryWatch_End();
funcmenustr.clear();
funcmenuid.clear();
txt = "Empty";
}

} while (true);

}








void Main(){

*(DWORD*)(format(0x15E30AC)) = 7;

MessageBoxA(0, "vSplorer - created by sloppey @v3rm \nvSplorer is still in BETA, expect bugs!", "vSplorer", MB_TOPMOST);
Scan();
std::vector<int> Children = Roblox::GetChildren(Workspace);
std::string whom = std::to_string(Children.size());
printf("Size of Workspace: %d\n", Children.size());
CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Starter, 0, 0, 0);
CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheckFuncMenu, 0, 0, 0);
DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)IDD2MAIN);

}


std::string get_str_between_two_str(const std::string &s,    //skidded off stackoverflow
const std::string &start_delim,
const std::string &stop_delim)
{
unsigned first_delim_pos = s.find(start_delim);
unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
unsigned last_delim_pos = s.find(stop_delim);

return s.substr(end_pos_of_first_delim,
last_delim_pos - end_pos_of_first_delim);
}



BOOL InitTreeViewImageLists(HWND hwndTV)
{



// Create the image list. 
if ((eehiml = ImageList_Create(15,
15,
ILC_COLOR24,
4, 0)) == NULL)
return FALSE;

// Add the open file, closed file, and document bitmaps. 

std::string nfm = dirPath() + "\\bin\\none.bmp";
std::string prtfm = dirPath() + "\\bin\\Part_icon.bmp";
std::string scrfm = dirPath() + "\\bin\\script.bmp";
std::string worksfm = dirPath() + "\\bin\\workspace.bmp";
std::string humfm = dirPath() + "\\bin\\humanoid.bmp";
std::string attachfm = dirPath() + "\\bin\\attachment.bmp";
std::string terrfm = dirPath() + "\\bin\\terrain.bmp";
std::string toolfm = dirPath() + "\\bin\\tool.bmp";
std::string modelfm = dirPath() + "\\bin\\model.bmp";
std::string valuefm = dirPath() + "\\bin\\Value.bmp";
std::string shirtfm = dirPath() + "\\bin\\shirt.bmp";
std::string pantsfm = dirPath() + "\\bin\\pants.bmp";
std::string soundfm = dirPath() + "\\bin\\sound.bmp";
std::string instancefm = dirPath() + "\\bin\\Instance.bmp";

HBITMAP hbmp = eehbmp;
HIMAGELIST himl = eehiml;

hbmp = (HBITMAP)LoadImage(0, nfm.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //none
imgs::noneid = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
DeleteObject(hbmp);

hbmp = (HBITMAP)LoadImage(0, valuefm.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //
imgs::valueid = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
DeleteObject(hbmp);

hbmp = (HBITMAP)LoadImage(0, instancefm.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //sound
imgs::instanceid = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
DeleteObject(hbmp);


imgs::LoadAllImages();

if (ImageList_GetImageCount(himl) < 2){
int il = ImageList_GetImageCount(himl);
MessageBoxA(0, "false", std::to_string(il).c_str(), 0);
return FALSE;
}

TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);

return TRUE;
}

BOOL WINAPI InitListViewColumns(HWND hwndLV)
{
char g_achTemp[256];
LV_COLUMN lvc;
int iCol;
lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
lvc.fmt = LVCFMT_LEFT;
lvc.cx = 100;
lvc.pszText = "";
for (iCol = 0; iCol < 2; iCol++) {
if (iCol == 0){
lvc.pszText = "Value";


}
if (iCol == 1){
lvc.pszText = "Property";
}
lvc.iSubItem = iCol;
LoadString(hInst, 1 + iCol,
g_achTemp, sizeof(g_achTemp));
if (ListView_InsertColumn(hwndLV, iCol, &lvc) == -1)
return FALSE;
}
return TRUE;
}

std::vector<int> PropertyuID;
std::vector<std::string> PropertyuName;






std::vector<std::string> parents;


int rGetParent(int Instance) {
if (*(DWORD*)Instance + 0x34)
{
return *(int*)(Instance + 0x34);
}
else{
return 0;
}
}






std::string props[] = { "RobloxLocked",
"Gravity",
"FilteringEnabled",
"Anchored",
"CanCollide",
"Value",
"FallenPartsDestroyHeight",
"Transparency",
"Refelctancy",
"BinType",
"MaxHealth",
"Health",
"WalkSpeed",
"JumpPower",
"BrickColor",
"Material",

"Archivable",
"Name",
"ClassName"
};


std::vector<std::string> revvec;
std::string reverse(std::string const& text)
{
std::stringstream           inStream(text);
std::stringstream           outStream;
std::vector<std::string>    words;

std::copy(std::istream_iterator<std::string>(inStream), std::istream_iterator<std::string>(), std::back_inserter(words));
std::copy(words.rbegin(), words.rend(), std::ostream_iterator<std::string>(outStream, "\n"));
return outStream.str();
}

void reverseVector(){
std::vector<std::string> n_v(parents.rbegin(), parents.rend());
parents.clear();
for (int i = 0; i < n_v.size(); i++){
printf("Indexing item: %s", n_v[i].c_str());
parents.push_back(n_v[i]);
}
}




void goBackToWorkspace(int inst){
Instance ins;
int par = inst;
std::string snamepar = ins.getClass(par)->c_str();
if (parents.size() > 0){
parents.clear();
}
do{
Sleep(1);
snamepar = ins.getClass(par)->c_str();
std::string snname = ins.getName(par)->c_str();
parents.push_back(snname);
par = rGetParent(par);
snamepar = ins.getClass(par)->c_str();

} while (snamepar != "DataModel");


reverseVector();
try{
for (int i = 0; i < parents.size(); i++){
MemoryWatch_Start();
rlua_getfield(rstate, -1, parents[i].c_str());
MemoryWatch_End();
}
}
catch (std::exception e){
MessageBoxA(0, e.what(), "Error", 0);
}
}

void getSelected(int instkey){
int selitem = InstanceKEY[instkey];
goBackToWorkspace(selitem);
}

int PushVector3(int state, const char* field, float x, float y, float z){
rgetglobal(state, "Vector3");
f_index(state, "new");
rlua_pushnumber(state, x);
rlua_pushnumber(state, y);
rlua_pushnumber(state, z);
rlua_pcall(state, 3, 1, 0);
rlua_setfield(state, -3, field);
return 3;
}

std::vector<std::string> propapi;
std::vector<std::string> funcapi;

std::string getClassName(){
rlua_getfield(rstate, -10002, "g_obj");
rlua_getfield(rstate, -1, "ClassName");
return rlua_tolstring(rstate, -1, NULL);
}

void parseProp(int instkey, const char* n, const char* rn){ //classname, name
std::string sn = n;
MemoryWatch_Start();
rlua_settop(rstate, 0);
MemoryWatch_End();
if (sn != "Workspace"){
try{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "game");
MemoryWatch_End();
getSelected(instkey);
}
catch (std::exception e){
printf("Error: %s\n", e.what());
}
}
else{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "game");
rlua_getfield(rstate, -1, "Workspace");
MemoryWatch_End();
}

MemoryWatch_Start();
rlua_setfield(rstate, -10002, "g_obj");
std::string cnameapi = getClassName();
MemoryWatch_End();
proplistidstr.clear();
proplistid.clear();
propapi.clear();
funcapi.clear();
funcmenustr.clear();
funcmenuid.clear();
delete(CallMethodMenu);
CallMethodMenu = CreateMenu();
if (cnameapi == "Part"){
cnameapi = "BasePart";
}
std::string apiurl = "https://anaminus.github.io/api/class/" + (std::string)cnameapi + ".html";
std::string apiurlInstance = "https://anaminus.github.io/api/class/" + (std::string)"Instance" + ".html";

std::string apiparsed = parsing::parsePropName(parsing::DownloadString(apiurl));
std::string apiparsedInstance = parsing::parsePropName(parsing::DownloadString(apiurlInstance));
std::istringstream f(apiparsed);
std::string line;
while (std::getline(f, line)) {
propapi.push_back(line);
}

std::string apiparsedfunc = parsing::parseFuncName(parsing::DownloadString(apiurl));
std::istringstream funcstr(apiparsedfunc);
std::string funcline;
while (std::getline(funcstr, funcline)) {
funcapi.push_back(funcline);
}

std::string apiparsedfuncinst = parsing::parseFuncName(parsing::DownloadString(apiurlInstance));
std::istringstream funcstrins(apiparsedfuncinst);
std::string funclineinst;
while (std::getline(funcstrins, funclineinst)) {
funcapi.push_back(funclineinst);
}

std::istringstream f2(apiparsedInstance);
std::string line2;
while (std::getline(f2, line2)) {
propapi.push_back(line2);
}
for (int i = 0; i < 2; i++){
propapi.pop_back(); //remove the last two items from the vector due to it being duplicates
}

for (int i = 0; i < propapi.size(); i++){
printf("Checking property: %s\n", propapi[i].c_str());
CheckProperty(rstate, propapi[i]);
}
reverseVectorProp();
globali = 0;
}

int PropertyLParam = 0;
std::vector<LPSTR> PropertyStack;
std::vector<int> PropertyIDX;

void AddObject(HWND listView, const char* Name, const char* Value, bool readOnly) //skidded from rTF, based off my idea (i was lazy same)
{
LVITEM lvI;
LPSTR objName = const_cast<LPSTR>(Name);
LPSTR objValue = const_cast<LPSTR>(Value);

lvI.pszText = TEXT(objValue); // objName
if (readOnly)
{
lvI.mask = LVIF_TEXT | LVIF_STATE | LVCF_SUBITEM | LVIF_PARAM | ES_READONLY;
}
else
{
lvI.mask = LVIF_TEXT | LVIF_STATE | LVCF_SUBITEM | LVIF_PARAM;
}
lvI.iItem = 0;
lvI.iSubItem = 0; // 0
lvI.state = 0;

lvI.lParam = (int)PropertyLParam;
PropertyLParam = PropertyLParam + 1;

PropertyStack.push_back(objName);

ListView_InsertItem(listView, &lvI);
ListView_SetItemText(listView, 0, 1, objName);

}


void parsePropID(LPSTR newval, int propid){
printf("Property of %s\n", proplistidstr[propid].c_str());
std::string datype = "";
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "g_obj");
rlua_getfield(rstate, -1, proplistidstr[propid].c_str());
int typ = rlua_type(rstate, -1);
MemoryWatch_End();
switch (typ){
case RLUA_TNUMBER: {
printf("Setting as a number value\n");
//rlua_pushnumber(rstate, std::atof(newval));
datype = "number";
break;
}
case RLUA_TSTRING: {
printf("Setting as a string value\n");
//rlua_pushstring(rstate, newval);
datype = "string";
break;
}
case RLUA_TBOOLEAN: {
printf("Setting as a boolean value\n");
//rlua_pushboolean(rstate, std::atoi(newval));
datype = "bool";
break;
}
case RLUA_TUSERDATA: {
printf("Setting as a userdata value\n");
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "typeof");
rlua_pushvalue(rstate, -2);
rlua_pcall(rstate, 1, 1, 0);
std::string typof = rlua_tolstring(rstate, -1, NULL);
datype = typof;
MemoryWatch_End();

break;
}
default:{
printf("Unknown value\n");
datype = "unknown";
}

}
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "g_obj");
if (datype == "number"){
rlua_pushnumber(rstate, std::atof(newval));
rlua_setfield(rstate, -2, proplistidstr[propid].c_str());
MemoryWatch_End();
}
else if (datype == "string"){
rlua_pushstring(rstate, newval);
rlua_setfield(rstate, -2, proplistidstr[propid].c_str());
MemoryWatch_End();
}
else if (datype == "Vector3"){
printf("Finna set VECTOR3 VALUE: %s\n", proplistidstr[propid].c_str());
rgetglobal(rstate, "Vector3");
f_index(rstate, "new");
std::vector<std::string> v3split = split(newval, ',');
parsing::replaceAll(v3split[0], " ", "");
parsing::replaceAll(v3split[1], " ", "");
parsing::replaceAll(v3split[2], " ", "");


rlua_pushnumber(rstate, std::atof(v3split[0].c_str()));
rlua_pushnumber(rstate, std::atof(v3split[1].c_str()));
rlua_pushnumber(rstate, std::atof(v3split[2].c_str()));

rlua_pcall(rstate, 3, 1, 0);
rlua_setfield(rstate, -3, proplistidstr[propid].c_str());
MemoryWatch_End();

}

else if (datype == "CFrame"){
printf("Finna set CFRAME VALUE: %s\n", proplistidstr[propid].c_str());
rgetglobal(rstate, "CFrame");
f_index(rstate, "new");
std::vector<std::string> cfsplit = split(newval, ',');
parsing::replaceAll(cfsplit[0], " ", "");
parsing::replaceAll(cfsplit[1], " ", "");
parsing::replaceAll(cfsplit[2], " ", "");

rlua_pushnumber(rstate, std::atof(cfsplit[0].c_str()));
rlua_pushnumber(rstate, std::atof(cfsplit[1].c_str()));
rlua_pushnumber(rstate, std::atof(cfsplit[2].c_str()));

rlua_pcall(rstate, 3, 1, 0);
rlua_setfield(rstate, -3, proplistidstr[propid].c_str());
MemoryWatch_End();

}

else if (datype == "BrickColor"){
printf("Finna set BRICKCOLOR VALUE: %s\n", proplistidstr[propid].c_str());
rgetglobal(rstate, "BrickColor");
f_index(rstate, "new");
rlua_pushstring(rstate, newval);
rlua_pcall(rstate, 1, 1, 0);
rlua_setfield(rstate, -3, proplistidstr[propid].c_str());
MemoryWatch_End();

}
else if (datype == "EnumItem"){
rlua_pushstring(rstate, newval);
rlua_setfield(rstate, -2, proplistidstr[propid].c_str());
MemoryWatch_End();

}

else if (datype == "bool"){
if (newval == "false" || newval == "0"){
newval = "0";
}
else{
newval = "1";
}
rlua_pushboolean(rstate, std::atoi(newval));
rlua_setfield(rstate, -2, proplistidstr[propid].c_str());
MemoryWatch_End();
}


}




void rDestroy(int state, bool ret){
if (ret == true){
MemoryWatch_Start();
}
f_index(state, "Destroy");
rlua_pushvalue(state, -2);
rlua_pcall(state, 1, 0, 0);
if (ret == true){
MemoryWatch_End();
}
}

std::string strreplace(std::string &s,
const std::string &toReplace,
const std::string &replaceWith)
{
return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}


std::string GetLastErrorAsString()
{
//Get the error message, if any.
DWORD errorMessageID = ::GetLastError();
if (errorMessageID == 0)
return std::string(); //No error message has been recorded

LPSTR messageBuffer = nullptr;
size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

std::string message(messageBuffer, size);

//Free the buffer.
LocalFree(messageBuffer);

return message;
}


int xid = 250;

int GetInstanceById(int id){
return InstanceKEY[id];
}

int InstanceIdByName(std::string name){
Instance inst;
printf("-----INSTANCEKEY SIZE IS %d\nINSTNAME SIZE IS %d\n", InstanceKEY.size(), instname.size());
try{
if (InstanceKEY.size() > 0){
printf("if size is > 0\n");
for (int i = 0; i < InstanceKEY.size(); i++){
//if (*(DWORD*)(InstanceKEY[i])){
printf("if instancekey[i], getting name\nThe getting name of id: %d\nMax Instances: %d\n", i, InstanceKEY.size());
const char* whom = inst.getName(InstanceKEY[i])->c_str();
std::string strwh = instname[i];
printf("got name form instname %s\n", strwh.c_str());
if (strwh == name){
printf("Returned id: %d\n", Roblox::vecidxitm[i]);
return Roblox::vecidxitm[i];
}
//}
//else{
// printf("Error getting instance[i]\n");
//return Roblox::vecidxitm[1];
//}
}
return 0;
}
}
catch (std::exception e){
MessageBoxA(0, e.what(), "Error", 0);
}
return 0;
}

void TVTEXTCOLORLOOP(){
COLORREF tvc;
do{
Sleep(200);
tvc = RGB(244, 66, 66);
ListView_SetTextColor(treeviewi, tvc);
for (int i = 66; i < 244; i++){
Sleep(200);
tvc = RGB(244, i, 66);
TreeView_SetTextColor(treeviewi, tvc);
}
for (int i = 244; i > 66; i--){
Sleep(200);
tvc = RGB(i, 244, 66);
TreeView_SetTextColor(treeviewi, tvc);
}
for (int i = 66; i < 244; i++){
Sleep(200);
tvc = RGB(66, 244, i);
TreeView_SetTextColor(treeviewi, tvc);
}
for (int i = 244; i > 66; i--){
Sleep(200);
tvc = RGB(66, i, 244);
TreeView_SetTextColor(treeviewi, tvc);
}
for (int i = 66; i < 244; i++){
Sleep(200);
tvc = RGB(i, 66, 244);
TreeView_SetTextColor(treeviewi, tvc);
}
for (int i = 244; i > 66; i--){
Sleep(200);
tvc = RGB(66, 66, i);
TreeView_SetTextColor(treeviewi, tvc);

}
} while (true);
}

void LoadScreen(HWND hWnd) {
std::string dirp = dirPath() + "\\bin\\theme.bmp";
PAINTSTRUCT     ps;
HDC             hdc;
BITMAP          bitmap;
HDC             hdcMem;
HGDIOBJ         oldBitmap;
bgBitMap = (HBITMAP)LoadImage(NULL, dirp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


hdc = BeginPaint(hWnd, &ps);

hdcMem = CreateCompatibleDC(hdc);
oldBitmap = SelectObject(hdcMem, bgBitMap);

GetObject(bgBitMap, sizeof(bitmap), &bitmap);
BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

SelectObject(hdcMem, oldBitmap);
DeleteDC(hdcMem);

EndPaint(hWnd, &ps);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

//TreeView_SetBkColor(treeviewi, color);

switch (uMsg)
{
case WM_INITDIALOG:
{

//MessageBoxA(0, "Starting scan", "", 0);
//LoadScreen(hwndDlg);
treeviewi = GetDlgItem(hwndDlg, 103);
listviewi = GetDlgItem(hwndDlg, 104);
IDDMAINHWND = GetDlgItem(hwndDlg, IDD_DIALOG1);
TreeView_SetExtendedStyle(treeviewi, 0, TVS_EX_DOUBLEBUFFER);

//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TVTEXTCOLORLOOP, 0, 0, 0);
SetWindowTheme(listviewi, L"Explorer", NULL);
SetWindowTheme(treeviewi, L"Explorer", NULL);
InitTreeViewImageLists(treeviewi);


g_root = InsertRootNode(treeviewi, "Game", 0);

Main();
if (treeviewi){
InitListViewColumns(listviewi);
ListView_SetExtendedListViewStyleEx(listviewi, 0, LVS_EX_GRIDLINES | LVS_EX_HEADERINALLVIEWS | LVS_EDITLABELS);

//HTREEITEM res = InsertRootNode(treeviewi, "Workspace");
//InsertChildNode(treeviewi, "Terrain", res);
}
break;

}
return TRUE;

case WM_CLOSE:
{
EndDialog(hwndDlg, 0);
}
return TRUE;

case WM_PAINT: {
LoadScreen(hwndDlg);
break;
}

case WM_NOTIFY: {
if (reinterpret_cast<LPNMHDR>(lParam)->hwndFrom == treeviewi)
{
if (reinterpret_cast<LPNMHDR>(lParam)->code == NM_RCLICK)
{
POINT p;
if (GetCursorPos(&p))
{


m_hMenu = CreatePopupMenu();

InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_REFRESH, "Refresh");
InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_DESTROY, "Destroy");
InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_CLONE, "Clone");
InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_TPTO, "Teleport To");

InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_CALLREMOTE, "Call Remote");

AppendMenu(m_hMenu, MF_POPUP, (UINT_PTR)CallMethodMenu, "Call Function");

for (int i = 0; i < funcapi.size(); i++){
Sleep(1);
funcmenuid.push_back(xid);
funcmenustr.push_back(funcapi[i]);
AppendMenu(CallMethodMenu, MF_STRING, xid, funcapi[i].c_str());
xid += 1;
}
xid = 250;


InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_DISABLED, IDM_INSERTOBJ, "Insert Instance (disabled)");
InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_DISABLED, IDM_CUT, "Cut");
InsertMenu(m_hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_DISABLED, IDM_PASTE, "Paste");
TrackPopupMenu(m_hMenu, TPM_TOPALIGN | TPM_LEFTALIGN, p.x, p.y, 0, hwndDlg, NULL);

}
}
}



switch (((LPNMHDR)lParam)->code){




case LVN_ENDLABELEDIT: {
LPNMHDR lpNmHdr = (LPNMHDR)lParam;
if (lpNmHdr->code == LVN_ENDLABELEDIT)
{
NMLVDISPINFO *info = (NMLVDISPINFO*)lParam;
if (info->item.pszText != NULL)
{

LVITEM same = info->item;
if (same.lParam != NULL)
{
int ifi = info->item.iItem;

parsePropID(same.pszText, ifi);

MemoryWatch_Start();
rw_checkstack(rstate, 4 * sizeof(DWORD));

MemoryWatch_End();

}

}

}
break;
}




case TVN_SELCHANGED:
{
ListView_DeleteAllItems(listviewi);

HTREEITEM hSelectedItem = TreeView_GetSelection(treeviewi);
TCHAR buffer[128];
if (hSelectedItem != NULL){

TVITEM item;
Instance rinst;
item.hItem = hSelectedItem;
item.mask = TVIF_TEXT;
item.cchTextMax = 128;
item.pszText = buffer;
if (TreeView_GetItem(treeviewi, &item))
{


//std::string betstr = get_str_between_two_str(item.pszText, " [#", "]");
if (InstanceKEY.size() > 3 || Roblox::refresh_ready == true){
printf("--------------selected getting id\n");
int selidx = InstanceIdByName(item.pszText);//std::atoi(betstr.c_str());
printf("--------------selected GOT id: %d\n", selidx);
int selinstance = InstanceKEY[selidx];
printf("--------------selected PASSED INSTANCEKEY\n", selidx);

const char* cnameinst = rinst.getClass(selinstance)->c_str();
const char* rname = rinst.getName(selinstance)->c_str();
//NewListItem(0, 0, "ClassName", (LPSTR)cnameinst);
//NewListItem(0, 0, "Name", (LPSTR)rname);
parseProp(selidx, cnameinst, rname);


try{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "game");
f_index(rstate, "Workspace");
f_index(rstate, "CurrentCamera");
f_index(rstate, "SelectionBox");
rlua_getfield(rstate, -10002, "g_obj");
rlua_setfield(rstate, -2, "Adornee");

//rlua_checkstack(rstate, 2 * sizeof(DWORD));
//rlua_settop(rstate, 0);
//rstate = rlua_newthread((GetLuaState(ScriptContext)));
MemoryWatch_End();
rw_checkstack(rstate, 4 * sizeof(DWORD));
printf("\n!!!!!!!!!!!!Stack Cleared, and grew\n");
}
catch (std::exception e){
//rlua_checkstack(rstate, 2 * sizeof(DWORD));
//rlua_settop(rstate, 0);
printf("Error: Bad Cast\n");
MemoryWatch_End();
}



//std::cout << item.pszText << " has been selected\nID: " << betstr.c_str() << "\n";
//std::cout << "ClassName: " << cnameinst << std::endl;
}
}
}
break;
}
//ect case

}
break;
}

case WM_ERASEBKGND:
return 1;



case WM_COMMAND:
{
switch (LOWORD(wParam))
{
case IDM_DESTROY:
{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "g_obj");
rDestroy(rstate, false);
MemoryWatch_End();
HTREEITEM hSelectedItem = TreeView_GetSelection(treeviewi);
TCHAR buffer[128];
if (hSelectedItem != NULL){

TVITEM item;
Instance rinst;
item.hItem = hSelectedItem;
item.mask = TVIF_TEXT;
item.cchTextMax = 128;
item.pszText = buffer;
if (TreeView_GetItem(treeviewi, &item))
{
TreeView_DeleteItem(treeviewi, item.hItem);
}
}
break;
}

case IDM_CLONE:
{
MemoryWatch_Start();
try{
rlua_getfield(rstate, -10002, "g_obj");
f_index(rstate, "Clone");
rlua_pushvalue(rstate, -2);
rlua_pcall(rstate, 1, 1, 0);
rgetglobal(rstate, "g_obj");
f_index(rstate, "Parent");
rlua_setfield(rstate, -3, "Parent");
MemoryWatch_End();
}
catch (std::exception e){
MemoryWatch_End();
MessageBoxA(0, e.what(), "Error", 0);
}

//MemoryWatch_End();
HTREEITEM hSelectedItem = TreeView_GetSelection(treeviewi);
TCHAR buffer[128];
if (hSelectedItem != NULL){

TVITEM item;
Instance rinst;
item.hItem = hSelectedItem;
item.mask = TVIF_TEXT;
item.cchTextMax = 128;
item.pszText = buffer;
if (TreeView_GetItem(treeviewi, &item))
{
InstanceKEY.clear();
Roblox::idxitm = 0;
Roblox::vecidxitm.clear();
instname.clear();



TreeView_DeleteAllItems(treeviewi);
g_root = InsertRootNode(treeviewi, "Game", -1);
Roblox::getAll(Workspace, g_root);
TreeView_Expand(treeviewi, g_root, TVE_EXPANDPARTIAL);

//HTREEITEM ti = TreeView_GetParent(treeviewi, item.hItem);


//TreeView_DeleteItem(treeviewi, item.lParam);
}
}
break;
}
case IDM_REFRESH: {
Roblox::refresh_ready = false;
InstanceKEY.clear();
Roblox::idxitm = 0;
Roblox::vecidxitm.clear();
instname.clear();
TreeView_DeleteAllItems(treeviewi);
g_root = InsertRootNode(treeviewi, "Game", -1);
CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Starter, 0, 0, 0);
DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)IDD2MAIN);
break;
}

case IDM_INSERTOBJ:
{

break;
}
case IDM_TPTO:
{
try{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "Game");
f_index(rstate, "GetService");
rlua_pushvalue(rstate, -2);
rlua_pushstring(rstate, "Players");
rlua_pcall(rstate, 2, 1, 0);
//f_index(rstate, "Players");
f_index(rstate, "LocalPlayer");
f_index(rstate, "Character");
f_index(rstate, "HumanoidRootPart");
rlua_getfield(rstate, -10002, "g_obj");
f_index(rstate, "CFrame");
rlua_setfield(rstate, -3, "CFrame");
MemoryWatch_End();
}
catch (std::exception e){
MemoryWatch_End();
MessageBoxA(0, e.what(), "Error", 0);
}

break;
}

case IDM_CALLREMOTE:{

DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)IDDMAIN);
break;
}

case 250:{
txt = funcmenustr[0];
break;
}
case 251:{
txt = funcmenustr[1];
break;

}
case 252:{
txt = funcmenustr[2];
break;
}
case 253: {
txt = funcmenustr[3];
break;
}
case 254:{
txt = funcmenustr[4];
break;
}
case 255: {
txt = funcmenustr[5];
break;
}
case 256:{
txt = funcmenustr[6];
break;
}
case 257: {
txt = funcmenustr[7];
break;
}
case 258: {
txt = funcmenustr[8];
break;
}
case 259: {
txt = funcmenustr[9];
break;
}
case 260:{
txt = funcmenustr[10];
break;
}
case 261:
{
txt = funcmenustr[11];

break;
}

}
break;
}
return TRUE;
}
return FALSE;
}

bool is_number(const std::string& s)
{
return !s.empty() && std::find_if(s.begin(),
s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

BOOL CALLBACK IDDMAIN(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


switch (uMsg)
{
case WM_INITDIALOG:
{

break;

}
return TRUE;

case WM_CLOSE:
{
EndDialog(hwndDlg, 0);
}
return TRUE;

case WM_NOTIFY: {
switch (((LPNMHDR)lParam)->code){


}
break;
}



case WM_COMMAND:
{
switch (LOWORD(wParam))
{
case 600: {
int len = GetWindowTextLengthW(GetDlgItem(hwndDlg, 601)) + 1;
wchar_t text[300];
GetWindowTextW(GetDlgItem(hwndDlg, 601), text, len);
std::wstring ws(text);
std::string str(ws.begin(), ws.end());
printf("Got Text: %s\n", str.c_str());
std::vector<std::string> spl = split(str, ',');
int itoret = 0;
try{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "g_obj");
rlua_getfield(rstate, -1, "FireServer");
rlua_pushvalue(rstate, -2);
for (int i = 0; i < spl.size(); i++){
Sleep(1);
if (is_number(spl[i])){
printf("(NUMBER)Pushing %s\n", spl[i].c_str());
rlua_pushnumber(rstate, std::atof(spl[i].c_str()));
}
else{
printf("(STRING)Pushing %s\n", spl[i].c_str());
rlua_pushstring(rstate, spl[i].c_str());
}
itoret += 1;
}
printf("Argcount: %d\n", itoret);
rlua_pcall(rstate, itoret + 1, 0, 0);
MemoryWatch_End();
}
catch (std::exception e){
MemoryWatch_End();
std::string ermsg = e.what();
ermsg += ".\nMake sure you selected a RemoteEvent.";
MessageBoxA(0, ermsg.c_str(), "Error", 0);
}
break;
}
case 602: {
int len = GetWindowTextLengthW(GetDlgItem(hwndDlg, 601)) + 1;
wchar_t text[300];
GetWindowTextW(GetDlgItem(hwndDlg, 601), text, len);
std::wstring ws(text);
std::string str(ws.begin(), ws.end());
printf("Got Text: %s\n", str.c_str());
std::vector<std::string> spl = split(str, ',');
int itoret = 0;
try{
MemoryWatch_Start();
rlua_getfield(rstate, -10002, "g_obj");
rlua_getfield(rstate, -1, "InvokeServer");
rlua_pushvalue(rstate, -2);
for (int i = 0; i < spl.size(); i++){
Sleep(1);
if (is_number(spl[i])){
printf("(NUMBER)Pushing %s\n", spl[i].c_str());
rlua_pushnumber(rstate, std::atof(spl[i].c_str()));
}
else{
printf("(STRING)Pushing %s\n", spl[i].c_str());
rlua_pushstring(rstate, spl[i].c_str());
}
itoret += 1;
}
printf("Argcount: %d\n", itoret);
rlua_pcall(rstate, itoret + 1, 0, 0);
MemoryWatch_End();
}
catch (std::exception e){
MemoryWatch_End();
std::string ermsg = e.what();
ermsg += ".\nMake sure you selected a RemoteFunction.";
MessageBoxA(0, ermsg.c_str(), "Error", 0);
}
break;
}

}
break;
}
return TRUE;
}
return FALSE;
}


void refresh_s1(){
do{
//printf("Not Ready\n");
Sleep(1);
} while (Roblox::refresh_ready == false);
//printf("Ready\n");
EndDialog(refHwnd, 0);
}

BOOL CALLBACK IDD2MAIN(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

refHwnd = hwndDlg;

switch (uMsg)
{
case WM_SHOWWINDOW: {
CreateThread(0, 0, (LPTHREAD_START_ROUTINE)refresh_s1, 0, 0, 0);
//EndDialog(hwndDlg, 0);
break;
}
case WM_INITDIALOG:
{


break;

}
return TRUE;

case WM_CLOSE:
{
EndDialog(hwndDlg, 0);
}
return TRUE;

case WM_NOTIFY: {
switch (((LPNMHDR)lParam)->code){


}
break;
}



case WM_COMMAND:
{
switch (LOWORD(wParam))
{



}
break;
}
return TRUE;
}
return FALSE;
}