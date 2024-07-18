#include "RBXClasses.h"
#include "Memory.h"

Instance DataModel::GetService(std::string className) {
	std::vector<Instance> children = this->GetChildren();

	for (Instance child : children) {
		if (child.GetClassType() == className) {
			return child;
		}
	}
}