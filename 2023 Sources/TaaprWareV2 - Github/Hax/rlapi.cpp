#include "rlapi.h"

TValue* luaO_nilobject = reinterpret_cast<TValue*>(addresses::luaO_nilobject);

const types::pseudo2addr pseudo2addr = functions::luau::pseudo2addr;

int gettop(UINT rL) {
    return (int)(rL_get(StkId, top) - rL_get(StkId, base));
}

void settop(UINT rL, int idx) {
    if (idx >= 0) {
        while (rL_get(StkId, top) < rL_get(StkId, base) + idx)
            setnilvalue(rL_get(StkId, top));
        rL_get(StkId, top) = rL_get(StkId, base) + idx;
    } else {
        rL_get(StkId, top) += idx + 1; // `subtract' index (index is negative)
    }
}

TValue* index2addr(UINT rL, int idx) {
    if (idx > 0) {
        TValue* o = rL_get(StkId, base) + (idx - 1);
        //api_check(L, idx <= L->ci->top - L->base);
        if (o >= rL_get(StkId, top))
            return (TValue*)luaO_nilobject;
        else
            return o;
    } else if (idx > LUA_REGISTRYINDEX) {
        //api_check(L, idx != 0 && -idx <= L->top - L->base);
        return rL_get(StkId, top) + idx;
    } else {
        return (TValue*)functions::luau::pseudo2addr(rL, idx);
    }
}

void pushvalue(UINT rL, int idx) {
    StkId o = index2addr(rL, idx);
    setobj2s(rL, rL_get(StkId, top), o);
    api_incr_top(rL);
}

void pushnil(UINT rL) {
    setnilvalue(rL_get(StkId, top));
    api_incr_top(L);
}

void pushnumber(UINT rL, double n) {
    setnvalue(rL_get(StkId, top), n);
    api_incr_top(L);
}

void pushinteger(UINT rL, int n) {
    setnvalue(rL_get(StkId, top), (double)(n));
    api_incr_top(L);
}

void pushunsigned(UINT rL, unsigned u) {
    setnvalue(rL_get(StkId, top), (double)(u));
    api_incr_top(L);
}

void pushvector(UINT rL, float x, float y, float z) {
    setvvalue(rL_get(StkId, top), x, y, z, 0.0f);
    api_incr_top(L);
}

void pushboolean(UINT rL, int b) {
    setbvalue(rL_get(StkId, top), (b != 0)); // ensure that true is 1
    api_incr_top(rL);
}

void pushtable(UINT rL, UINT t) {
    sethvalue(rL, rL_get(StkId, top), t);
    api_incr_top(rL);
}

unsigned int luaS_hash(const char* str, size_t len) {
    // Note that this hashing algorithm is replicated in BytecodeBuilder.cpp, BytecodeBuilder::getStringHash
    unsigned int a = 0, b = 0;
    unsigned int h = unsigned(len);

    // hash prefix in 12b chunks (using aligned reads) with ARX based hash (LuaJIT v2.1, lookup3)
    // note that we stop at length<32 to maintain compatibility with Lua 5.1
    while (len >= 32) {
#define rol(x, s) ((x >> s) | (x << (32 - s)))
#define mix(u, v, w) a ^= h, a -= rol(h, u), b ^= a, b -= rol(a, v), h ^= b, h -= rol(b, w)

        // should compile into fast unaligned reads
        uint32_t block[3];
        memcpy(block, str, 12);

        a += block[0];
        b += block[1];
        h += block[2];
        mix(14, 11, 25);
        str += 12;
        len -= 12;

#undef mix
#undef rol
    }

    // original Lua 5.1 hash for compatibility (exact match when len<32)
    for (size_t i = len; i > 0; --i)
        h ^= (h << 5) + (h >> 2) + (uint8_t)str[i - 1];

    return h;
}

void pushlstring(UINT rL, const char* s, size_t len) {
    setsvalue(rL, rL_get(StkId, top), luaS_newlstr(rL, s, len));
    api_incr_top(L);
}

void pushstring(UINT rL, const char* s) {
    if (s == NULL)
        pushnil(rL);
    else
        pushlstring(rL, s, strlen(s));
}

UINT luaS_newlstr(UINT rL, const char* str, size_t l) {
    unsigned int h = luaS_hash(str, l);
    // This is not required but i believe if you dont have it your exploit strings will always fail EQ checks with roblox strings
    /*
    for (TString* el = L->global->strt.hash[lmod(h, L->global->strt.size)]; el != NULL; el = el->next)
    {
        if (el->len == l && (memcmp(str, getstr(el), l) == 0))
        {
            // string may be dead
            if (isdead(L->global, obj2gco(el)))
                changewhite(obj2gco(el));
            return el;
        }
    }
    */
    return newlstr(rL, str, l, h); // not found
}

UINT newlstr(UINT rL, const char* str, size_t l, unsigned int h) {
    // this is the first good function i have recreated ever
    // since this was hell for me to make the first time ill add comments to help someone else whos also doing it for the first time
    if (l > MAXSSIZE) {
        printf(XorString("String too big! (How did you make a string over 1 billion characters long)\n"));
        return 0;
    }
    const int mem_size = 24 + l + 1; // 24 is the size of a TString + length of string + 1 byte for the '\0'

    UINT ts = (UINT)malloc(mem_size);
    //printf("Allocated memory at %X\n", ts);
    luaC_init(rL, ts, LUA_TSTRING); // DO NOT FORGET THIS!!!!!!!!!!!!!!!!!!!! (without this the GC will completely error and die)
    //printf("Initialized\n");
    structptr_set(ts, UINT, rTString, atom, ATOM_UNDEF); // not required, looking for ATOM_UNDEF (32768) is good for identifying newlstr pseudocode in ida
    // atom in computer science is for stopping any other code from erroring or being unstable because it accessed an object in the middle of a calculation
    //printf("Set ts->atom\n");
    structptr_set(ts, UINT, rTString, hash, h); // not required, encrypted, not used for eq checks suprisingly
    //printf("Set ts->hash\n");
    structptr_set(ts, UINT, rTString, len, unsigned(l)); // encrypted
    //printf("Set ts->len\n");

    memcpy((char*)(struct_get(ts, UINT, rTString, data)), str, l); // structptr_get wouldnt work here because we need address not value at address
    // you can see from Cheat Engine how easy it is to get a const char from a TString
    //printf("Set ts->data\n");
    ((char*)(struct_get(ts, UINT, rTString, data)))[l] = '\0'; // ending 0, remove if you want random garbage in memory at the more unexpected times
    //printf("Added null terminator\n");
    // this is actually useless which is something i didnt realize when writing it
    // it would only do stuff if i recreated the commented code i put in luaS_newlstr
    /*
    UINT rG = structptr_get(rL, UINT, rL, global);
    //printf("Got rG\n");
    h = lmod(h, structptr_get(rG, UINT, rG, strt_size));
    //printf("Set h to %X\n", h);
    structptr_set(ts, UINT, rTString, next, (*structptr_get(rG, UINT**, rG, strt_hash))[h]);
    //printf("Set ts->next\n");
    (*structptr_get(rG, UINT**, rG, strt_hash))[h] = ts;
    //printf("Set tb->hash[h]\n");

    structptr_set(ts, UINT, rG, strt_nuse, structptr_get(rG, UINT, rG, strt_nuse) + 1);
    //printf("Incremented nuse\n");
    if (structptr_get(rG, UINT, rG, strt_nuse) > (uint32_t)(structptr_get(rG, UINT, rG, strt_size)) && structptr_get(rG, UINT, rG, strt_size) <= INT_MAX / 2) {
        //luaS_resize(L, strt->size * 2); // too crowded
        printf("oh no\n");
    }
    */
    //printf("Done\n");
    return (UINT)ts;
}

void insert(UINT rL, int idx) {
    StkId p = index2addr(rL, idx);
    for (StkId q = rL_get(StkId, top); q > p; q--)
        setobj2s(L, q, q - 1);
    setobj2s(L, p, rL_get(StkId, top));
}

void xmove(UINT from, UINT to, int n) {
    if (from == to) {
        return;
    }
    StkId ttop = *reinterpret_cast<StkId*>(to + offsets::rL::top);
    StkId ftop = *reinterpret_cast<StkId*>(from + offsets::rL::top) - n;
    for (int i = 0; i < n; i++) {
        setobj2s(to, ttop + i, ftop + i);
    }
    *reinterpret_cast<StkId*>(from + offsets::rL::top) = ftop;
    *reinterpret_cast<StkId*>(to + offsets::rL::top) = ttop + n;
}