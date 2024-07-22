#ifndef XVALUE_INCLUDED //Can be used to determine if ValueX is included or not.
#define XVALUE_INCLUDED //If it isn’t, include it and define that it’s included.
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

/* Data Types (DT) */
#define XVAL_VSTRING 8 //String data type
#define XVAL_VINT 7 //Integer data type
#define XVAL_VBOOL 6 //Bool data type
#define XVAL_VLONG 5 //Long data type
#define XVAL_VDWORD 4 //DWORD data type
#define XVAL_VVOIDPTR 3 //void* data type

#define XVAL_VNAME (-1) //Name property of xvalue

#define XVAL_VPVAL 1 //Plain value data type
#define XVAL_VNONE 0 //Standing for the null or no value for ValueX
/* End (DT) */

#define XVAL_FAILURE -1
#define XVAL_SUCCESS -2

struct xvalue {
	int XVAL_TYPE; //Holds the type value (so I can identify the value)
	int XVAL_INT; //Holds the integer value if it is an integer, otherwise, set to the default NULLOBJECT int value
	std::string XVAL_STRING; //Holds the string value if it is a string, otherwise, set to the default NULLOBJECT string value
	bool XVAL_BOOL; //Holds the bool value if its a bool, otherwise, set to the default NULLOBJECT bool value
	long XVAL_LONG; //Holds the long value if its a long, otherwise, set to the default NULLOBJECT long value
	DWORD XVAL_DWORD; //Holds the DWORD value if its a DWORD, otherwise, set to the default NULLOBJECT DWORD value
	void* XVAL_VOIDPTR; //Holds the void* value if its a void*, otherwise, set to the default NULLOBJECT void* value

	std::string XVAL_NAME; //Holds the variable name
};

struct value { //For the return of functions so I can basically use this as an "auto" so I don't have to vary functions to get different types.
	int XVAL_RET; //Holds either XVAL_SUCCESS or XVAL_FAILURE, for function purposes
	xvalue XVAL; //Holds the XValue
};

struct inval {
	int XVAL_INT; //Holds the integer value if it is an integer, otherwise, its nothing
	std::string XVAL_STRING; //Holds the string value if it is a string, otherwise, its nothing
	bool XVAL_BOOL; //Holds the bool value if its a bool, otherwise, its nothing
	long XVAL_LONG; //Holds the long value if its a long, otherwise, its nothing
	DWORD XVAL_DWORD; //Holds the DWORD value if its a DWORD, otherwise, its nothing
	void* XVAL_VOIDPTR; //Holds the void* value if its a void*, otherwise, its nothing
};

xvalue XVAL_NULLOBJECT; //Later set, but to an xvalue with no value

std::string XVAL_NULLOBJ_STRING = "_XVAL_NULLOBJ"; //NULL Object string identifier

class ValueX {
private:
	std::vector<xvalue> XVAL_VARS; //Holds all variables, clean, and easy to manage later on
public:
	ValueX() {
		xvalue xval;
		xval.XVAL_TYPE = XVAL_VNONE; //Set to "NONE" even though it has value, by default this is how a NULL object will be created
		xval.XVAL_INT = 0;
		xval.XVAL_STRING = XVAL_NULLOBJ_STRING; //Normally would set to "" but I put "_XVAL_NULLOBJ" so it is easy to identify if its a NULL Object
		xval.XVAL_BOOL = false;
		xval.XVAL_LONG = 0x0;
		xval.XVAL_DWORD = 0x0;
		xval.XVAL_VOIDPTR = nullptr;
		xval.XVAL_NAME = XVAL_NULLOBJ_STRING; //Normally would be set to "" but I put "_XVAL_NULLOBJ" so it is easy to identify if its a NULL Object

		XVAL_NULLOBJECT = xval; //Set the NULLOBJECT to be my version of a NULL value
	}

	int getVariableCount() { return XVAL_VARS.size(); }

	bool isNullObject(xvalue xval) {
		if (xval.XVAL_BOOL == XVAL_NULLOBJECT.XVAL_BOOL && xval.XVAL_INT == XVAL_NULLOBJECT.XVAL_INT && xval.XVAL_LONG == XVAL_NULLOBJECT.XVAL_LONG && xval.XVAL_NAME == XVAL_NULLOBJECT.XVAL_NAME && xval.XVAL_STRING == XVAL_NULLOBJECT.XVAL_STRING && xval.XVAL_TYPE == XVAL_NULLOBJECT.XVAL_TYPE)
			return true;
		else
			return false;
	}

	int getType(xvalue xval) {
		return xval.XVAL_TYPE;
	}

	std::string variableTypeToString(int XVAL_TYPE) {
		if (XVAL_TYPE == XVAL_VSTRING)
			return "XVAL_VSTRING"; //std::string
		if (XVAL_TYPE == XVAL_VBOOL)
			return "XVAL_VBOOL"; //bool
		if (XVAL_TYPE == XVAL_VINT)
			return "XVAL_VINT"; //int
		if (XVAL_TYPE == XVAL_VLONG)
			return "XVAL_VLONG"; //long
		if (XVAL_TYPE == XVAL_VNAME)
			return "XVAL_VNAME"; //Name
		if (XVAL_TYPE == XVAL_VPVAL)
			return "XVAL_VPVAL"; //Raw xval
		if (XVAL_TYPE == XVAL_VDWORD)
			return "XVAL_VDWORD"; //DWORD
		if (XVAL_TYPE == XVAL_VVOIDPTR)
			return "XVAL_VVOIDPTR"; //void*
	}

	int getVariableIndex(xvalue xval) {
		for (int i = 0; i < XVAL_VARS.size(); i++) {
			xvalue xval_i = XVAL_VARS.at(i);
			if (xval_i.XVAL_NAME == xval_i.XVAL_NAME)
				return i;
		}
	}

	value getRawValue(int XVAL_TYPE, xvalue xval) {
		value ret;
		ret.XVAL_RET = XVAL_FAILURE;

		try {
			if (XVAL_TYPE == XVAL_VSTRING)
				ret.XVAL.XVAL_STRING = xval.XVAL_STRING; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //std::string
			if (XVAL_TYPE == XVAL_VBOOL)
				ret.XVAL.XVAL_BOOL = xval.XVAL_BOOL; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //bool
			if (XVAL_TYPE == XVAL_VINT)
				ret.XVAL.XVAL_INT = xval.XVAL_INT; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //int
			if (XVAL_TYPE == XVAL_VLONG)
				ret.XVAL.XVAL_LONG = xval.XVAL_LONG; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //long
			if (XVAL_TYPE == XVAL_VNAME)
				ret.XVAL.XVAL_NAME = xval.XVAL_NAME; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //Name
			if (XVAL_TYPE == XVAL_VPVAL)
				ret.XVAL = xval; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //Raw xval
			if (XVAL_TYPE == XVAL_VDWORD)
				ret.XVAL.XVAL_DWORD = xval.XVAL_DWORD; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //DWORD
			if (XVAL_TYPE == XVAL_VVOIDPTR)
				ret.XVAL.XVAL_VOIDPTR = xval.XVAL_VOIDPTR; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //void*
		}
		catch (...) {}

		return ret;
	}

	xvalue findXValue(std::string name) {
		try {
			for (int i = 0; i < XVAL_VARS.size(); i++) {
				xvalue xval = XVAL_VARS.at(i);
				if (xval.XVAL_NAME == name)
					return xval;
			}
		}
		catch (...) {
			return XVAL_NULLOBJECT;
		}
	}

	value getValue(int XVAL_TYPE, std::string name) {
		xvalue xval = findXValue(name);
		value ret;
		ret.XVAL_RET = XVAL_FAILURE;

		try {
			if (XVAL_TYPE == XVAL_VSTRING)
				ret.XVAL.XVAL_STRING = xval.XVAL_STRING; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //std::string
			if (XVAL_TYPE == XVAL_VBOOL)
				ret.XVAL.XVAL_BOOL = xval.XVAL_BOOL; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //bool
			if (XVAL_TYPE == XVAL_VINT)
				ret.XVAL.XVAL_INT = xval.XVAL_INT; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //int
			if (XVAL_TYPE == XVAL_VLONG)
				ret.XVAL.XVAL_LONG = xval.XVAL_LONG; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //long
			if (XVAL_TYPE == XVAL_VNAME)
				ret.XVAL.XVAL_NAME = xval.XVAL_NAME; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //Name
			if (XVAL_TYPE == XVAL_VPVAL)
				ret.XVAL = xval; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //Raw xval
			if (XVAL_TYPE == XVAL_VDWORD)
				ret.XVAL.XVAL_DWORD = xval.XVAL_DWORD; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //DWORD
			if (XVAL_TYPE == XVAL_VVOIDPTR)
				ret.XVAL.XVAL_VOIDPTR = xval.XVAL_VOIDPTR; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //void*
		}
		catch (...) {}

		return ret;
	}

	value getVariable(std::string name) {
		value ret;
		ret.XVAL_RET = XVAL_FAILURE;
		xvalue xval = findXValue(name);

		try {
			value ret2 = getValue(xval.XVAL_TYPE, name);
			if (ret2.XVAL.XVAL_NAME == xval.XVAL_NAME && ret2.XVAL.XVAL_TYPE == xval.XVAL_TYPE)
				ret.XVAL = xval;
			ret.XVAL_RET = ret2.XVAL_RET;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	value getVariable(int variable_index) {
		value ret;
		ret.XVAL_RET = XVAL_FAILURE;

		try {
			ret.XVAL = XVAL_VARS.at(variable_index);
			ret.XVAL_RET = XVAL_SUCCESS;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	value addValue(xvalue xval) {
		value ret;
		ret.XVAL_RET = XVAL_FAILURE;

		try {
			ret.XVAL = xval;
			XVAL_VARS.push_back(xval);
			ret.XVAL_RET = XVAL_SUCCESS;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	value removeValue(xvalue xval) {
		value ret;

		try {
			ret.XVAL_RET = XVAL_FAILURE;

			ret.XVAL = xval;
			XVAL_VARS.erase(XVAL_VARS.begin() + getVariableIndex(xval));
			ret.XVAL_RET = XVAL_SUCCESS;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	value removeValue(std::string name) {
		value ret;

		try {
			ret.XVAL_RET = XVAL_FAILURE;

			ret.XVAL = findXValue(name);
			XVAL_VARS.erase(XVAL_VARS.begin() + getVariableIndex(ret.XVAL));
			ret.XVAL_RET = XVAL_SUCCESS;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	value removeValue(int variable_index) {
		value ret;

		try {
			ret.XVAL_RET = XVAL_FAILURE;

			ret.XVAL = XVAL_VARS.at(variable_index);
			XVAL_VARS.erase(XVAL_VARS.begin() + getVariableIndex(ret.XVAL));
			ret.XVAL_RET = XVAL_SUCCESS;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	value createValue(int XVAL_TYPE, inval in, std::string name) {
		value ret;
		ret.XVAL_RET = XVAL_FAILURE;

		try {
			if (XVAL_TYPE == XVAL_VPVAL)
				throw std::exception("cannot create a value with type XVAL_UPVAL");
			if (XVAL_TYPE == XVAL_VNAME)
				throw std::exception("cannot create a value with type XVAL_NAME");

			if (XVAL_TYPE == XVAL_VSTRING) {
				ret.XVAL.XVAL_STRING = in.XVAL_STRING; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //std::string
				ret.XVAL.XVAL_BOOL = XVAL_NULLOBJECT.XVAL_BOOL;
				ret.XVAL.XVAL_INT = XVAL_NULLOBJECT.XVAL_INT;
				ret.XVAL.XVAL_LONG = XVAL_NULLOBJECT.XVAL_LONG;
				ret.XVAL.XVAL_DWORD = XVAL_NULLOBJECT.XVAL_DWORD;
				ret.XVAL.XVAL_VOIDPTR = XVAL_NULLOBJECT.XVAL_VOIDPTR;
			}
			if (XVAL_TYPE == XVAL_VBOOL) {
				ret.XVAL.XVAL_STRING = XVAL_NULLOBJECT.XVAL_STRING;
				ret.XVAL.XVAL_BOOL = in.XVAL_BOOL; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //bool
				ret.XVAL.XVAL_INT = XVAL_NULLOBJECT.XVAL_INT;
				ret.XVAL.XVAL_LONG = XVAL_NULLOBJECT.XVAL_LONG;
				ret.XVAL.XVAL_DWORD = XVAL_NULLOBJECT.XVAL_DWORD;
				ret.XVAL.XVAL_VOIDPTR = XVAL_NULLOBJECT.XVAL_VOIDPTR;
			}
			if (XVAL_TYPE == XVAL_VINT) {
				ret.XVAL.XVAL_STRING = XVAL_NULLOBJECT.XVAL_STRING;
				ret.XVAL.XVAL_BOOL = XVAL_NULLOBJECT.XVAL_BOOL;
				ret.XVAL.XVAL_INT = in.XVAL_INT; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //int
				ret.XVAL.XVAL_LONG = XVAL_NULLOBJECT.XVAL_LONG;
				ret.XVAL.XVAL_DWORD = XVAL_NULLOBJECT.XVAL_DWORD;
				ret.XVAL.XVAL_VOIDPTR = XVAL_NULLOBJECT.XVAL_VOIDPTR;
			}
			if (XVAL_TYPE == XVAL_VLONG) {
				ret.XVAL.XVAL_STRING = XVAL_NULLOBJECT.XVAL_STRING;
				ret.XVAL.XVAL_BOOL = XVAL_NULLOBJECT.XVAL_BOOL;
				ret.XVAL.XVAL_INT = XVAL_NULLOBJECT.XVAL_INT;
				ret.XVAL.XVAL_LONG = in.XVAL_LONG; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //long
				ret.XVAL.XVAL_DWORD = XVAL_NULLOBJECT.XVAL_DWORD;
				ret.XVAL.XVAL_VOIDPTR = XVAL_NULLOBJECT.XVAL_VOIDPTR;
			}
			if (XVAL_TYPE == XVAL_VDWORD) {
				ret.XVAL.XVAL_STRING = XVAL_NULLOBJECT.XVAL_STRING;
				ret.XVAL.XVAL_BOOL = XVAL_NULLOBJECT.XVAL_BOOL;
				ret.XVAL.XVAL_INT = XVAL_NULLOBJECT.XVAL_INT;
				ret.XVAL.XVAL_LONG = XVAL_NULLOBJECT.XVAL_LONG;
				ret.XVAL.XVAL_DWORD = in.XVAL_DWORD; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //DWORD
				ret.XVAL.XVAL_VOIDPTR = XVAL_NULLOBJECT.XVAL_VOIDPTR;
			}
			if (XVAL_TYPE == XVAL_VVOIDPTR) {
				ret.XVAL.XVAL_STRING = XVAL_NULLOBJECT.XVAL_STRING;
				ret.XVAL.XVAL_BOOL = XVAL_NULLOBJECT.XVAL_BOOL;
				ret.XVAL.XVAL_INT = XVAL_NULLOBJECT.XVAL_INT;
				ret.XVAL.XVAL_LONG = XVAL_NULLOBJECT.XVAL_LONG;
				ret.XVAL.XVAL_DWORD = XVAL_NULLOBJECT.XVAL_DWORD;
				ret.XVAL.XVAL_VOIDPTR = in.XVAL_VOIDPTR; ret.XVAL.XVAL_TYPE = XVAL_TYPE; ret.XVAL_RET = XVAL_SUCCESS; //void*
			}

			ret.XVAL.XVAL_NAME = name;
			XVAL_VARS.push_back(ret.XVAL);
			ret.XVAL_RET = XVAL_SUCCESS;
		}
		catch (...) {
			ret.XVAL_RET = XVAL_FAILURE;
		}

		return ret;
	}

	void printAllVariables() {
		for (int i = 0; i < getVariableCount(); i++) {
			xvalue xval = getVariable(i).XVAL;
			util->print("Variable %s {\n	type: %s\n", xval.XVAL_NAME.c_str(), variableTypeToString(xval.XVAL_TYPE).c_str());
			if (getType(xval) == XVAL_VSTRING)
				util->print("	value: %s\n}\n", xval.XVAL_STRING.c_str()); //std::string
			if (getType(xval) == XVAL_VBOOL)
				util->print("	value: %s\n}\n", (xval.XVAL_BOOL ? "true" : "false")); //bool
			if (getType(xval) == XVAL_VINT)
				util->print("	value: %i\n}\n", xval.XVAL_INT); //int
			if (getType(xval) == XVAL_VLONG)
				util->print("	value: %x\n}\n", xval.XVAL_LONG); //long
			if (getType(xval) == XVAL_VDWORD)
				util->print("	value: %x\n}\n", xval.XVAL_DWORD); //DWORD
			if (getType(xval) == XVAL_VVOIDPTR)
				util->print("	value: (adr)%x\n}\n", (DWORD)xval.XVAL_VOIDPTR); //void*
		}
	}
};
#endif

/*
  I should probably call a doctor for help.
*/
