
#include "Instance.h"

DWORD getParent(DWORD Instance)
{
	return *(DWORD*)(Instance + 0x34);
}

std::vector<DWORD> getChildren(DWORD Instance)
{
	std::vector<DWORD> childrenVector;
	DWORD childVector = *(DWORD*)(Instance + 44);
	DWORD childEnd = *(DWORD*)(childVector + 4);
	DWORD childStart = *(DWORD*)childVector;
	DWORD childMax = *(DWORD*)(childVector + 4) - *(DWORD*)(childVector) >> 3;
	DWORD childCount = 0;
	for (; childStart != childEnd; childStart += 8)
	{
		if (childCount == childMax)
			break;
		else
		{
			childCount++;
			childrenVector.push_back(*(DWORD*)childStart);
		}
	}
	return childrenVector;
}

DWORD getChildFromName(DWORD Instance, PCHAR InstanceName)
{
	std::vector<DWORD> childrenVector = getChildren(Instance);
	for (DWORD child : childrenVector)
		if (!strcmp(getInstanceName(child)->c_str(), InstanceName))
			return child;
	return NULL;
}

DWORD getChildFromClassName(DWORD Instance, PCHAR ClassName)
{
	std::vector<DWORD> childrenVector = getChildren(Instance);
	for (DWORD child : childrenVector)
		if (!strcmp(getInstanceClassName(child)->c_str(), ClassName))
			return child;
	return NULL;
}

std::string* getInstanceName(DWORD Instance)
{
	return *(std::string**)(Instance + 0x28);
}

std::string* getInstanceClassName(DWORD Instance)
{
	return *(std::string**)(*(DWORD*)(Instance + 0xC) + 4);
}


RBX::Instance::Instance()
{

}

RBX::Instance RBX::Instance::operator[](PCHAR ChildName)
{
	return this->FindFirstChild(ChildName);
}

RBX::Instance RBX::Instance::GetParent()
{
	 DWORD Parent = getParent(this->InstanceAddress);
	 if (Parent)
		 return RBX::Instance(Parent);
	 return NULL;
}

 std::vector<RBX::Instance> RBX::Instance::GetChildren()
 {
	 std::vector<RBX::Instance> Children;
	 for (DWORD Child : getChildren(this->InstanceAddress))
		 Children.push_back(RBX::Instance(Child));
	 return Children;
 }

 RBX::Instance RBX::Instance::FindFirstChild(PCHAR ChildName)
 {
	 DWORD Child = getChildFromName(this->InstanceAddress, ChildName);
	 if (Child)
		 return RBX::Instance(Child);
	 return NULL;
 }

 RBX::Instance RBX::Instance::FindFirstChildOfClass(PCHAR ClassName)
 {
	 DWORD Child = getChildFromClassName(this->InstanceAddress, ClassName);
	 if (Child)
		 return RBX::Instance(Child);
	 return NULL;
 }

 DWORD RBX::Instance::GetAddress()
 {
	 return this->InstanceAddress;
 }

 RBX::Instance::Instance(DWORD Address)
 {
	 this->InstanceAddress = Address;
	 this->Name = getInstanceName(Address);
	 this->ClassName = getInstanceClassName(Address);
 }
