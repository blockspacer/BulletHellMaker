#include "Attack.h"
#include "EditorMovablePoint.h"

EditorAttack::EditorAttack(int id) : id(id) {
	nextEMPID = 0;
	mainEMP = std::make_shared<EditorMovablePoint>(nextEMPID);
}

std::string EditorAttack::format() {
	std::string res = "";
	res += "(" + tos(id) + ")" + delim;
	res += "(" + name + ")" + delim;
	res += "(" + mainEMP->format() + ")" + delim;
	if (playAttackAnimation) {
		res += "1";
	} else {
		res += "0";
	}
	for (auto p : soundEffectNames) {
		res += delim + "(" + tos(p.first) + ")" + delim + "(" + p.second + ")";
	}
	return res;
}

void EditorAttack::load(std::string formattedString) {
	auto items = split(formattedString, DELIMITER);
	id = stoi(items[0]);
	name = items[1];
	mainEMP = std::make_shared<EditorMovablePoint>(nextEMPID);
	mainEMP->load(items[2]);
	if (items[3] == "1") {
		playAttackAnimation = true;
	} else {
		playAttackAnimation = false;
	}
	for (int i = 4; i < items.size() + 4; i += 2) {
		soundEffectNames.push_back(std::make_pair(std::stof(items[i]), items[i + 1]));
	}
}

bool EditorAttack::legal(SpriteLoader& spriteLoader, std::string& message) {
	bool good = true;
	if (contains(name, '(') || contains(name, ')')) {
		message += "Attack \"" + name + "\" cannot have the character '(' or ')' in its name\n";
		good = false;
	}
	for (auto p : soundEffectNames) {
		if (contains(p.second, '(') || contains(p.second, ')')) {
			message += "Sound effect \"" + p.second + "\" cannot have the character '(' or ')' in its name\n";
			good = false;
		}
	}
	if (mainEMP) {
		if (!mainEMP->legal(spriteLoader, message)) {
			good = false;
		}
	} else {
		message += "Attack \"" + name + "\" is missing its mainEMP\n";
	}
	//TODO: check that each sound file is a valid file
	return good;
}

void EditorAttack::executeAsEnemy(EntityCreationQueue& queue, SpriteLoader& spriteLoader, entt::DefaultRegistry& registry, uint32_t entity, float timeLag, int attackPatternID, int enemyID, int enemyPhaseID) {
	queue.pushBack(std::make_unique<EMPSpawnFromEnemyCommand>(registry, spriteLoader, mainEMP, entity, timeLag, id, attackPatternID, enemyID, enemyPhaseID, playAttackAnimation));
}

void EditorAttack::executeAsPlayer(EntityCreationQueue & queue, SpriteLoader & spriteLoader, entt::DefaultRegistry & registry, uint32_t entity, float timeLag, int attackPatternID, int enemyID, int enemyPhaseID) {
	queue.pushBack(std::make_unique<EMPSpawnFromPlayerCommand>(registry, spriteLoader, mainEMP, entity, timeLag, id, attackPatternID, playAttackAnimation));
}

float EditorAttack::searchLargestHitbox() {
	return mainEMP->searchLargestHitbox();
}

std::shared_ptr<EditorMovablePoint> EditorAttack::searchEMP(int id) {
	if (mainEMP->getID() == id) {
		return mainEMP;
	}
	return mainEMP->searchID(id);
}
