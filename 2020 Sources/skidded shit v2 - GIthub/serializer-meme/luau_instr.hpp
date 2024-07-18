#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <Psapi.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <limits.h>
#include <TlHelp32.h>
#include <DbgHelp.h>
#include <Psapi.h>
#include <WinInet.h>
#include <urlmon.h>
#include <bitset>
#include <stdint.h>
#include <string.h>
#include <iterator>
#include <codecvt>
#include <SubAuth.h>
#include <stdint.h>


#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "WinInet.lib")
#pragma comment(lib, "urlmon.lib")


extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
#include "lua/lobject.h"
#include "lua/lstring.h"
#include "lua/lopcodes.h"
#include "lua/lfunc.h"
#include "lua/ldo.h"
}

#include <cstdint>
#include <optional>

namespace syn
{

    extern uint8_t LuaU_MagicMul;

#pragma pack(push, 1)
    union LuauInstruction
    {
        struct
        {
            uint8_t OpCode;
            uint8_t A;
            union
            {
                struct
                {
                    uint8_t B;
                    uint8_t C;
                };
                uint16_t Bx;
                int16_t sBx;
            };
        };
        uint32_t Value;

        [[nodiscard]] uint32_t GetAx() const { return Value >> 8; }
        [[nodiscard]] int32_t GetsAx() const { return (int32_t)GetAx(); }
        void SetAx(const uint32_t value) { Value = Value & 0xFF | value << 8; }
        void SetsAx(const int32_t value) { SetAx((uint32_t)value); }

        explicit LuauInstruction()
        {
            Value = 0;
        }

        explicit LuauInstruction(const Instruction C)
        {
            Value = C;
        }

        void SetOpCode(const uint8_t ROpCode)
        {
            OpCode = (uint8_t)((uint8_t)ROpCode * LuaU_MagicMul);
        }

        explicit operator Instruction() const
        {
            return Value;
        }

        explicit operator DWORD() const
        {
            return Value;
        }
    };
    static_assert(sizeof(LuauInstruction) == sizeof(uint32_t), "LuauInstruction size mismatch");

    /* OneWay = Lua -> Luau, TwoWay = Lua -> Luau -> Lua */
    class OneWayLuauTranslator
    {
    private:
        Proto* CurrentProto = nullptr;
    public:
        __declspec(noinline) std::vector<LuauInstruction> Convert(Instruction* Instrs, size_t Size) const;

        OneWayLuauTranslator(Proto* p)
        {
            CurrentProto = p;
        }
    };

    /* OneWay = Lua -> Luau, TwoWay = Lua -> Luau -> Lua */
    class OneWayLuauDecompiler
    {
    public:
        __declspec(noinline) static std::vector<LuauInstruction> Convert(LuauInstruction* OInstrs, size_t Size);
    };
}

namespace syn
{
    uint8_t LuaU_MagicMul = 227;

    enum LuauOp
    {
        OP_NOOP,		/* N	No Operation */
        OP_EXIT,		/* N	Updates L->savedpc and exits the VM */
        OP_LOADNIL,		/* A	Base[A] := nil	*/
        OP_LOADBOOL,	/* ABC	Base[A] := (bool)B; Pc += C */
        OP_LOADINT,		/* ABx	Base[A] := (double)Bx */
        OP_LOADK,		/* ABx	Base[A] := K[Bx] */
        OP_MOVE,		/* ABx	Base[A] := Base[Bx] */
        OP_GETENV,		/* AC	Base[A] := Env{K[Pc[1].Value]}; Pc++ * */
        OP_SETENV,		/* AC	Env{K[Pc[1].Value]} := Base[A]; Pc++ * */
        OP_GETUPVAL,	/* ABx	Base[A] := UpVal[Bx] */
        OP_SETUPVAL,	/* ABx	UpVal[Bx] := Base[A] */
        OP_CLOSE,		/* A	See Lua 5.1 OP_CLOSE */
        OP_GETENVM,		/* ABx	Base[A] := Env->MysteryValue ? K[Bx] : MultiIndex(Env, K, Pc[1].Value); Pc++ ** */
        OP_GETTABLE,	/* ABC	Base[A] := Base[B]{Base[C]} *** */
        OP_SETTABLEV,	/* ABC	Base[B]{Base[C]} := Base[A] *** */
        OP_GETTABLEK,	/* ABC	Base[A] := Base[B]{K[Pc[1].Value]}; Pc++ * */
        OP_SETTABLEK,	/* ABC	Base[B]{K[Pc[1].Value]} := Base[A]; Pc++ * */
        OP_GETTABLEN,	/* ABC	Base[A] := Base[B]{C} */
        OP_SETTABLEN,	/* ABC	Base[B]{C} := Base[A] */
        OP_CLOSURE,		/* ABx	Base[A] := NewClosure(P[Bx]) // Same as Lua 5.1 (pseudo-instructions, OP_MOVE = luaF_findupval, OP_GETUPVAL = cl->upvals[B], etc.) */
        OP_SELF,		/* ABC Base[A] := Base[B]{K[Pc[1].Value]}; Base[A+1] := Base[B]; Pc++; goto OP_CALL * */
        OP_CALL,		/* ABC	Same as Lua 5.1? todo */
        OP_RETURN,		/* AB	Same as Lua 5.1? todo */
        OP_JMP,			/* Bx	Pc += Bx */
        OP_JMPH,		/* Bx	if (G(L)->MysteryFunction) G(L)->MysteryFunction(L); goto OP_JMP ^ // NOTE: Pc is not incremented before going to OP_JMP */
        OP_TEST0,		/* AB 	if (!Base[A]) Pc += B; */
        OP_TEST1,		/* AB	if (Base[A]) Pc += B; */
        OP_EQ,			/* ABx	if (Base[A] == Base[Pc[1].Value]) Pc += Bx; else Pc++; ^^ */
        OP_LE,			/* ABx	if (Base[A] <= Base[Pc[1].Value]) Pc += Bx; else Pc++; ^^ */
        OP_LT,			/* ABx	if (Base[A] < Base[Pc[1].Value]) Pc += Bx; else Pc++; ^^ */
        OP_NEQ,			/* ABx	if (Base[A] != Base[Pc[1].Value]) Pc += Bx; else Pc++; ^^ */
        OP_GT,			/* ABx	if (Base[A] > Base[Pc[1].Value]) Pc += Bx; else Pc++; ^^ */
        OP_GE,			/* ABx	if (Base[A] >= Base[Pc[1].Value]) Pc += Bx; else Pc++; ^^ */
        OP_ADD,			/* ABC	Base[A] := Base[B] + Base[C] */
        OP_SUB,			/* ABC	Base[A] := Base[B] - Base[C] */
        OP_MUL,			/* ABC	Base[A] := Base[B] * Base[C] */
        OP_DIV,			/* ABC	Base[A] := Base[B] / Base[C] */
        OP_MOD,			/* ABC	Base[A] := Base[B] % Base[C] */
        OP_POW,			/* ABC	Base[A] := Base[B] ^ Base[C] */
        OP_ADDK,		/* ABC	Base[A] := Base[B] + K[C] */
        OP_SUBK,		/* ABC	Base[A] := Base[B] - K[C] */
        OP_MULK,		/* ABC	Base[A] := Base[B] * K[C] */
        OP_DIVK,		/* ABC	Base[A] := Base[B] / K[C] */
        OP_MODK,		/* ABC	Base[A] := Base[B] % K[C] */
        OP_POWK,		/* ABC	Base[A] := Base[B] ^ K[C] */
        OP_TESTSETAND,	/* ABC	Base[A] := Base[B] ? Base[C] : Base[B] */
        OP_TESTSETOR,	/* ABC	Base[A] := Base[B] ? Base[B] : Base[C] */
        OP_TESTSETANDK,	/* ABC	Base[A] := Base[B] ? K[C] : Base[B] */
        OP_TESTSETORK,	/* ABC	Base[A] := Base[B] ? Base[B] : K[C] */
        OP_CONCAT,		/* ABC	See Lua 5.1 OP_CONCAT */
        OP_NOT,			/* AB	Base[A] := !Base[B] */
        OP_UNM,			/* AB	Base[A] := -Base[B] */
        OP_LEN,			/* AB   Base[A] := #Base[B] */
        OP_NEWTABLE,	/* AB	Base[A] := CreateTable(Size: Pc[1].Value, Hash: B ? 1 << (B - 1) : 0); Pc++ */
        OP_LOADTABLE,	/* AB	Base[A] := K[Bx] // Loads (copies) a const table from K[Bx] */
        OP_SETLIST,		/* ABC	Base[A]{Idx+0}, ..., Base[A]{Idx+(C-2)} := Base[B+0], ..., Base[B+(C-2)]; Pc++ // where Idx = Pc[1].Value and C-1 = Size */
        OP_FORPREP,     /* AsBx if (Step == 0 ? Start < End : End < Start) Pc += sBx; // where Step = Base[A+1], Start = Base[A+2], and End = Base[A]. Jumps to exit loop.*/
        OP_FORLOOP,     /* AsBx It += Step; if (Step == 0 ? End <= It : It <= End) Pc += sBx; [4] // where Step = Base[A+1], It = Base[A+2], and End = Base[A]. Jumps to continue loop.*/
        OP_TFORLOOP,	/* AsBx	Pc++; if (!IteratorCall(A, Pc[1].Value)) Pc += sBx; */
        OP_IPAIRSPREP,	/* AsBx	if (Base[A] == ipairsaux && Type(Base[A+1]) == "table" && Type(Base[A+2]) == "number") Base[A+2] = (lightuserdata)0; Pc += sBx; */
        OP_IPAIRSLOOP,	/* AsBx	[4][7] */
        OP_PAIRSPREP,	/* AsBx	if (Base[A] == luaB_next && Type(Base[A+1]) == "table" && Base[A+2] == nil) Base[A+2] = (lightuserdata)0; Pc += sBx; */
        OP_PAIRSLOOP,	/* AsBx	[4][8] */
        OP_VARARG,		/* AB See Lua 5.1 VARARG */
        OP_INIT,		/* N Clears the stack from L->top up to the end of the last function's stack frame */
        OP_INIT2,		/* A Clears the stack from L->top up to the end of the last function's stack frame, also prepares VARARG's (uses A for calc's, its p->numparams). */
        OP_LOADKX,		/* A Base[A] := K[Pc[1].Value]; Pc++ */
        OP_JMPHX		/* sAx Pc += sAx [4] */
    };
}


using namespace syn;

std::vector<syn::LuauInstruction> syn::OneWayLuauTranslator::Convert(Instruction* Instrs, const size_t Size) const
{


        struct RelocationData
    {
        int OriginalPc;
        int TranslatedPc;
        Instruction OriginalInstr;
        Instruction TranslatedInstr;
    };

    auto VanillaInstrs = std::vector<Instruction>(Instrs, Instrs + Size);
    auto PcTranslation = (int*)malloc(Size * 4);
    auto PcRelocationNeeded = std::vector<RelocationData>();

    //Stage 1 - Relocations.
    auto SkipPc = (bool*)malloc(Size);
    SecureZeroMemory(SkipPc, Size);

    for (size_t i = 0; i < VanillaInstrs.size(); i++)
    {
        //Check if the PC is marked to be skipped.
        if (SkipPc[i])
            continue;

        //Get instruction.
        const auto Instr = VanillaInstrs[i];
        const auto Opc = GET_OPCODE(Instr);

        if (Opc == OpCode::OP_SELF)
        {
            /*
                Problem: LuaU's OP_SELF requires a OP_CALL proceed it. Normal Lua does not do this, so we must relocate all OP_SELF's to where the OP_CALL is.
                [03] self       1   1   258  ; "HttpGet"		--> The OP_SELF
                [04] loadk      3   3        ; "hello world"
                [05] loadk      4   4        ; "same"
                [06] loadk      5   5        ; 123
                [07] getglobal  6   1        ; game
                [08] call       6   1   0
                [09] call       1   0   0						--> The actual call
                After relocation:
                [03] loadk      3   3        ; "hello world"
                [04] loadk      4   4        ; "same"
                [05] loadk      5   5        ; 123
                [06] getglobal  6   1        ; game
                [07] call       6   1   0
                [08] self       1   1   258  ; "HttpGet"		--> The OP_SELF
                [09] call       1   0   0						--> The actual call
                We can match up the OP_SELF by looking for the OP_CALL with the same A register in it after the self.
             */

             //Get our call target.
            auto CallI = i;
            auto CallTarget = VanillaInstrs[CallI];
            for (; !(GET_OPCODE(CallTarget) == OpCode::OP_CALL && GETARG_A(CallTarget) == GETARG_A(Instr));)
                CallTarget = VanillaInstrs[CallI++];

            //Insert our OP_SELF before the call instruction.
            VanillaInstrs.insert(VanillaInstrs.begin() + (CallI - 1), Instr);

            //Remove the old OP_SELF.
            VanillaInstrs.erase(VanillaInstrs.begin() + i);

            //Mark the new OP_SELF to be skipped.
            SkipPc[CallI - 2] = true;
        }
    }

    //De-allocate the SkipPc table.
    free(SkipPc);

    //Stage 2 - Translation.
    auto LuaUInstrs = std::vector<LuauInstruction>();

    LuauInstruction InitInstr(0);

    if (CurrentProto->lastlinedefined == 1)
    {
        InitInstr.SetOpCode(LuauOp::OP_INIT2);
        InitInstr.A = CurrentProto->numparams;
        LuaUInstrs.push_back(InitInstr);
    }
    else
    {
        InitInstr.SetOpCode(LuauOp::OP_INIT);
        LuaUInstrs.push_back(InitInstr);
    }

    for (size_t i = 0; i < VanillaInstrs.size(); i++)
    {
        LuauInstruction LuaUInstr(0);
        const auto Instr = VanillaInstrs[i];
        const auto Opc = GET_OPCODE(Instr);

        //Explicitly not - 1 as it will be the next instruction pushed.
        PcTranslation[i] = (int)LuaUInstrs.size();

        if (Opc == OpCode::OP_MOVE)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_MOVE);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.Bx = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_LOADK)
        {
            const auto Const = &CurrentProto->k[GETARG_Bx(Instr)];

            if (Const->tt == LUA_TNUMBER
                && nvalue(Const) < 65535
                && nvalue(Const) > 0
                && GET_OPCODE(VanillaInstrs[i + 1]) != OpCode::OP_SELF)
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADINT);
                LuaUInstr.A = GETARG_A(Instr);
                LuaUInstr.Bx = static_cast<uint16_t>(nvalue(Const));
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = GETARG_A(Instr);
                LuaUInstr.Bx = GETARG_Bx(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_LOADBOOL)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_LOADBOOL);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);
            LuaUInstr.C = GETARG_C(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_LOADNIL)
        {
            //LuaU's OP_LOADNIL only does a single register... we need to do duplications (facepalm)

            auto A = GETARG_A(Instr);
            auto B = GETARG_B(Instr);

            LuaUInstr.SetOpCode(LuauOp::OP_LOADNIL);

            while (B >= A)
            {
                LuaUInstr.A = B--;
                LuaUInstrs.push_back(LuaUInstr);
            }
        }

        else if (Opc == OpCode::OP_GETUPVAL)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_GETUPVAL);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.Bx = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_GETGLOBAL)
        {
            //"OP_GETGLOBAL" was replaced by "OP_GETENV" in LuaU, practically does the exact same thing - has a psuedoinstruction instead of the Bx though. (cuz we really need more psuedoinstructions!)

            LuaUInstr.SetOpCode(LuauOp::OP_GETENV);
            LuaUInstr.A = GETARG_A(Instr);

            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(GETARG_Bx(Instr));
        }

        else if (Opc == OpCode::OP_GETTABLE)
        {
            //"OP_GETTABLE" is split up into 3 different versions in LuaU - OP_GETTABLE, OP_GETTABLEK, and OP_GETTABLEN. OP_GETTABLE and OP_GETTABLEK are the only instructions actually relevant here.

            if (ISK(GETARG_C(Instr)))
            {
                //Use OP_GETTABLEK.

                LuaUInstr.SetOpCode(LuauOp::OP_GETTABLEK);
                LuaUInstr.A = GETARG_A(Instr);
                LuaUInstr.B = GETARG_B(Instr);

                LuaUInstrs.push_back(LuaUInstr);
                LuaUInstrs.emplace_back(GETARG_C(Instr) - 256);
            }
            else
            {
                //Use OP_GETTABLE.

                LuaUInstr.SetOpCode(LuauOp::OP_GETTABLE);
                LuaUInstr.A = GETARG_A(Instr);
                LuaUInstr.B = GETARG_B(Instr);
                LuaUInstr.C = GETARG_C(Instr);

                LuaUInstrs.push_back(LuaUInstr);
            }
        }

        else if (Opc == OpCode::OP_SETGLOBAL)
        {
            //The same routine as OP_GETGLOBAL can be done here, just with OP_SETENV instead.

            LuaUInstr.SetOpCode(LuauOp::OP_SETENV);
            LuaUInstr.A = GETARG_A(Instr);

            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(GETARG_Bx(Instr));
        }

        else if (Opc == OpCode::OP_SETUPVAL)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_SETUPVAL);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.Bx = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_SETTABLE)
        {
            //OP_SETTABLE can use A as either a constant or a register - LuaU only supports registers. A temp instruction is needed.

            auto RealSet = GETARG_C(Instr);
            if (ISK(RealSet))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = RealSet - 256;

                LuaUInstrs.push_back(LuaUInstr);

                RealSet = 254;
            }

            if (ISK(GETARG_B(Instr)))
            {
                //Use OP_SETTABLEK.

                LuaUInstr.SetOpCode(LuauOp::OP_SETTABLEK);
                LuaUInstr.A = RealSet;
                LuaUInstr.B = GETARG_A(Instr);
                LuaUInstr.C = 0;

                LuaUInstrs.push_back(LuaUInstr);
                LuaUInstrs.emplace_back(GETARG_B(Instr) - 256);
            }
            else
            {
                //Use OP_SETTABLEV.

                LuaUInstr.SetOpCode(LuauOp::OP_SETTABLEV);
                LuaUInstr.A = RealSet;
                LuaUInstr.B = GETARG_A(Instr);
                LuaUInstr.C = GETARG_B(Instr);

                LuaUInstrs.push_back(LuaUInstr);
            }
        }

        else if (Opc == OpCode::OP_NEWTABLE)
        {
            //More useless psuedoinstructions!

            LuaUInstr.SetOpCode(LuauOp::OP_NEWTABLE);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_C(Instr);

            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(GETARG_B(Instr));
        }

        else if (Opc == OpCode::OP_SELF)
        {
            //We don't need to relocate here, its done before this code.
            if (ISK(GETARG_C(Instr)))
            {
                //We can use regular OP_SELF.

                LuaUInstr.SetOpCode(LuauOp::OP_SELF);
                LuaUInstr.A = GETARG_A(Instr);
                LuaUInstr.B = GETARG_B(Instr);

                LuaUInstrs.push_back(LuaUInstr);
                LuaUInstrs.emplace_back(GETARG_C(Instr) - 256);
            }
            else
            {
                //TODO: Synapse assumption - Register bound OP_SELF's are preceded by a OP_LOADK.

                const auto RInstr = LuaUInstrs.at(LuaUInstrs.size() - 1);
                const auto RealConst = RInstr.Bx;

                LuaUInstrs.pop_back();

                LuaUInstr.SetOpCode(LuauOp::OP_SELF);
                LuaUInstr.A = GETARG_A(Instr);
                LuaUInstr.B = GETARG_B(Instr);

                LuaUInstrs.push_back(LuaUInstr);
                LuaUInstrs.emplace_back(RealConst);
            }
        }

        else if (Opc == OpCode::OP_ADD)
        {
            auto CmpB = GETARG_B(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = GETARG_B(Instr) - 256;

                LuaUInstrs.push_back(LuaUInstr);

                CmpB = 254;
            }

            LuaUInstr.A = GETARG_A(Instr);

            if (ISK(GETARG_C(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_ADDK);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr) - 256;
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_ADD);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_SUB)
        {
            auto CmpB = GETARG_B(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = GETARG_B(Instr) - 256;

                LuaUInstrs.push_back(LuaUInstr);

                CmpB = 254;
            }

            LuaUInstr.A = GETARG_A(Instr);

            if (ISK(GETARG_C(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_SUBK);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr) - 256;
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_SUB);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_MUL)
        {
            auto CmpB = GETARG_B(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = GETARG_B(Instr) - 256;

                LuaUInstrs.push_back(LuaUInstr);

                CmpB = 254;
            }

            LuaUInstr.A = GETARG_A(Instr);

            if (ISK(GETARG_C(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_MULK);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr) - 256;
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_MUL);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_DIV)
        {
            auto CmpB = GETARG_B(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = GETARG_B(Instr) - 256;

                LuaUInstrs.push_back(LuaUInstr);

                CmpB = 254;
            }

            LuaUInstr.A = GETARG_A(Instr);

            if (ISK(GETARG_C(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_DIVK);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr) - 256;
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_DIV);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_MOD)
        {
            auto CmpB = GETARG_B(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = GETARG_B(Instr) - 256;

                LuaUInstrs.push_back(LuaUInstr);

                CmpB = 254;
            }

            LuaUInstr.A = GETARG_A(Instr);

            if (ISK(GETARG_C(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_MODK);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr) - 256;
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_MOD);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_POW)
        {
            auto CmpB = GETARG_B(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_LOADK);
                LuaUInstr.A = 254;
                LuaUInstr.Bx = GETARG_B(Instr) - 256;

                LuaUInstrs.push_back(LuaUInstr);

                CmpB = 254;
            }

            LuaUInstr.A = GETARG_A(Instr);

            if (ISK(GETARG_C(Instr)))
            {
                LuaUInstr.SetOpCode(LuauOp::OP_POWK);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr) - 256;
            }
            else
            {
                LuaUInstr.SetOpCode(LuauOp::OP_POW);
                LuaUInstr.B = CmpB;
                LuaUInstr.C = GETARG_C(Instr);
            }

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_UNM)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_UNM);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_NOT)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_NOT);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_LEN)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_LEN);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_CONCAT)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_CONCAT);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);
            LuaUInstr.C = GETARG_C(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_JMP)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_JMP);
            LuaUInstr.sBx = 0;

            LuaUInstrs.push_back(LuaUInstr);

            //Add relocation.
            RelocationData Relocation{};
            Relocation.OriginalPc = i;
            Relocation.TranslatedPc = LuaUInstrs.size() - 1;
            Relocation.OriginalInstr = (Instruction)Instr;
            Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
            PcRelocationNeeded.push_back(Relocation);
        }

        else if (Opc == OpCode::OP_EQ)
        {
            uint8_t RegB = GETARG_B(Instr);
            uint8_t RegC = GETARG_C(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuauInstruction Temp(0);
                Temp.SetOpCode(LuauOp::OP_LOADK);
                Temp.A = 254;
                Temp.Bx = RegB;

                LuaUInstrs.push_back(Temp);

                RegB = 254;
            }

            if (ISK(GETARG_C(Instr)))
            {
                LuauInstruction Temp(0);
                Temp.SetOpCode(LuauOp::OP_LOADK);
                Temp.A = 255;
                Temp.Bx = RegC;

                LuaUInstrs.push_back(Temp);

                RegC = 255;
            }

            if (GETARG_A(Instr))
                LuaUInstr.SetOpCode(LuauOp::OP_EQ);
            else
                LuaUInstr.SetOpCode(LuauOp::OP_NEQ);

            LuaUInstr.A = RegB;

            /* Always paired with a JMP in the vanilla compiler */
            LuaUInstr.sBx = GETARG_sBx(VanillaInstrs[++i]) + 1;

            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(RegC);
        }

        else if (Opc == OpCode::OP_LT)
        {
            uint8_t RegB = GETARG_B(Instr);
            uint8_t RegC = GETARG_C(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuauInstruction Temp(0);
                Temp.SetOpCode(LuauOp::OP_LOADK);
                Temp.A = 254;
                Temp.Bx = RegB;

                LuaUInstrs.push_back(Temp);

                RegB = 254;
            }

            if (ISK(GETARG_C(Instr)))
            {
                LuauInstruction Temp(0);
                Temp.SetOpCode(LuauOp::OP_LOADK);
                Temp.A = 255;
                Temp.Bx = RegC;

                LuaUInstrs.push_back(Temp);

                RegC = 255;
            }

            if (GETARG_A(Instr))
                LuaUInstr.SetOpCode(LuauOp::OP_LT);
            else
                LuaUInstr.SetOpCode(LuauOp::OP_GT);

            LuaUInstr.A = RegB;

            /* Always paired with a JMP in the vanilla compiler */
            LuaUInstr.sBx = GETARG_sBx(VanillaInstrs[++i]) + 1;
            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(RegC);
        }

        else if (Opc == OpCode::OP_LE)
        {
            uint8_t RegB = GETARG_B(Instr);
            uint8_t RegC = GETARG_C(Instr);

            if (ISK(GETARG_B(Instr)))
            {
                LuauInstruction Temp(0);
                Temp.SetOpCode(LuauOp::OP_LOADK);
                Temp.A = 254;
                Temp.Bx = RegB;

                LuaUInstrs.push_back(Temp);

                RegB = 254;
            }

            if (ISK(GETARG_C(Instr)))
            {
                LuauInstruction Temp(0);
                Temp.SetOpCode(LuauOp::OP_LOADK);
                Temp.A = 255;
                Temp.Bx = RegC;

                LuaUInstrs.push_back(Temp);

                RegC = 255;
            }

            if (GETARG_A(Instr))
                LuaUInstr.SetOpCode(LuauOp::OP_LE);
            else
                LuaUInstr.SetOpCode(LuauOp::OP_GE);

            LuaUInstr.A = RegB;

            /* Always paired with a JMP in the vanilla compiler */
            LuaUInstr.sBx = GETARG_sBx(VanillaInstrs[++i]) + 1;

            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(RegC);
        }

        else if (Opc == OpCode::OP_TEST)
        {
            if (GETARG_C(Instr))
                LuaUInstr.SetOpCode(LuauOp::OP_TEST1);
            else
                LuaUInstr.SetOpCode(LuauOp::OP_TEST0);

            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = 1;

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_TESTSET)
        {
            /*
                Reimplementation is required for this instruction - the built in instructions are way too limited.
                We can use this combo to recreate it:
                [1] test0/test1 <b> 2
                [2] move <a> <b>
                [3] jmp <value> --> From the original TESTSET assumption.
                [4] <next instr>
            */

            if (GETARG_C(Instr))
                LuaUInstr.SetOpCode(LuauOp::OP_TEST1);
            else
                LuaUInstr.SetOpCode(LuauOp::OP_TEST0);

            LuaUInstr.A = GETARG_B(Instr);
            LuaUInstr.B = 2;

            LuaUInstrs.push_back(LuaUInstr);

            LuaUInstr.SetOpCode(LuauOp::OP_MOVE);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.Bx = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_CALL || Opc == OpCode::OP_TAILCALL)
        {
            //Tailcalls don't exist within LuaU. We don't need to deal with the psuedo-instructions either, they are handled fine by the regular loop.

            LuaUInstr.SetOpCode(LuauOp::OP_CALL);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);
            LuaUInstr.C = GETARG_C(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_RETURN)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_RETURN);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);
            LuaUInstr.C = GETARG_C(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_FORLOOP)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_FORLOOP);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.sBx = 0;

            LuaUInstrs.push_back(LuaUInstr);

            //Add relocation.
            RelocationData Relocation{};
            Relocation.OriginalPc = i;
            Relocation.TranslatedPc = LuaUInstrs.size() - 1;
            Relocation.OriginalInstr = (Instruction)Instr;
            Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
            PcRelocationNeeded.push_back(Relocation);
        }

        else if (Opc == OpCode::OP_FORPREP)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_FORPREP);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.sBx = 0;

            LuaUInstrs.push_back(LuaUInstr);

            //Add relocation.
            RelocationData Relocation{};
            Relocation.OriginalPc = i;
            Relocation.TranslatedPc = LuaUInstrs.size() - 1;
            Relocation.OriginalInstr = (Instruction)Instr;
            Relocation.TranslatedInstr = (Instruction)LuaUInstrs.at(LuaUInstrs.size() - 1);
            PcRelocationNeeded.push_back(Relocation);
        }

        else if (Opc == OpCode::OP_TFORLOOP)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_TFORLOOP);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.sBx = 1;

            LuaUInstrs.push_back(LuaUInstr);
            LuaUInstrs.emplace_back(GETARG_C(Instr));
        }

        else if (Opc == OpCode::OP_SETLIST)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_SETLIST);
            LuaUInstr.B = GETARG_A(Instr) + 1; /* Stack start (tbl + 1)  */
            LuaUInstr.C = GETARG_B(Instr) + 1; /* Size of table (b - 1)? */
            LuaUInstrs.push_back(LuaUInstr);

            /* Extended setlist */
            int c;
            if (GETARG_C(Instr) == 0)
                c = VanillaInstrs.at(i++);
            else
                c = GETARG_C(Instr);

            LuaUInstrs.emplace_back((c - 1) * LFIELDS_PER_FLUSH); /* Table start index */
        }

        else if (Opc == OpCode::OP_CLOSE)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_CLOSE);
            LuaUInstr.A = GETARG_A(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_CLOSURE)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_CLOSURE);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.Bx = GETARG_Bx(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }

        else if (Opc == OpCode::OP_VARARG)
        {
            LuaUInstr.SetOpCode(LuauOp::OP_VARARG);
            LuaUInstr.A = GETARG_A(Instr);
            LuaUInstr.B = GETARG_B(Instr);

            LuaUInstrs.push_back(LuaUInstr);
        }
    }

    //Stage 3 - Resolve secondary relocations.
    for (auto& Data : PcRelocationNeeded)
    {
        /*
            We resolve relocations by re-calculating the sBx offset after conversion is complete.
            We keep a table of the PC Vanilla -> PC LuaU offsets (PcTranslation) along with a 'needs-relocation' table (PcRelocationNeeded).
            We can then calculate the actual sBx parameter by getting the actual target instruction (from the PcTranslation table), then rebase the instruction off of that.
         */

        const auto Target = PcTranslation[Data.OriginalPc + GETARG_sBx(Data.OriginalInstr) + 1];
        const auto Sbx = Target - Data.TranslatedPc - 1;

        if (Sbx > 32768 || Sbx < -32768)
        {
            throw std::exception("todo: large sBx for translation not supported yet");
        }
        else
        {
            LuauInstruction RInstr(Data.TranslatedInstr);
            RInstr.sBx = Sbx;

            LuaUInstrs[Data.TranslatedPc] = RInstr;
        }
    }

    //Free PcTranslation table.
    free(PcTranslation);


        return LuaUInstrs;
}