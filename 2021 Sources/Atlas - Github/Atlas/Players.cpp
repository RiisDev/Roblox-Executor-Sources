#include "RBXClasses.h"
#include "Memory.h"

Player Players::GetLocalPlayer() {
	return Player(Memory::GetPointerAddress(this->Address + 0x110));
}

std::vector<Player> Players::GetPlayers() {
	std::vector<Player> players = {};

	std::vector<Instance> children = this->GetChildren();

	for (Instance child : children) {
		if (child.GetClassType() == "Player") {
			players.push_back(Player(child.Address));
		}
	}

	return players;
}